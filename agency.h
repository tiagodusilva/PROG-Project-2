#ifndef AGENCY_H
#define AGENCY_H

#include "travelPack.h"
#include "address.h"
#include "client.h"
#include <string>
#include <vector>
#include <fstream>
#include <map>

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
	std::vector<Client> getClientList() const;
	size_t getClientListSize() const;
	std::vector<TravelPack> getPackList() const;
	size_t getPackListSize() const;
	TravelPack getPackWithId(const int id) const;
	// Will return false if pack is not found
	bool getAvailabilityOfPack(const int id) const;
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
	bool readAllClientsFromFile(std::ifstream & file, unsigned & lineTracker);

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
		Prints a client with a given vat to the screen
		Returns false if something goes wrong
	*/
	bool printClientByVAT(unsigned vat) const;

	/**
		Prints all the Agency's clients to the screen
	*/
	void printClients() const;

	/**
		Prints all Clients in a short, numbered list, from 1 to n
		Prints by order in the vector, so that 1 corresponds to index 0
	*/
	void printClientList() const;

	/**
		Prints all the packs a Client has bought
	*/
	void printPacksByClient(const int vat) const;

	/**
		Prints the pack with a given ID
		Returns false if something goes wrong
	*/
	bool printPackById(const int) const;

	/**
		Prints all the packs that have been bought by every client
	*/
	void printPacksSoldToEveryone() const;

	/**
		Prints all packs that have been sold at least once
	*/
	void printPacksSold() const;

	/**
		Prints all packs to the screen in a formatted form
	*/
	void printPacks(const bool onlyAvailable) const;

	/**
		Prints all packs matching a given destination to the screen in a formatted form
	*/
	void printPacksByDestination(const std::string & s, const bool onlyAvailable) const;

	/**
		Prints all packs matching a given Date interval to the screen in a formatted form
	*/
	void printPacksByDate(const Date & start, const Date & end, const bool onlyAvailable) const;

	/**
		Prints all packs matching a given destination and Date interval to the screen in a formatted form
	*/
	void printPacksByDestinationAndDate(const std::string & s, const Date & start, const Date & end, const bool onlyAvailable) const;

	/**
		Prints the statistics of agency
	*/
	void printStatistics() const;

	/**
		Prints the N most visited destinations by all clients, N is given
	*/
	void printMostVisitedDestinations(const int n) const;

	/**
		Prints the recommendation of a pack for all clients
	*/
	void printClientRecommendations() const;

	// OTHER METHODS

	/**
		Saves all the Agency data to the files specified in the object
	*/
	void saveData() const;

	/*
		Generates a multimap with the most popular destinations
		Multimap<int, string>
	*/
	void generatePopularDestinations(std::multimap<int, std::string>& reversedPackMap) const;

	/**
		Sell a pack by the given VAT and pack ID
	*/
	bool purchasePack(const int vat, const int id, const int tickets);

	/**
		Remove a Client by inserting a vat or choosing from a list of clients
		Remove a Client through its VAT number
	*/
	bool removeClientByVat(const int vat);

	bool removeClientByIndex(const int index);

	/**
		Makes the pack with the given ID permanently unavaiable
	*/
	bool makePackUnavailableById(const int id);

	bool makePackUnavailableByIndex(const int index);

	/**
		Change a Client by a given vat
	*/
	bool changeClient(unsigned vat);

	/*
		Change a pack by a given id
	*/
	bool changePack(const int id);

	/**
		Returns true if a given VAT is already registered to a Client
	*/
	bool isVatUsed(unsigned vat) const;

	/**
		Creates a map with <option, packId> of all Packs
		Map will have a size of packCounter
		If onlyAvailable is true, it will only process available Packs
		If printMap is true, it will print the list to the screen
		Returns false if the map is empty when the methods finishes
	*/
	bool packMap(std::map<int, int>& packMap, int & packCounter, const bool onlyAvailable, const bool printMap) const;

	/**
		Creates with <option, clientVat> of all Clients
		Map will have a size of clientCounter
		If printMap is true, it will print the list to the screen
		Returns false if the map is empty when the methods finishes
	*/
	bool clientMap(std::map<int, int>& clientMap, int & clientCounter, const bool printMap) const;

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

	/**
		Returns a pointer to the client with the given VAT
		Returns nullptr if not found
	*/
	Client* getClientPointer(const int vat);

	/*
		Returns a pointer to the TravelPack with the given Id
		Returns nullptr if not found
	*/
	TravelPack* getPackPointer(const int id);

};

#endif // AGENCY_H