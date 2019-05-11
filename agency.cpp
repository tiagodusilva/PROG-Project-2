#include "agency.h"
#include "customUtilities.h"
#include <iomanip>

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

vector<Client> Agency::getClientList() const {
	return this->clientList;
}

size_t Agency::getClientListSize() const
{
	return this->clientList.size();
}

vector<TravelPack> Agency::getPackList() const {
	return this->packList;
}

size_t Agency::getPackListSize() const
{
	return this->packList.size();
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

void Agency::printClientByVAT(unsigned vat) const
{
	cout << endl;

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
	cout << endl;

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

void Agency::printClientList() const
{
	for (size_t j = 0; j < this->clientList.size(); j++) {
		cout << j + 1 << ". " << left << setw(30) << this->clientList.at(j).getName() << right << "    VAT: "
			<< this->clientList.at(j).getVAT() << endl;
	}
}

void Agency::printPacksByClient(const int vat) const
{
	cout << endl;

	if (this->clientList.empty()) {
		cout << "No registered clients" << endl;
		return;
	}
	if (this->packList.empty()) {
		cout << "No registered packs" << endl;
		return;
	}

	// Look for the client
	for (size_t i = 0; i < this->clientList.size(); i++)
	{
		if (this->clientList.at(i).getVAT() == vat)
		{
			// Loop through all of its TravelPacks
			for (size_t j = 0; j < this->clientList.at(i).getTravelPacksListSize(); j++)
			{
				int pack = this->clientList.at(i).getTravelPackAt(j);
				// Print pack with this ID
				this->printPackById(pack);
			}
			return;
		}
	}

	cout << "Client with VAT \"" << vat << "\" not found" << endl;	
}

void Agency::printPackById(const int id) const
{
	cout << endl;

	int aux = abs(id);
	for (size_t i = 0; i < this->packList.size(); i++)
	{
		if (abs(this->packList.at(i).getId()) == aux)
		{
			cout << this->packList.at(i) << endl;
			return;
		}
	}

	cout << "Pack with id \"" << id << "\" not found" << endl;
}

void Agency::printPacks(const bool onlyAvaiable) const
{

	cout << endl;

	if (this->packList.size() == 0)
	{
		cout << "No registered packs" << endl;
		return;
	}

	int count = 0;
	for (size_t i = 0; i < this->packList.size(); i++)
	{
		// if onlyAvaible is activated, will only print if its avaiable, if not, prints anyway
		if (!onlyAvaiable || this->packList.at(i).isAvaiable())
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
	if (count == 0)
		// In this case, if there are no packs, it will enter the condition at the start
		cout << "No avaiable packs found" << endl;

}

void Agency::printPacksByDestination(const string & s, const bool onlyAvaiable) const
{
	string sl = s, aux;
	cu::strTrim(sl);
	cu::strLower(sl);

	cout << endl;

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
				// if onlyAvaible is activated, will only print if its avaiable, if not, prints anyway
				if (!onlyAvaiable || this->packList.at(i).isAvaiable())
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

				// If it finds one match, it doesn't need to check the remaining locations
				break; 
			}
		}
	}
	if (count == 0)
	{
		if (onlyAvaiable)
			cout << "No avaiable packs matching the location \"" << s << "\"" << endl;
		else
			cout << "No packs matching the location \"" << s << "\"" << endl;
	}
		
}

void Agency::printPacksByDate(const Date & start, const Date & end, const bool onlyAvaiable) const
{
	cout << endl;

	if (this->packList.size() == 0)
	{
		cout << "No registered packs" << endl;
		return;
	}

	int count = 0;
	for (size_t i = 0; i < this->packList.size(); i++)
	{
		// if onlyAvaible is activated, will only check if its avaiable, if not, checks anyway
		if (!onlyAvaiable || this->packList.at(i).isAvaiable())
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
	{
		if (onlyAvaiable)
			cout << "No avaiable packs during the interval " << start << " to " << end << endl;
		else
			cout << "No packs during the interval " << start << " to " << end << endl;
	}
		
}

void Agency::printPacksByDestinationAndDate(const string& s, const Date& start, const Date& end, const bool onlyAvaiable) const
{
	string sl = s, aux;
	cu::strTrim(sl);
	cu::strLower(sl);

	cout << endl;

	if (this->packList.size() == 0)
	{
		cout << "No registered packs" << endl;
		return;
	}

	int count = 0;
	for (size_t i = 0; i < this->packList.size(); i++)
	{
		// If onlyAvaible is activated, will only print if its avaiable, if not, prints anyway
		// Also immediatly checks if the it's in the given Dates range
		if ((!onlyAvaiable || this->packList.at(i).isAvaiable())
			&& (start <= this->packList.at(i).getDeparture()
			&& end >= this->packList.at(i).getReturn()))
		{
			for (size_t j = 0; j < this->packList.at(i).getDestinationsSize(); j++)
			{
				aux = this->packList.at(i).getDestinationAt(j);
				cu::strLower(aux);
			
				if (sl.find(aux) != string::npos)
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

					// If it finds a match, it does not need to check the others
					break;
				}
			} // End inner for loop
		}
	} // End outer for loop

	if (count == 0)
	{
		if (onlyAvaiable)
			cout << "No avaiable packs matching the location \"" << s << "\"" << endl;
		else
			cout << "No packs matching the location \"" << s << "\"" << endl;
	}
}


