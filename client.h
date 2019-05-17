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
	Client(std::string name, unsigned vat, unsigned short household, Address address, std::vector<int> & travelPacks, unsigned totalspent);  // client read from file

	// GET methods

	std::string getName() const;
	unsigned getVAT() const;
	unsigned short getHousehold() const;
	Address getAddress() const;
	std::vector<int> getTravelPacksList() const;
	size_t getTravelPacksListSize() const;
	/**
		If index is out of range, returns 0
	*/
	int getTravelPackAt(int index) const;
	unsigned getTotalSpent() const;

	// SET methods

	bool setName(std::string new_name);
	bool setVAT(unsigned new_VATnumber);
	bool setHousehold(unsigned short new_household);
	bool setAddress(Address address);
	bool setTravelPacksList(std::vector<int> & new_travelPacks);
	bool setTotalSpent(unsigned new_totalSpent);

	// OTHER PUBLIC METHODS

	void printSummary() const;

	/**
		Returns true if the user has bought the pack
		Id should always be posivite
	*/
	bool hasBought(const int id) const;

	void addPack(const int id, const int price, const int tickets);

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

// T1G02
// up201800170 Breno Accioly
// up201806516 Tiago Silva