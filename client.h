#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <iostream>
#include <vector>
#include "agency.h"
#include "address.h"
#include <map>

class Client
{
public:
	Client();
	Client(std::string name, unsigned vat, unsigned short household, Address address, std::vector<int> & travelPacks, unsigned totalspent);  // client read from file
	bool readFromFile(std::ifstream & file, unsigned int & lineTracker);
	// GET methods

	std::string getName() const;
	unsigned getVAT() const;
	unsigned short getHousehold() const;
	Address getAddress() const;
	std::vector<int> getTravelPacksList() const;
	unsigned getTotalSpent() const;

	// SET methods

	bool setName(std::string new_name);
	bool setVAT(unsigned new_VATnumber);
	bool setHousehold(unsigned short new_household);
	bool setAddress(Address address);
	bool setTravelPacksList(std::vector<int> & new_travelPacks);
	bool setTotalSpent(unsigned new_totalSpent);

	// other methods
	std::vector<int> readTravelPacksID(std::string travelPacks, unsigned int & lineTracker);
	friend class Agency;

private:
	std::map<unsigned int, std::string> vatNumbersInUse;
	std::string name;
	unsigned int vat, household, totalSpent;
	std::vector<int> travelPacks;
	Address address;
};

#endif // CLIENT_H