// FILE OUTPUT METHODS

void Agency::saveData() const
{
	printAllClientsToFile();
	printAllPacksToFile();
	return;
}

// OTHER PUBLIC METHODS

void Agency::printStatistics() const {

	int soldPacks = 0;
	int totalSold = 0;

	for (size_t i = 0; i < this->clientList.size(); i++) {
		totalSold += this->clientList.at(i).getTotalSpent();
		soldPacks += this->clientList.at(i).getTravelPacksListSize();
	}

	cout << "Number of packs sold: " << soldPacks << endl;
	cout << "Total value of packs sold: " << totalSold << endl;
}

void Agency::printMostVisitedDestinations(int n) const {
	int auxInt = 1;
	map<string, int> packMap;
	multimap<int, string> reversedPackMap;

	// create a map of a place and the number of times it was visited by all clients
	for (size_t i = 0; i < this->clientList.size(); i++) { 

		for (size_t j = 0; j < this->clientList.at(i).getTravelPacksList().size(); j++) { 

			for (size_t k = 0; k < packWithId(this->clientList.at(i).getTravelPacksList().at(j)).getDestinationsSize(); k++) {
				
				if (packWithId(this->clientList.at(i).getTravelPacksList().at(j)).getDestinationsSize() > 1 && k==0)
					continue;

				if (packMap.find(packWithId(this->clientList.at(i).getTravelPacksList().at(j)).getDestinationAt(k)) == packMap.end())
					packMap[packWithId(this->clientList.at(i).getTravelPacksList().at(j)).getDestinationAt(k)] = 1;

				else
					packMap[packWithId(this->clientList.at(i).getTravelPacksList().at(j)).getDestinationAt(k)] += 1;
			}
		}
	}

	// change the key and value of packMap in order to order by the number of visits
	for (auto& iter: packMap)
		reversedPackMap.insert(pair <int, string>(iter.second, iter.first));

	for (auto iter = reversedPackMap.rbegin(); iter != reversedPackMap.rend(); ++iter) {
		cout << '\t' << auxInt << ". " << iter->second;
		cout << " (Total Visits: " <<  iter->first << ")\n\n";
		auxInt++;

		if (n + 1 == auxInt) break;
	}
}

bool Agency::removeClient()
{
	int vat;
	int option = 0;

	if (this->clientList.size() == 0) {
		cout << "No clients to be removed" << endl;
		return false;
	}

	cout << "--------  Remove Client  --------" << endl << endl;
	cout << "1. Remove by VAT number" << endl
		<< "2. Select from client list" << endl
		<< "0. Previous Menu" << endl << endl;

	while (true) {
		if (!cu::readInt(option, "Option"))
		{
			cout << "Operation Aborted" << endl;
			cu::pauseConsole();
			return false;
		}
		if (option >= 0 && option <= 2) break;
		else cout << "Not a valid option!" << endl;
	}

	switch (option)
	{
	case 0:
		return false;
		break; // Safety break ;D
	case 1:
		while (true) {
			if (!cu::readInt(vat, "VAT number"))
			{
				cout << "Operation Aborted" << endl;
				return false;
			}
			for (size_t i = 0; i < this->clientList.size(); i++) {
				if (this->clientList.at(i).getVAT() == vat) {
					this->clientList.erase(this->clientList.begin() + i);
					cout << "Client removed" << endl;
					return true;
				}
			}
			cout << "Client not found" << endl;
		}
		break;
	case 2:
		this->printClientList();

		while (true) {
			if (!cu::readInt(option, "Option"))
			{
				cout << "Operation aborted" << endl;
				return false;
			}
			if (1 <= option && option <= (int)(this->clientList.size() + 1)) break;
			else cout << "Not a valid option!" << endl;
		}
		this->clientList.erase(this->clientList.begin() + option - 1);
		cout << "Client removed" << endl;
		return true;
		break;
	default:
		break;
	}
	
	return false; // If it somehow gets here, something went wrong
}

