#include "agency.h"
#include "address.h"
#include "customUtilities.h"
#include "client.h"
#include <fstream>

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

Agency::Agency(std::string& agencyFileName)
{
	// TEMP
	name = "Undefined Name";
	url = "404";
	fileNameClients = "";
	fileNamePacks = "";
	address = Address();
	vat = 0;
	packList = {};
	clientList = {};
} // Replace once read agency from file is complete

void stringToLower(std::string& s)
{
	for (size_t i = 0; i < s.size(); i++)
	{
		if (s.at(i) >= 'A' && s.at(i) <= 'Z')
			s.at(i) = s.at(i) + ('a' - 'A');
	}
} // Used for checking locations

bool Agency::readNewClient() {
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

	
    cout << "Address:" << endl;

	if (!auxAddress.readUserInput())
		return false;

	newClient.setAddress(auxAddress);

	clientList.push_back(newClient);

	return true;
}

// READ METHODS

bool Agency::readClientFromFile(std::ifstream & file, Client & client, unsigned int & lineTracker) {
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

	file >> auxInt;
	client.setTotalSpent(auxInt);
	lineTracker++;

	file.ignore(1000, '\n');
	
	return true;
}

bool Agency::readClientsFromFile(std::ifstream & file, unsigned int & lineTracker)
{
	while (true)
	{
		Client client;

		if (!readClientFromFile(file, client, lineTracker))
			return false;

		clientList.push_back(client);

		if (file.peek() != ':')
			break;

		lineTracker++;
		file.ignore(1000, '\n');
	}
	return true;
}

/*
	remove client by inserting a vat or choosing from a list of clients
*/

bool Agency::removeClient() {

	int vat; 
	int option=0;

	if (clientList.size() == 0) {
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
			for (size_t i = 0; i < clientList.size(); i++) {
				if (clientList.at(i).getVAT() == vat) {
					clientList.erase(clientList.begin() + i);
					cout << "Client removed" << endl;
					return true;
				}
			}
			cout << "Client not found" << endl;
		}
	}
	else {
		for (size_t j = 0; j < clientList.size(); j++) {
			cout << j + 1 << "." << clientList.at(j).getName() << " - VAT :"
				<< clientList.at(j).getVAT() << endl;
		}

		while (true) {
			cu::readInt(option, "Option");
			if (1 <= option && option <= clientList.size() + 1) break;
			else cout << "Not a valid option!" << endl;
		}
		clientList.erase(clientList.begin() + option - 1);
		cout << "Client removed" << endl;
		return true;

	}
	return true;
}

// GET METHODS

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


// SET METHODS


bool Agency::setName(std::string new_name) {
	if (new_name.size() == 0) return false;
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

