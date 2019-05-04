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
	nif = 0;
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
	nif = 0;
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