bool Agency::changeClient(const unsigned vat) {
	int i, num;
	string str;
	Address auxAddress;

	if (this->clientList.empty()) {
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

	while (true)
	{
		system("cls");
		cout << "  Possible fields to change" << endl;
		cout << "\t1. Name" << endl << "\t2. VAT" << endl << "\t3. Household" << endl
			<< "\t4. Address" << endl << "\t0. Cancel" << endl;

		if (!cu::readInt(num, "What do you want to change"))
			return false;
		if (num >= 0 && num <= 4)
			break;
		else
		{
			cout << "Not a valid option!" << endl;
			cu::pauseConsole();
		}
	}

	system("cls");

	switch (num)
	{
	case 1:
		cu::readStr(str, "New Name");
		clientList.at(i).setName(str); break;
	case 2:
		while (true) {
			cu::readInt(num, "New VAT");
			if (!isVatUsed(num)) break;
		}
		clientList.at(i).setVAT(num, this->clientList); break;
	case 3:
		while (true) {
			cu::readInt(num, "New Household");
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
	cu::pauseConsole();

	return true;
}

bool Agency::changePack(const int id)
{
	int absId = abs(id);
	if (this->packList.empty())
	{
		cout << "There are no packs to change" << endl;
		return false;
	}

	TravelPack* pack = nullptr;
	for (size_t i = 0; i < this->packList.size(); i++)
	{
		if (abs(this->packList.at(i).getId()) == absId)
		{
			pack = &this->packList.at(i);
			break;
		}
	}

	if (pack == nullptr)
	{
		cout << "Pack with id \"" << id << "\" not found" << endl;
		return false;
	}

	// At this point, the pack has been found

	int aux;
	while (true)
	{
		system("cls");
		cout << "  Possible fields to change" << endl;
		cout << "\t1. Destinations" << endl << "\t2. Departure Date" << endl << "\t3. Return Date" << endl
			<< "\t4. Price" << endl << "\t5. Maximum Bookings" << endl << "\t0. Cancel" << endl;

		if (!cu::readInt(aux, "What do you want to change"))
			return false;
		if (aux >= 0 && aux <= 5)
			break;
		else
		{
			cout << "Not a valid option!" << endl;
			cu::pauseConsole();
		}
	}

	system("cls");

	Date aDate;
	string s;
	vector<string> destinations;
	char c = 'o';

	switch (aux)
	{
	case '0':
		return false;
		break; // Safety break ;D
	case '1': // Destinations
		cout << "New Destinations" << endl;
		if (!cu::readStr(s, "Main destination"))
			return false;
		destinations.push_back(s);

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
		if (!pack->setDestinations(destinations))
			return false; // Extra safety, should never return false
		break;
	case '2': // Departure Date
		while (true)
		{
			cout << "New departure Date:" << endl;
			if (!aDate.readUserInput())
				return false;
			if (pack->setDeparture(aDate))
				break;
		}
		break;
	case '3': // Return Date
		while (true)
		{
			cout << "New return Date:" << endl;
			if (!aDate.readUserInput())
				return false;
			if (pack->setReturn(aDate))
				break;
		}
		break;
	case '4': // Price
		while (true)
		{
			if (!cu::readInt(aux, "New Maximum Bookings"))
				return false;
			if (pack->setMaxBookings(aux))
				break;
		}
		// Now to update ALL the client's totalSpent...
		for (size_t i = 0; i < this->clientList.size(); i++)
		{
			Client* clientPtr = &this->clientList.at(i);
			for (size_t j = 0; j < clientPtr->getTravelPacksListSize(); j++)
			{
				if (abs(clientPtr->getTravelPackAt(j)) == absId)
				{
					clientPtr->setTotalSpent(clientPtr->getTotalSpent() - pack->getPrice());
				}
			}
		}
		break;
	case '5': // Max Bookings
		while (true)
		{
			if (!cu::readInt(aux, "New Maximum Bookings"))
				return false;
			if (pack->setMaxBookings(aux))
				break;
		}
		break;
	default:
		break;
	}

	cout << "Pack changed successfully" << endl;
	cu::pauseConsole();

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

TravelPack Agency::packWithId(int id) const {
	for (size_t i = 0; i < this->packList.size(); i++) {
		if (abs(this->packList.at(i).getId()) == id)
			return this->packList.at(i);
	}
	return {};
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
