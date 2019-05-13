#include "agency.h"
#include "customUtilities.h"
#include <iomanip>
#include <map>
#include <set>

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

TravelPack Agency::getPackWithId(const int id) const {
	for (size_t i = 0; i < this->packList.size(); i++) {
		if (abs(this->packList.at(i).getId()) == id)
			return this->packList.at(i);
	}
	return {};
}

bool Agency::getAvailabilityOfPack(const int id) const
{
	int auxId = abs(id);

	for (size_t i = 0; i < this->packList.size(); i++)
	{
		if (abs(this->packList.at(i).getId()) == auxId)
			return this->packList.at(i).isAvailable();
	}

	return false;
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
	unsigned lineTracker = 1;
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
	lineTracker = 1;
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
	lineTracker = 1;
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

	getline(file, this->name);
	if (file.eof()) return false;
	lineTracker++;

	file >> this->vat;
	if (file.eof()) return false;
	file.ignore(1000, '\n');
	lineTracker++;

	getline(file, this->url);
	if (file.eof()) return false;
	lineTracker++;

	if (!this->address.readFromFile(file, lineTracker)) return false;

	getline(file, this->fileNameClients);
	if (file.eof()) return false;
	lineTracker++;

	getline(file, this->fileNamePacks);
	lineTracker++;

	return true;
}

bool Agency::readNewClientUserInput() {
	int n;
	string str;
	Client newClient;
	Address auxAddress;

	do {
		if (!cu::readStr(str, "Name")) return false;
	} while (!newClient.setName(str));

	do {
		if (!cu::readInt(n, "VAT")) return false;
	} while (!newClient.setVAT(n, clientList));

	do {
		if (!cu::readInt(n, "Household")) return false;
	} while (!newClient.setHousehold(n));

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
		lineTracker++;
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
		lineTracker++;
	}
	return true;
}


// CONSOLE OUTPUT METHODS

