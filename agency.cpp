#include "agency.h"
#include "customUtilities.h"

using namespace std;

Agency::Agency()
{
	name = "Undefined Name";
	url = "404";
	fileNameClients = "";
	fileNamePacks = "";
	address = Address();
	vat = 0;
	packList = {};
	clientList = {};
}

#pragma region GETTERS

string Agency::getName() const {
	return this->name;
}

unsigned Agency::getVAT() const {
	return this->vat;
}

Address Agency::getAddress() const {
	return this->address;
}

string Agency::getURL() const {
	return this->url;
}

vector<Client> Agency::getClients() const {
	return this->clientList;
}

vector<TravelPack> Agency::getPacksList() const {
	return this->packList;
}

string Agency::getFileNameClients() const {
	return this->fileNameClients;
}

string Agency::getFileNamePacks() const {
	return this->fileNamePacks;
}

#pragma endregion

#pragma region SETTERS

bool Agency::setName(std::string new_name) {
	if (new_name.empty()) return false;
	this->name = new_name;
	return true;
}

bool Agency::setVATnumber(unsigned new_VAT) {
	this->vat = new_VAT;
	return true;
}

bool Agency::setAddress(Address new_address) {
	this->address = new_address;
	return true;
}

bool Agency::setURL(std::string new_url) {
	if (new_url.size() == 0) return false;
	this->url = new_url;
	return true;
}

bool Agency::setClients(std::vector<Client> & new_clients)
{
	this->clientList = new_clients;
	return true;
}

bool Agency::setPackets(std::vector<TravelPack> & new_packList) {
	this->packList = new_packList;
	return true;
}

#pragma endregion

// READ METHODS

bool Agency::loadData(const std::string & agencyFileName, const bool isVerbose)
{
	ifstream file;
	unsigned lineTracker = 0;
	file.open(agencyFileName);
	if (!file) // if file does not exist
	{
		if (isVerbose)
			cerr << "ERROR: Agency file \"" << agencyFileName << "\" does not exist" << endl;
		return false;
	}
	if (cu::isFileEmpty(file))
	{
		if (isVerbose)
			cerr << "ERROR: Agency file is empty" << endl;
		file.close();
		return false;
	}

	if (!this->readAgencyFromFile(file, lineTracker))
	{
		if (isVerbose)
			cerr << "ERROR: Wrong input at line " << lineTracker << " of file \"" << agencyFileName << "\"" << endl;
		file.close();
		return false;
	}

	file.close();

	file.open(this->fileNamePacks);
	if (!file) // if file does not exist
	{
		if (isVerbose)
			cerr << "ERROR: Packs file \"" << this->fileNamePacks << "\" does not exist, shutting down..." << endl;
		file.close();
		exit(42);
		return false; // Should never execute
	}
	lineTracker = 0;
	if (!this->readAllPacksFromFile(file, lineTracker))
	{
		if (isVerbose)
			cerr << "ERROR: Wrong input at line " << lineTracker << " of file \"" << this->fileNamePacks << "\"" << endl;
		file.close();
		return false;
	}
	file.close();

	file.open(this->fileNameClients);
	if (!file) // if file does not exist
	{
		if (isVerbose)
			cerr << "ERROR: Clients file \"" << this->fileNameClients << "\" does not exist, shutting down..." << endl;
		file.close();
		exit(42);
		return false; // Should never execute
	}
	lineTracker = 0;
	if (!this->readAllClientsFromFile(file, lineTracker))
	{
		if (isVerbose)
			cerr << "ERROR: Wrong input at line " << lineTracker << " of file \"" << this->fileNameClients << "\"" << endl;
		file.close();
		return false;
	}
	file.close();

	if (isVerbose)
		cout << "Agency data loaded correctly" << endl;

	return true;
}

