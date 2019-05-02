#include "address.h"
#include "customUtilities.h"

using namespace std;

bool verifyZipCode(const string zip)
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

Address::Address()
{
	this->street = "Undefined Street";
	this->doorNumber = "X";
	this->floor = "X";
	this->zipCode = "XXXX-XXX";
	this->locality = "Undefined locality";

}

#pragma region GETTERS

	string Address::getStreet() const
	{
		return this->street;
	}

	string Address::getDoorNumber() const
	{
		return this->doorNumber;
	}

	string Address::getFloor() const
	{
		return this->floor;
	}

	string Address::getZipCode() const
	{
		return this->zipCode;
	}

	string Address::getLocality() const
	{
		return this->locality;
	}

#pragma endregion

#pragma region SETTERS

	bool Address::setStreet(const std::string street)
	{
		this->street = street;
		return true;
	}

	bool Address::setDoorNumber(const std::string doorNumber)
	{
		this->doorNumber = doorNumber;
		return true;
	}

	bool Address::setFloor(const std::string floor)
	{
		this->floor = floor;
		return true;
	}

	bool Address::setZipCode(const std::string zipCode)
	{
		if (verifyZipCode(zipCode))
		{
			this->zipCode = zipCode;
			return true;
		}
		else
			return false;
	}

	bool Address::setLocality(const std::string locality)
	{
		this->locality = locality;
		return true;
	}

#pragma endregion

std::ostream& operator<<(std::ostream& stream, const Address& address)
{
	stream << address.street << " / " << address.doorNumber <<
		" / " << address.floor << " / " <<
		address.zipCode << " / " << address.locality;
	return stream;
}

std::ofstream& operator<<(ofstream& stream, const Address& address)
{
	stream << address.street << " / " << address.doorNumber <<
		" / " << address.floor << " /" <<
		address.zipCode << " / " << address.locality;
	return stream;
}

// WARNING: Almost all error checking is done in the setters
bool Address::readUserInput()
{
	// cout << "Input Ctrl + Z at any point to interrupt the input" << endl;
	string s;

	do
	{
		if (!cu::readStr(s, "Street Name"))
			return false;
	} while (!this->setStreet(s));

	do
	{
		if (!cu::readStr(s, "Door Numbers"))
			return false;
	} while (!this->setDoorNumber(s));

	char c;

	if (!cu::readConfirmation(c, "Do you wish to insert floor"))
		return false;

	if (c == 'y' || c == 'Y')
	{
		do
		{
			if (!cu::readStr(s, "Floor"))
				return false;
		} while (!this->setFloor(s));
	}
	else
	{
		this->setFloor("-");
	}

	do
	{
		if (!cu::readStr(s, "Door Numbers (XXXX-XXX)"))
			return false;
	} while (!this->setZipCode(s));

	do
	{
		if (!cu::readStr(s, "Locality"))
			return false;
	} while (!this->setLocality(s));

	return true;
}

bool Address::readFromFile(ifstream & fin, unsigned int & lineTracker)
{
	getline(fin, this->street,'/');
	if (fin.eof()) return false;
	
	getline(fin, this->doorNumber, '/');
	if (fin.eof()) return false;

	getline(fin, this->floor, '/');
	if (fin.eof()) return false;

	getline(fin, this->zipCode, '/');
	if (fin.eof()) return false;

	getline(fin, this->locality);
	if (fin.eof()) return false;

	lineTracker++;
	return true;
}

