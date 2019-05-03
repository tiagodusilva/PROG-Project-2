#ifndef AGENCY_H
#define AGENCY_H

#include "travelPack.h"
#include "address.h"
#include "client.h"
#include <string>
#include <vector>

class Agency
{
public:
	Agency();
	Agency(std::string & agencyFileName);

private:
	std::string name, url, fileNameClients, fileNamePacks;
	Address address;
	int nif;
	std::vector<TravelPack> packList;
	std::vector<Client> clientList;
};

#endif // AGENCY_H