bool Agency::readAgencyFromFile(ifstream & file, unsigned & lineTracker) {
	Address address;

	getline(file, name);
	if (file.eof()) return false;
	lineTracker++;

	file >> vat;
	if (file.eof()) return false;
	file.ignore(1000, '\n');
	lineTracker++;

	getline(file, url);
	if (file.eof()) return false;
	lineTracker++;

	if (!address.readFromFile(file, lineTracker)) return false;

	getline(file, fileNameClients);
	if (file.eof()) return false;
	lineTracker++;

	getline(file, fileNamePacks);
	lineTracker++;

	return true;
}

bool Agency::readNewClientUserInput() {
	string str;
	Client newClient;
	Address auxAddress;

	do {
		if (!cu::readStr(str, "Name")) return false;
	} while (!newClient.setName(str));

	do {
		if (!cu::readStr(str, "VAT")) return false;
	} while (!newClient.setVAT(stoi(str), clientList));

	do {
		if (!cu::readStr(str, "Household")) return false;
	} while (!newClient.setHousehold(stoi(str)));

	do
	{
		cout << "Address:" << endl;
		if (!auxAddress.readUserInput())
			return false;
	} while (!newClient.setAddress(auxAddress));

	this->clientList.push_back(newClient);

	return true;
}

bool Agency::readAllClientsFromFile(std::ifstream & file, unsigned & lineTracker)
{
	if (cu::isFileEmpty(file))
	{
		cout << "WARNING: Clients file \"" << this->fileNameClients << "\" is empty" << endl;
		cout << "Assuming there are no clients" << endl;
		this->packList = {};
		return true;
	}

	while (true)
	{
		Client aux;
		if (!this->readClientFromFile(file, aux, lineTracker))
			return false;

		this->clientList.push_back(aux);

		if (file.peek() != ':')
			break;

		file.ignore(1000, '\n');
	}
	return true;
}

bool Agency::readNewPackUserInput()
{
	string s;
	int n;
	vector<string> destinations = {};
	Date date;
	TravelPack pack;

	if (!cu::readStr(s, "Main destination"))
		return false;
	destinations.push_back(s);

	char c = 'o';
	if (!cu::readConfirmation(c, "Insert secondary destinations"))
		return false;

	if (c == 'y' || c == 'Y')
	{
		cout << "Write \"stop\" to finish inputting secondary destinations" << endl;
		do
		{
			if (!cu::readStr(s, "Destinations"))
				return false;
			destinations.push_back(s);
		} while (s != "stop");
		destinations.pop_back(); // Removes the extra "stop"
	}
	if (!pack.setDestinations(destinations))
		return false; // Extra safety, should never return false

	do
	{
		cout << "Departure date:" << endl;
		if (!date.readUserInput())
			return false;
	} while (!pack.setDeparture(date));

	do
	{
		cout << "Return date:" << endl;
		if (!date.readUserInput())
			return false;
	} while (!pack.setReturn(date));

	do
	{
		if (!cu::readInt(n, "Price"))
			return false;
	} while (!pack.setPrice(n));

	pack.setCurrentBookings(0);
	do
	{
		if (!cu::readInt(n, "Maximum bookings"))
			return false;
	} while (!pack.setMaxBookings(n));

	pack.setId(this->maxPackId);
	this->maxPackId++;

	return true;
}

bool Agency::readAllPacksFromFile(std::ifstream & file, unsigned & lineTracker)
{
	if (cu::isFileEmpty(file))
	{
		cout << "WARNING: Packs file \"" << this->fileNamePacks << "\" is empty" << endl;
		cout << "Assuming there are no travel packs" << endl;
		this->packList = {};
		return true;
	}

	int n;
	file >> n;
	if (file.eof() || file.fail()) return false;
	this->maxPackId = abs(n);
	lineTracker++;

	while (true)
	{
		TravelPack aux;
		if (!this->readPackFromFile(file, aux, lineTracker))
			return false;

		this->packList.push_back(aux);

		if (file.peek() != ':')
			break;

		file.ignore(1000, '\n');
	}
	return true;
}


// CONSOLE OUTPUT METHODS

