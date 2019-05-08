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
	// CONSTRUCTORS

	Agency();


	// GET METHODS

	std::string getName() const;
	unsigned getVAT() const;
	Address getAddress() const;
	std::string getURL() const;
	std::vector<Client> getClients() const;
	std::vector<TravelPack> getPacksList() const;
	std::string getFileNameClients() const;
	std::string getFileNamePacks() const;

	// SET METHODS

	bool setName(std::string new_name);
	bool setVATnumber(unsigned new_VAT);
	bool setAddress(Address new_address);
	bool setURL(std::string new_url);
	bool setClients(std::vector<Client> & new_clients);
	bool setPackets(std::vector<TravelPack> & new_packList);


	// READ METHODS

	bool readAgencyFromFile(std::ifstream & file, unsigned int & lineTracker);

	/**
		Reads a new Client from user input
		Guarentees a unique VAT
		Ctrl + Z to abort input (returns false)
	*/
	bool readNewClientUserInput();

	/**
		Reads all the Clients from a file
	*/
	bool readClientsFromFile(std::ifstream & file,  unsigned int & lineTracker);

	/**
		Reads a new Pack from user input
		Guarentees a new ID
		Ctrl + Z to abort input (returns false)
	*/
	bool readNewPackUserInput();

	/**
		Reads all the packs from a file
	*/
	bool readAllPacksFromFile();


	// OUTPUT METHODS

	/**
		Prints all packs to the screen in a formatted form
	*/
	void printPacks() const;

	/**
		Prints all packs matching a given destination to the screen in a formatted form
	*/
	void printPacksByDestination(const std::string & s) const;
	
	/**
		Prints all packs matching a given Date interval to the screen in a formatted form
	*/
	void printPacksByDate(const Date & start, const Date & end) const;

	/**
		Prints all packs matching a given destination and Date interval to the screen in a formatted form
	*/
	void printPacksByDestinationAndDate(const std::string & s, const Date & start, const Date & end) const;


	// OTHER METHODS

	/**
		Remove a Client by inserting a vat or choosing from a list of clients
	*/
	bool removeClient();

	/**
		Returns true if a given VAT is already registered to a Client
	*/
	bool isVatUsed(unsigned vat);


private:
	int maxPackId;
	std::string name, url, fileNameClients, fileNamePacks;
	Address address;
	unsigned vat;
	std::vector<TravelPack> packList;
	std::vector<Client> clientList;

	/** 
		Read one Client from file
		Return false if it fails 
	*/
	bool readClientFromFile(std::ifstream & file, Client & client, unsigned int & lineTracker);

	/**
		Reads one TravelPack from file
		Returns false if it fails
	*/
	bool readPackFromFile(std::ifstream & fin, TravelPack & pack, unsigned int & lineTracker);

};

#endif // AGENCY_H