bool Agency::printClientByVAT(unsigned vat) const
{
	cout << endl;

	if (this->clientList.empty()) {
		cout << "No registered clients" << endl;
		return false;
	}

	for (size_t i = 0; i < this->clientList.size(); i++) {
		if (this->clientList.at(i).getVAT() == vat) {
			cout << this->clientList.at(i) << endl;
			return true;
		}
	}

	cout << "Client with VAT \"" << vat << "\" not found" << endl;
	return false;
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
	for (size_t i = 0; i < this->clientList.size(); i++)
	{
		cout << i << " .";
		this->clientList.at(i).printSummary();
		cout << endl;
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

	set<int> printedPacks = {};
	int count = 0;

	// Look for the client
	for (size_t i = 0; i < this->clientList.size(); i++)
	{
		if (this->clientList.at(i).getVAT() == vat)
		{
			if (this->clientList.at(i).getTravelPacksListSize() == 0)
			{
				cout << "No packs registered to this Client" << endl;
				return;
			}

			// Loop through all of its TravelPacks
			for (size_t j = 0; j < this->clientList.at(i).getTravelPacksListSize(); j++)
			{
				int pack = this->clientList.at(i).getTravelPackAt(j);
				if (printedPacks.find(pack) == printedPacks.end())
				{
					// Set doesn't have this pack
					printedPacks.insert(pack);
					// Print pack with this ID
					if (count == 0)
					{
						this->printPackById(pack);
					}
					else
					{
						cout << PACK_OUTPUT_SEPARATOR << endl;
						this->printPackById(pack);
					}
				}
			}
			return;
		}
	}

	cout << "Client with VAT \"" << vat << "\" not found" << endl;
}

bool Agency::printPackById(const int id) const
{
	cout << endl;

	if (this->packList.empty())
	{
		cout << "No registered Packs";
		return false;
	}

	int aux = abs(id);
	for (size_t i = 0; i < this->packList.size(); i++)
	{
		if (abs(this->packList.at(i).getId()) == aux)
		{
			cout << this->packList.at(i) << endl;
			return true;
		}
	}

	cout << "Pack with id \"" << id << "\" not found" << endl;
	return false;
}

void Agency::printPacksSoldToEveryone() const
{
	set<int> packSet;

	for (size_t i = 0; i < this->packList.size(); i++)
	{
		packSet.insert(abs(this->packList.at(i).getId()));
	}

	for (size_t i = 0; i < this->clientList.size(); i++)
	{
		if (packSet.empty())
			break;

		for (auto it = packSet.begin(); it != packSet.end(); )
		{
			if (!this->clientList.at(i).hasBought(*it))
			{
				it = packSet.erase(it);
				// Returns the iterator to the next element (past the removed one)
				// This is the way for c++11, in other standards it will break
			}
			else
				++it; // If not erased, we need to move the iterator to the next element
		}

	}

	if (packSet.empty())
	{
		cout << "There are no packs that have been sold to everyone" << endl;
		return;
	}

	auto it = packSet.begin();
	// First element (valid because we know it's not empty)
	this->printPackById(*it);
	++it;

	// Iterating from the second element (if they exist)
	for (; it != packSet.end(); ++it)
	{
		cout << PACK_OUTPUT_SEPARATOR << endl;
		this->printPackById(*it);
	}
}

void Agency::printPacksSold() const
{
	set <int> packSet;

	for (size_t i = 0; i < this->clientList.size(); i++)
	{
		for (size_t j = 0; j < this->clientList.at(i).getTravelPacksListSize(); j++)
		{
			packSet.insert(this->clientList.at(i).getTravelPackAt(j));
		}
	}

	if (packSet.empty())
	{
		cout << "No packs have been sold to anyone" << endl;
	}

	auto it = packSet.begin();
	// First element (valid because we know it's not empty)
	this->printPackById(*it);
	++it;

	// Iterating from the second element (if they exist)
	for (; it != packSet.end(); ++it)
	{
		cout << PACK_OUTPUT_SEPARATOR << endl;
		this->printPackById(*it);
	}

}

void Agency::printPacks(const bool onlyAvailable) const
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
		// if onlyAvaible is activated, will only print if its available, if not, prints anyway
		if (!onlyAvailable || this->packList.at(i).isAvailable())
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
		cout << "No available packs found" << endl;

}

void Agency::printPacksByDestination(const string & s, const bool onlyAvailable) const
{
	string sl = s;
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
		if (this->packList.at(i).containsDestination(sl))
		{
			// if onlyAvaible is activated, will only print if its available, if not, prints anyway
			if (!onlyAvailable || this->packList.at(i).isAvailable())
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
		if (onlyAvailable)
			cout << "No available packs matching the location \"" << s << "\"" << endl;
		else
			cout << "No packs matching the location \"" << s << "\"" << endl;
	}

}

void Agency::printPacksByDate(const Date & start, const Date & end, const bool onlyAvailable) const
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
		// if onlyAvaible is activated, will only check if its available, if not, checks anyway
		if (!onlyAvailable || this->packList.at(i).isAvailable())
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
		if (onlyAvailable)
			cout << "No available packs during the interval " << start << " to " << end << endl;
		else
			cout << "No packs during the interval " << start << " to " << end << endl;
	}

}

void Agency::printPacksByDestinationAndDate(const string& s, const Date& start, const Date& end, const bool onlyAvailable) const
{
	string sl = s;
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
		// If onlyAvailable is activated, will only print if its available, if not, prints anyway
		// Also immediatly checks if the it's in the given Dates range
		if ((!onlyAvailable || this->packList.at(i).isAvailable())
			&& (start <= this->packList.at(i).getDeparture()
				&& end >= this->packList.at(i).getReturn())
			&& this->packList.at(i).containsDestination(sl))
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
	} // End outer for loop

	if (count == 0)
	{
		if (onlyAvailable)
			cout << "No available packs matching the location \"" << s << "\"" << endl;
		else
			cout << "No packs matching the location \"" << s << "\"" << endl;
	}
}

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