void Agency::printClientByVAT(unsigned vat) const {

	if (this->clientList.empty()) {
		cout << "No registered clients" << endl;
		return;
	}

	for (size_t i=0; i < this->clientList.size(); i++) {
		if (this->clientList.at(i).getVAT() == vat) {
			cout << this->clientList.at(i) << endl;
			return;
		}
	}

	cout << "Client with VAT " << vat << " not found" << endl;
	return;
}

void Agency::printClients() const
{
	if (this->clientList.empty())
	{
		cout << "No registered clients" << endl;
		return;
	}

	cout << this->clientList.front() << endl;
	for (size_t i = 1; i < this->clientList.size(); i++)
	{
		cout << CLIENT_OUTPUT_SEPARATOR << endl;
		cout << this->clientList.at(i) << endl;
	}
}

void Agency::printPacks() const
{
	if (this->packList.empty())
	{
		cout << "No registered packs" << endl;
		return;
	}

	cout << this->packList.front() << endl;
	for (size_t i = 1; i < this->packList.size(); i++)
	{
		cout << PACK_OUTPUT_SEPARATOR << endl;
		cout << this->packList.at(i) << endl;
	}
}

void Agency::printPacksByDestination(const string & s) const
{
	string sl = s, aux;
	cu::strTrim(sl);
	cu::strLower(sl);
	if (this->packList.size() == 0)
	{
		cout << "No registered packs" << endl;
		return;
	}

	int count = 0;
	for (size_t i = 0; i < this->packList.size(); i++)
	{
		for (size_t j = 0; j < this->packList.at(i).getDestinationsSize(); j++)
		{
			aux = this->packList.at(i).getDestinationAt(j);
			cu::strLower(aux);
			if (aux.find(sl) != string::npos)
			{
				if (count == 0)
				{
					cout << this->packList.at(i) << endl;
					count++;
				}
				else
				{
					cout << PACK_OUTPUT_SEPARATOR << endl;
					cout << this->packList.at(i) << endl;
				}
			}
		}
	}
	if (count == 0)
		cout << "No packs matching the location \"" << s << "\"" << endl;
}

void Agency::printPacksByDate(const Date & start, const Date & end) const
{
	if (this->packList.size() == 0)
	{
		cout << "No registered packs" << endl;
		return;
	}

	int count = 0;
	for (size_t i = 0; i < this->packList.size(); i++)
	{
		for (size_t j = 0; j < this->packList.at(i).getDestinationsSize(); j++)
		{
			if (start <= this->packList.at(i).getDeparture()
				&& end >= this->packList.at(i).getReturn())
			{
				if (count == 0)
				{
					cout << this->packList.at(i) << endl;
					count++;
				}
				else
				{
					cout << PACK_OUTPUT_SEPARATOR << endl;
					cout << this->packList.at(i) << endl;
				}
			}
		}
	}
	if (count == 0)
		cout << "No packs during the interval " << start << " to " << end << endl;
}

void Agency::printPacksByDestinationAndDate(const string& s, const Date& start, const Date& end) const
{
	string sl = s, aux;
	cu::strTrim(sl);
	cu::strLower(sl);
	if (this->packList.size() == 0)
	{
		cout << "No registered packs" << endl;
		return;
	}

	int count = 0;
	for (size_t i = 0; i < this->packList.size(); i++)
	{
		for (size_t j = 0; j < this->packList.at(i).getDestinationsSize(); j++)
		{
			aux = this->packList.at(i).getDestinationAt(j);
			cu::strLower(aux);
			cout << aux << endl;
			if (sl.find(aux) != string::npos
				&& start <= this->packList.at(i).getDeparture()
				&& end >= this->packList.at(i).getReturn())
			{
				if (count == 0)
				{
					cout << this->packList.at(i) << endl;
					count++;
				}
				else
				{
					cout << PACK_OUTPUT_SEPARATOR << endl;
					cout << this->packList.at(i) << endl;
				}
			}

		}
	}
	if (count == 0)
		cout << "No packs matching the location \"" << s << "\"" << endl;
}


