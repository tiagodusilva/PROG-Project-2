#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "address.h"

const int CLIENT_OUTPUT_ALIGNMENT = 15;
const std::string CLIENT_OUTPUT_SEPARATOR = " ----------------//----------------";

class Client
{
public:
	Client();
	Client(std::string name, unsigned vat, unsigned short household, Address address);
	Client(std::string name, unsigned vat, unsigned short household, Address address, std::vector<int> & travelPacks, unsigned totalspent);  // client read from file

	// GET methods

	std::string getName() const;
	unsigned getVAT() const;
	unsigned short getHousehold() const;
	Address getAddress() const;
	std::vector<int> getTravelPacksList() const;
	unsigned getTotalSpent() const;

	// SET methods

	bool setName(std::string new_name);
	bool setVAT(unsigned new_VATnumber, std::vector<Client> clients);
	bool setHousehold(unsigned short new_household);
	bool setAddress(Address address);
	bool setTravelPacksList(std::vector<int> & new_travelPacks);
	bool setTotalSpent(unsigned new_totalSpent);

	// OTHER METHODS

	bool changeClient(std::vector<Client> & clients);


	// OUTPUT STREAM OPERATOR OVERRIDES

	friend std::ostream& operator<< (std::ostream & stream, const Client & client);

	friend std::ofstream& operator<< (std::ofstream & stream, const Client & client);

private:
	std::string name;
	unsigned int vat, household, totalSpent;
	std::vector<int> travelPacks;
	Address address;
};

#endif // CLIENT_H