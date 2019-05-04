#ifndef AGENCY_H
#define AGENCY_H

#include "travelPack.h"
#include "address.h"
#include "client.h"
#include <string>
#include <vector>
#include <fstream>

class Client;

class Agency
{
public:
	Agency();
	Agency(std::string & agencyFileName);

	// GET METHODS
	std::string getName() const;
	unsigned getVAT() const;
	Address getAddress() const;
	std::string getURL() const;
	std::vector<Client> getClients() const;
	std::vector<TravelPack> getPacksList() const;

	// SET METHODS
	bool setName(std::string new_name);
	bool setVATnumber(unsigned new_VAT);
	bool setAddress(Address new_address);
	bool setURL(std::string new_url);
	bool setClients(std::vector<Client> & new_clients);
	bool setPackets(std::vector<TravelPack> & new_packList);


	// Read Methods

	bool readNewClient();  // reads client from user input

	bool readClientsFromFile(std::ifstream & file,  unsigned int & lineTracker); // reads clients from file


	// Other Methods

	bool removeClient(); 

private:
	std::string name, url, fileNameClients, fileNamePacks;
	Address address;
	unsigned vat;
	std::vector<TravelPack> packList;
	std::vector<Client> clientList;

	// read one client from file, return false if it fails 
	bool readClientFromFile(std::ifstream & file, Client & client, unsigned int & lineTracker);  
};

#endif // AGENCY_H