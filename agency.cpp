#include "agency.h"

using namespace std;

Agency::Agency()
{
	name = "Undefined Name";
	url = "404";
	fileNameClients = "";
	fileNamePacks = "";
	address = Address();
	nif = 0;
	packList = {};
	clientList = {};
}

Agency::Agency(std::string& agencyFileName)
{
	// TEMP
	name = "Undefined Name";
	url = "404";
	fileNameClients = "";
	fileNamePacks = "";
	address = Address();
	nif = 0;
	packList = {};
	clientList = {};
} // Replace once read agency from file is complete

void stringToLower(std::string& s)
{
	for (size_t i = 0; i < s.size(); i++)
	{
		if (s.at(i) >= 'A' && s.at(i) <= 'Z')
			s.at(i) = s.at(i) + ('a' - 'A');
	}
} // Used for checking locations