// FILE OUTPUT METHODS

void Agency::saveData() const
{
	printAllClientsToFile();
	printAllPacksToFile();
	return;
}

// OTHER PUBLIC METHODS

bool Agency::removeClient()
{

	int vat;
	int option = 0;

	if (this->clientList.size() == 0) {
		cout << "No clients to be removed" << endl;
		return false;
	}

	cout << "1. Remove by VAT number" << endl
		<< "2. Select from client list" << endl;

	while (true) {
		cu::readInt(option, "Option");
		if (option >= 1 && option <= 2) break;
		else cout << "Not a valid option!" << endl;
	}

	if (option == 1) {
		while (true) {
			cu::readInt(vat, "VAT number");
			for (size_t i = 0; i < this->clientList.size(); i++) {
				if (this->clientList.at(i).getVAT() == vat) {
					this->clientList.erase(this->clientList.begin() + i);
					cout << "Client removed" << endl;
					return true;
				}
			}
			cout << "Client not found" << endl;
		}
	}
	else {
		for (size_t j = 0; j < this->clientList.size(); j++) {
			cout << j + 1 << "." << this->clientList.at(j).getName() << " - VAT :"
				<< this->clientList.at(j).getVAT() << endl;
		}

		while (true) {
			cu::readInt(option, "Option");
			if (1 <= option && option <= (int) (this->clientList.size() + 1)) break;
			else cout << "Not a valid option!" << endl;
		}
		this->clientList.erase(this->clientList.begin() + option - 1);
		cout << "Client removed" << endl;
		return true;

	}
	return true;
}

bool Agency::changeClient(unsigned vat) {
	int i, num;
	string str;
	Address auxAddress;

	if (this->clientList.size() == 0) {
		cout << "No clients to be changed" << endl;
		return false;
	}
	else if (!isVatUsed(vat)) {
		cout << "Client with VAT " << vat << " not found" << endl;
		return false;
	}

	for (size_t j = 0; j < this->clientList.size(); j++) {
		if (this->clientList.at(j).getVAT() == vat) {
			i = j; break;
		}
	}

	cout << "1. Name" << endl << "2. VAT" << endl << "3. Household" << endl
		<< "4. Address" << endl << "0. Cancel" << endl;

	while (true) {
		cu::readInt(num, "What do you want to change");
		if (num >= 0 && num <= 4) break;
		else cout << "Not a valid option!" << endl;
	}

	switch (num)
	{
	case 1:
		cu::readStr(str, "Name");
		clientList.at(i).setName(str); break;
	case 2:
		while (true) {
			cu::readInt(num, "VAT");
			if (!isVatUsed(num)) break;
		}
		clientList.at(i).setVAT(num, this->clientList); break;
	case 3:
		while (true) {
			cu::readInt(num, "Household");
			if (num >= 1) break;
			cout << "Insert a valid household number" << endl;
		}
		clientList.at(i).setHousehold(num); break;
	case 4:
		auxAddress.readUserInput();
		clientList.at(i).setAddress(auxAddress); break;
	default:
		return false;
	}

	cout << "Client changed sucessful" << endl;

	return true;
}

bool Agency::isVatUsed(unsigned vat) const
{
	bool foundVat = false;

	for (size_t i = 0; i < this->clientList.size(); i++)
	{
		if (vat == this->clientList.at(i).getVAT())
		{
			foundVat = true;
			break;
		}
	}

	return foundVat;
}

// PRIVATE METHODS

