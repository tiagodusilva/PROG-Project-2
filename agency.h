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

	/**
		Loads all the data related to an Agency
		If isVerbose is true, it print to cout/cerr the status of the operation
	*/
	bool loadData(const std::string & agencyFileName, const bool isVerbose);

	bool readAgencyFromFile(std::ifstream & file, unsigned & lineTracker);

	/**
		Reads a new Client from user input
		Guarentees a unique VAT
		Ctrl + Z to abort input (returns false)
	*/
	bool readNewClientUserInput();

	/**
		Reads all the Clients from a file
	*/
	bool readAllClientsFromFile(std::ifstream & file,  unsigned & lineTracker);

	/**
		Reads a new Pack from user input
		Guarentees a new ID
		Ctrl + Z to abort input (returns false)
	*/
	bool readNewPackUserInput();

	/**
		Reads all the packs from a file
		If file is empty, sets packList to {}
		Returns false if it encounters an unexpected error
		WARNING: Does not check if file exists
	*/
	bool readAllPacksFromFile(std::ifstream & file, unsigned & lineTracker);


	// OUTPUT METHODS

	/**
		Prints all the Agency's clients to the screen
	*/
	void printClients() const;

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
		Saves all the Agency data to the files specified in the object
	*/
	void saveData() const;

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
	bool readClientFromFile(std::ifstream & file, Client & client, unsigned & lineTracker);

	/**
		Reads one TravelPack from file
		Returns false if it fails
	*/
	bool readPackFromFile(std::ifstream & fin, TravelPack & pack, unsigned & lineTracker);

	void printAllClientsToFile() const;

	void printAllPacksToFile() const;

};

#endif // AGENCY_H