void Agency::printMostVisitedDestinations(const int n) const {
	int auxInt = 1;
	multimap<int, string> reversedPackMap;

	this->generatePopularDestinations(reversedPackMap);

	for (auto iter = reversedPackMap.rbegin(); iter != reversedPackMap.rend(); ++iter) {
		if (iter->first == 0) {
			if (auxInt == 1)
				cout << "No places visited by any client..." << endl;
			break;
		}
		cout << '\t' << auxInt << ". " << iter->second;
		cout << " (Total Visits: " << iter->first << ")\n\n";
		auxInt++;

		if (n + 1 == auxInt) break;
	}
}

void Agency::printClientRecommendations(const int n) const {

	int auxInt;
	bool foundRecommendation = true;
	bool auxBool = false;
	string recommendation;
	multimap<int, string> reversedPackMap;

	this->generatePopularDestinations(reversedPackMap);

	if (n <= 0) {
		cout << "No possible recommendations" << endl;	return;
	}

	for (size_t i = 0; i < this->clientList.size(); i++) {
		
		auxInt = 1;

		for (auto iter = reversedPackMap.rbegin(); iter != reversedPackMap.rend(); ++iter) {

			recommendation = iter->second;
			cu::strLower(recommendation);
			foundRecommendation = true;
			auxBool = false;

			// checks if the recommendation is possible
			for (size_t w = 0; w < this->packList.size(); w++) {
				if (this->packList.at(w).containsDestination(recommendation) && this->packList.at(w).isAvailable()) {
					auxBool = true;
					break;
				}
			}
			if (!auxBool) continue;

			for (size_t j = 0; j < this->clientList.at(i).getTravelPacksListSize(); j++)
			{
				// checks if the client already went to the recommendation
				if (getPackWithId(this->clientList.at(i).getTravelPackAt(j)).containsDestination(recommendation)) {
					foundRecommendation = false;
					break;
				}
			}

			if (foundRecommendation) {

				// searches for the available pack with the recommendation and prints it
				for (size_t k = 0; k < this->packList.size(); k++) {

					if (this->packList.at(k).containsDestination(recommendation) && this->packList.at(k).isAvailable()) {
						cout << this->clientList.at(i).getName() << " recommended pack:" << endl;
						cout << this->packList.at(k) << endl << endl;
						break;
					}
				}
			}
			if (foundRecommendation) break;

			auxInt++;
			if (n + 1 == auxInt) break;
		}
		// in case no available recommendation is possible
		if (!foundRecommendation || !auxBool) {
			cout << this->clientList.at(i).getName() << ':' << endl;
			cout << "Recommendation for this client not available" << endl << endl;;
		}

		foundRecommendation = true;
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

void Agency::generatePopularDestinations(std::multimap<int, string> & reversedPackMap) const
{
	map<string, int> packMap;
	reversedPackMap.clear();

	// create a map of a place and the number of times it was visited by all clients
	for (size_t i = 0; i < this->clientList.size(); i++) {

		for (size_t j = 0; j < this->clientList.at(i).getTravelPacksList().size(); j++) {

			for (size_t k = 0; k < this->getPackWithId(this->clientList.at(i).getTravelPacksList().at(j)).getDestinationsSize(); k++) {

				if (packMap.find(this->getPackWithId(this->clientList.at(i).getTravelPacksList().at(j)).getDestinationAt(k)) == packMap.end())
					packMap[this->getPackWithId(this->clientList.at(i).getTravelPacksList().at(j)).getDestinationAt(k)] = 1;

				else
					packMap[this->getPackWithId(this->clientList.at(i).getTravelPacksList().at(j)).getDestinationAt(k)] += 1;
			}
		}
	}

	// adds in packMap the places with 0 visitors
	for (size_t a = 0; a < this->packList.size(); a++) {
		for (size_t b = 0; b < this->packList.at(a).getDestinationsSize(); b++) {

			if (packMap.find(this->packList.at(a).getDestinationAt(b)) == packMap.end())
				packMap[this->packList.at(a).getDestinationAt(b)] = 0;

		}
	}

	// change the key and value of packMap in order to order by the number of visits
	for (auto& iter : packMap)
		reversedPackMap.insert(pair <int, string>(iter.second, iter.first));
}

bool Agency::purchasePack(const int vat, const int id, const int tickets)
{
	Client* client = this->getClientPointer(vat);

	if (client == nullptr)
	{
		cout << "Client with VAT \"" << vat << "\" not found" << endl;
	}

	TravelPack* pack = this->getPackPointer(id);

	if (pack == nullptr) {
		cout << "Pack with ID \"" << id << "\" not found" << endl;
		return false;
	}

	if (!pack->isAvailable()) {
		cout << "Pack with ID \"" << id << "\" is unavailable" << endl;
		return false;
	}

	int availableTickets = pack->getMaxBookings() - pack->getCurrentBookings();
	if (tickets <= 0)
	{
		cout << "Can't purchase less than one ticket" << endl;
		return false;
	}

	if (tickets > availableTickets)
	{
		cout << "Can't overbook a Pack" << endl;
		return false;
	}

	// After this point, all inputs were valid

	// Update pack info

	pack->setCurrentBookings(pack->getCurrentBookings() + tickets);

	if (pack->getCurrentBookings() >= pack->getMaxBookings())
		pack->makeUnavailable();

	// Update client info

	client->addPack(id, pack->getPrice(), tickets);

	cout << "Purchase successful" << endl;

	return true;
}

bool Agency::removeClientByVat(const int vat)
{
	if (this->clientList.empty())
	{
		cout << "No Clients to be removed" << endl;
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
	return false;
}

bool Agency::removeClientByIndex(const int index)
{
	if (this->clientList.empty())
	{
		cout << "No Clients to be removed" << endl;
		return false;
	}

	if (index >= (int)this->clientList.size())
	{
		cout << "Index out of range" << endl;
		return false;
	}

	this->clientList.erase(this->clientList.begin() + index);

	cout << "Client removed successfully" << endl;
	return true;
}

bool Agency::makePackUnavailableById(const int id)
{
	if (this->packList.empty())
	{
		cout << "No Packs to be removed" << endl;
		return false;
	}

	TravelPack* pack = this->getPackPointer(id);

	if (pack == nullptr)
	{
		cout << "Pack not found" << endl;
		return false;
	}

	pack->makeUnavailable();
	cout << "Pack is now unavailable" << endl;
	return true;
}

bool Agency::makePackUnavailableByIndex(const int index)
{
	if (this->packList.empty())
	{
		cout << "No Packs to be removed" << endl;
		return false;
	}

	if (index >= (int)this->packList.size())
	{
		cout << "Index out of range" << endl;
		return false;
	}

	this->packList.at(index).makeUnavailable();

	cout << "Pack is now unavailable" << endl;
	return true;
}

// This is kinda agencyMenu, but private stuff is private
bool Agency::changeClient(const unsigned vat) {
	int num;
	string str;
	Address auxAddress;

	if (this->clientList.empty()) {
		cout << "No clients to be changed" << endl;
		return false;
	}

	Client* client = this->getClientPointer(vat);

	if (client == nullptr)
	{
		cout << "Client with VAT \"" << vat << "\" not found" << endl;
		return false;
	}

	while (true)
	{
		system("cls");
		cout << "  Possible fields to change" << endl;
		cout << "\t1. Name" << endl << "\t2. VAT" << endl << "\t3. Household" << endl
			<< "\t4. Address" << endl << "\t0. Cancel" << endl;

		if (!cu::readInt(num, "Field to change"))
			return false;
		if (num >= 0 && num <= 4)
			break;
		else
		{
			cout << "Invalid option" << endl;
			cu::pauseConsole();
		}
	}

	system("cls");

	switch (num)
	{
	case 1:
		if (!cu::readStr(str, "New Name"))
		{
			cout << "Operation Aborted" << endl;
			return false;
		}
		client->setName(str); break;
	case 2:
		while (true) {
			if (!cu::readInt(num, "New VAT"))
			{
				cout << "Operation aborted" << endl;
				return false;
			}
			if (!isVatUsed(num))
			{
				client->setVAT(num, this->clientList);
				break;
			}
		}
		break;
	case 3:
		while (true) {
			if (!cu::readInt(num, "New Household"))
			{
				cout << "Operation aborted" << endl;
				return false;
			}
			if (num >= 1) break;
			cout << "Insert a valid household number" << endl;
		}
		client->setHousehold(num); break;
	case 4:
		if (!auxAddress.readUserInput())
		{
			cout << "Operation aborted" << endl;
			return false;
		}
		client->setAddress(auxAddress);
		break;
	default:
		return false;
	}

	//cout << "Client changed successfully" << endl;   // agr----

	return true;
}

// This is kinda agencyMenu, but private stuff is private
bool Agency::changePack(const int id)
{
	if (this->packList.empty())
	{
		cout << "There are no packs to change" << endl;
		return false;
	}

	TravelPack* pack = this->getPackPointer(id);

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
	int priceChange;
	char c = 'o';

	switch (aux)
	{
	case 0:
		return false;
		break; // Safety break ;D
	case 1: // Destinations
		cout << "New Destinations" << endl;
		if (!cu::readStr(s, "Main destination"))
			return false;
		destinations.push_back(s);

		if (!cu::readConfirmation(c, "Insert secondary destinations"))
			return false;

		if (c == 'y')
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

	case 2: // Departure Date
		while (true)
		{
			cout << "New departure Date:" << endl;
			if (!aDate.readUserInput())
				return false;
			if (pack->setDeparture(aDate))
				break;
		}
		break;

	case 3: // Return Date
		while (true)
		{
			cout << "New return Date:" << endl;
			if (!aDate.readUserInput())
				return false;
			if (pack->setReturn(aDate))
				break;
		}
		break;

	case 4: // Price

		priceChange = pack->getPrice();
		while (true)
		{
			if (!cu::readInt(aux, "New Price"))
				return false;
			if (pack->setPrice(aux))
				break;
		}

		if (!cu::readConfirmation(c, "Do you wish to reflect this change in all the Clients' Total Spent"))
			return false;

		if (c == 'y')
		{
			// Calculate price difference
			priceChange = priceChange - aux;
			// Now to update ALL the client's totalSpent...
			for (size_t i = 0; i < this->clientList.size(); i++)
			{
				Client* clientPtr = &this->clientList.at(i);
				int absId = abs(id);
				for (size_t j = 0; j < clientPtr->getTravelPacksListSize(); j++)
				{
					if (abs(clientPtr->getTravelPackAt(j)) == absId)
					{
						clientPtr->setTotalSpent(clientPtr->getTotalSpent() - priceChange);
					}
				}
			}
			cout << "All clients' Total Spent is now updated" << endl;
		}
		else
			cout << "No changes were made to the Clients' Total Spent" << endl;


		break;

	case 5: // Max Bookings
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

	return true;
}

bool Agency::isVatUsed(const unsigned vat) const
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

bool Agency::isIdUsed(const int id) const
{
	int auxId = abs(id);
	bool foundId = false;

	for (size_t i = 0; i < this->packList.size(); i++)
	{
		if (auxId == abs(this->packList.at(i).getId()))
		{
			foundId = true;
			break;
		}
	}

	return foundId;
}

bool Agency::packMap(std::map<int, int> & packMap, int & packsFound, const bool onlyAvailable, const bool printMap) const
{
	if (this->packList.empty())
	{
		cout << "No Packs registered" << endl;
		return false;
	}

	packMap.clear();
	packsFound = 0;
	for (size_t i = 0; i < this->packList.size(); i++)
	{
		if (!onlyAvailable || this->packList.at(i).isAvailable())
		{
			packsFound++;
			if (printMap)
			{
				cout << "\t" << packsFound << ". ";
				this->packList.at(i).printSummary();
				cout << endl;
			}

			packMap.insert(pair<int, int>(packsFound, this->packList.at(i).getId()));
		}
	}

	if (packsFound == 0) // No packs available
	{
		cout << "There are no available Packs" << endl;
		return false;
	}

	return true;
}

bool Agency::clientMap(std::map<int, int> & clientMap, int & clientCounter, const bool printMap) const
{
	if (this->clientList.empty())
	{
		cout << "No Clients registered" << endl;
		return false;
	}

	clientMap.clear();
	clientCounter = 0;

	for (size_t i = 0; i < this->clientList.size(); i++)
	{
		clientCounter++;
		if (printMap)
		{
			cout << "\t" << clientCounter << ". ";
			this->clientList.at(i).printSummary();
			cout << endl;
		}

		clientMap.insert(pair<int, int>(clientCounter, this->clientList.at(i).getVAT()));
	}

	return true;
}

// PRIVATE METHODS

bool Agency::readClientFromFile(std::ifstream & file, Client & client, unsigned & lineTracker) {
	string str;
	Address auxAddress;
	vector<int> travelPacks = {};
	unsigned int auxInt;


	getline(file, str);
	if (file.eof() || file.fail() || !client.setName(str)) return false;
	lineTracker++;

	file >> auxInt;
	if (file.eof() || file.fail() || !client.setVAT(auxInt, clientList)) return false;
	lineTracker++;

	file >> auxInt;
	if (file.eof() || file.fail() || !client.setHousehold(auxInt)) return false;
	lineTracker++;
	file.ignore(1000, '\n');

	if (!auxAddress.readFromFile(file, lineTracker))
		return false;
	if (!client.setAddress(auxAddress))
	{
		--lineTracker; // Decrease lineTracker to the correct line before exiting
		return false;
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

		if (!client.setTravelPacksList(travelPacks)) return false;
		lineTracker++;
	}
	else // If it peeks a '-'
	{
		file.ignore(1000, '\n'); // Advance to the next line
		if (!client.setTravelPacksList(travelPacks)) return false; // Sets to {} ( travelPacks is initialized to {} )
	}

	file >> auxInt;
	if (file.eof() || file.fail() || !client.setTotalSpent(auxInt)) return false;
	lineTracker++;

	file.ignore(1000, '\n');

	return true;
}

bool Agency::readPackFromFile(std::ifstream& fin, TravelPack & pack, unsigned & lineTracker)
{
	int n;
	string s;
	Date date;

	fin >> n;
	if (fin.eof() || fin.fail() || !pack.setId(n) || this->isIdUsed(n))
		return false;
	lineTracker++;

	fin.ignore(1000, '\n');


	getline(fin, s);
	if (fin.eof() || fin.fail()) return false;

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

	if (!pack.setDestinations(destinations)) return false;
	lineTracker++;

	if (!date.readFromFile(fin, lineTracker))
		return false;
	if (!pack.setDeparture(date))
	{
		--lineTracker; // Decrease lineTracker to the correct line before exiting
		return false;
	}
	// Auto updates lineTracker

	if (!date.readFromFile(fin, lineTracker))
		return false;
	if (!pack.setReturn(date))
	{
		--lineTracker; // Decrease lineTracker to the correct line before exiting
		return false;
	}
	// Auto updates lineTracker

	fin >> n;
	if (fin.eof() || fin.fail() || !pack.setPrice(n)) return false;
	lineTracker++;

	fin >> n;
	if (fin.eof() || fin.fail() || !pack.setMaxBookings(n))	return false;
	lineTracker++;

	fin >> n;
	if (fin.eof() || fin.fail() || !pack.setCurrentBookings(n))	return false;
	lineTracker++;

	fin.ignore(1000, '\n');

	pack.updateAvailability();

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

Client* Agency::getClientPointer(const int vat)
{
	for (size_t i = 0; i < this->clientList.size(); i++)
	{
		if (this->clientList.at(i).getVAT() == vat)
			return &this->clientList.at(i);
	}

	return nullptr;
}

TravelPack* Agency::getPackPointer(const int id)
{
	int auxId = abs(id);
	for (size_t i = 0; i < this->packList.size(); i++)
	{
		if (abs(this->packList.at(i).getId()) == auxId)
			return &this->packList.at(i);
	}

	return nullptr;
}