bool Agency::readClientFromFile(std::ifstream & file, Client & client, unsigned & lineTracker) {
	string str;
	Address auxAddress;
	vector<int> travelPacks = {};
	unsigned int auxInt;


	getline(file, str);
	client.setName(str);
	if (file.eof() || file.fail()) return false;
	lineTracker++;

	file >> auxInt;
	client.setVAT(auxInt, clientList);
	if (file.eof() || file.fail()) return false;
	lineTracker++;

	file >> auxInt;
	client.setHousehold(auxInt);
	if (file.eof() || file.fail()) return false;
	lineTracker++;
	file.ignore(1000, '\n');

	if (!auxAddress.readFromFile(file, lineTracker))
		return false;
	else {
		client.setAddress(auxAddress);
	}

	if (file.peek() != '-')
	{
		int num;
		while (true)
		{
			file >> num;
			if (file.eof()) return false;

			travelPacks.push_back(num);

			if (file.peek() == ' ')
			{
				file.get();
				if (file.eof()) return false;

				if (file.peek() == ';')
				{
					file.get();
					if (file.eof()) return false;
				}
				else
					break;
			}
			else if (file.peek() == ';')
			{
				file.get();
				if (file.eof()) return false;
			}
			else
				break;
		}

		client.setTravelPacksList(travelPacks);
		lineTracker++;
	}
	else // If it peeks a '-'
	{
		file.ignore(1000, '\n'); // Advance to the next line
		client.setTravelPacksList(travelPacks); // Sets to {} ( travelPacks is initialized to {} )
	}

	file >> auxInt;
	client.setTotalSpent(auxInt);
	lineTracker++;

	file.ignore(1000, '\n');

	return true;
}

bool Agency::readPackFromFile(std::ifstream& fin, TravelPack & pack, unsigned & lineTracker)
{
	int n;
	string s;
	Date date;

	// TODO: Verify if the ID hasn't been used yet
	fin >> n;
	if (fin.eof() || fin.fail() || !pack.setId(n))
		return false;
	lineTracker++;

	fin.ignore(1000, '\n');


	getline(fin, s);
	if (fin.eof()) return false;

	// Separate all read locations
	int start = 0, end = s.find('-');
	string sub;
	vector<string> destinations = {};

	if (end == string::npos)
	{
		// There are no secondary locations
		cu::strTrim(s);
		destinations.push_back(s);
	}
	else
	{
		sub = s.substr(0, end);
		cu::strTrim(sub);
		destinations.push_back(sub);

		do
		{
			// There are secondary locations
			start = end + 1;
			end = s.find(',', start);
			sub = s.substr(start, end - start);
			cu::strTrim(sub);
			destinations.push_back(sub);

		} while (end != string::npos); // Stops reading once there's nothing more
	}

	if (!pack.setDestinations(destinations))
		return false;
	lineTracker++;

	if (!date.readFromFile(fin, lineTracker) || !pack.setDeparture(date))
		return false;
	lineTracker++;

	if (!date.readFromFile(fin, lineTracker) || !pack.setReturn(date))
		return false;
	lineTracker++;

	fin >> n;
	if (fin.eof() || fin.fail() || !pack.setPrice(n))
		return false;
	lineTracker++;

	fin >> n;
	if (fin.eof() || fin.fail() || !pack.setMaxBookings(n))
		return false;
	lineTracker++;

	fin >> n;
	if (fin.eof() || fin.fail() || !pack.setCurrentBookings(n))
		return false;
	lineTracker++;

	fin.ignore(1000, '\n');

	pack.updateAvaiability();

	return true;
}

void Agency::printAllClientsToFile() const
{
	ofstream fout;
	fout.open(this->fileNameClients);

	if (this->clientList.empty())
	{
		fout.close();
		return;
	}

	fout << this->clientList.front() << endl;

	for (size_t i = 1; i < this->clientList.size(); i++)
	{
		fout << "::::::::::" << endl;
		fout << clientList.at(i) << endl;
	}

	fout.close();

	return;
}

void Agency::printAllPacksToFile() const
{
	ofstream fout;
	fout.open(this->fileNamePacks);

	fout << this->maxPackId << endl;

	if (this->packList.empty())
	{
		fout.close();
		return;
	}

	fout << this->packList.front() << endl;

	for (size_t i = 1; i < this->packList.size(); i++)
	{
		fout << "::::::::::" << endl;
		fout << packList.at(i) << endl;
	}

	fout.close();

	return;
}
