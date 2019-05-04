#ifndef AGENCY_H
#define AGENCY_H

#include "travelPack.h"
#include "address.h"
#include "client.h"
#include "travelPack.h"
#include <string>
#include <vector>
#include <fstream>

class Client;

class Agency
{
public:
	Agency();
	Agency(std::string & agencyFileName);

	// Read Methods

	bool readNewClient();  // reads client from user input

	bool readClientsFromFile(std::ifstream & file,  unsigned int & lineTracker); // reads clients from file

private:
	std::string name, url, fileNameClients, fileNamePacks;
	Address address;
	int nif;
	std::vector<TravelPack> packList;
	std::vector<Client> clientList;

	// read one client from file, return false if it fails 
	bool readClientFromFile(std::ifstream & file, Client & client, unsigned int & lineTracker);  
};

#endif // AGENCY_H