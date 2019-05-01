#include "address.h"

using namespace std;

Address::Address()
{
	this->street = "Undefined Street";
	this->doorNumber = "X";
	this->floor = "X";
	this->zipCode = "XXXX-XXX";
	this->locality = "Undefined locality";

}

bool verifyZipCode(string & zip)
{
	if (zip.size() != 8)
		return false;
	if (!isdigit(zip[0]))
		return false;
	if (!isdigit(zip[1]))
		return false;
	if (!isdigit(zip[2]))
		return false;
	if (!isdigit(zip[3]))
		return false;
	if (zip[4] != '-')
		return false;
	if (!isdigit(zip[5]))
		return false;
	if (!isdigit(zip[6]))
		return false;
	if (!isdigit(zip[7]))
		return false;
	return true;
}
