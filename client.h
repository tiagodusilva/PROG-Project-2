#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <vector>
#include "address.h"

class Client
{
public:
	Client();

private:
	std::string name;
	unsigned int vat, household, totalSpent;
	std::vector<int> travelPacks;
	Address address;
};

#endif // CLIENT_H