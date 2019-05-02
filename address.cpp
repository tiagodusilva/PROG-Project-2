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

std::ostream& operator<<(std::ostream& stream, const Address& address)
{
	stream << address.street << " / " << address.doorNumber <<
		" / " << address.floor << " / " <<
		address.zipCode << " / " << address.locality;
	return stream;
}

// Needs more error checking!
bool Address::readUserInput()
{
	cout << "Input Ctrl + Z at any point to interrupt the input" << endl;
	do
	{
		cout << "Street name ? ";
		getline(cin, this->street);
		if (cin.eof())
			return false;
	} while (this->street.empty());

	do
	{
		cout << "Door number ? ";
		getline(cin, this->street);
		if (cin.eof())
			return false;
	} while (this->street.empty());

	char c = 'o';

	cout << "Do you wish to insert floor (y/n) ? ";
	while (!(cin >> c) || !(c == 'y' || c == 'n' || c == 'Y' || c == 'N'))
	{
		if (cin.eof())
			return false;
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Do you wish to insert floor (y/n) ? ";
	}

	cin.ignore(1000, '\n');

	if (c == 'y' || c == 'Y')
	{
		do
		{
			cout << "Floor ? ";
			getline(cin, this->floor);
			if (cin.eof())
				return false;
		} while (this->floor.empty());
	}
	else
	{
		this->floor = "-";
	}

	do
	{
		cout << "Zip code (XXXX-XXX) ? ";
		getline(cin, this->zipCode);
		if (cin.eof())
			return false;
		// Warn user of wrong format
	} while (this->zipCode.empty() || !verifyZipCode(this->zipCode));


	do
	{
		cout << "Locality ? ";
		getline(cin, this->locality);
		if (cin.eof())
			return false;
	} while (this->locality.empty());

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

