#ifndef ADDRESS_H
#define ADDRESS_H

#include <string>
#include <iostream>
#include <fstream>

class Address
{
public:
	Address();

	// GET methods

	std::string getStreet() const;
	std::string getDoorNumber() const;
	std::string getFloor() const;
	std::string getZipCode() const;
	std::string getLocality() const;

	// SET methods

	bool setStreet(const std::string street);
	bool setDoorNumber(const std::string doorNumber);
	bool setFloor(const std::string floor);
	bool setZipCode(const std::string zipCode);
	bool setLocality(const std::string  locality);

	// Other methods

	/**
		Outputs Address to ostream, does not get called on ofstream
	*/
	friend std::ostream& operator<< (std::ostream& stream, const Address& address);

	/**
		Literally only differs from a regular ostream because of a goddamned whitespace...
	*/
	friend std::ofstream& operator<<(std::ofstream& stream, const Address& address);

	/**
		Read an Address from user input
		Ctrl + Z to abort input (returns false)
	*/
	bool readUserInput();

	/**
		Call to read an Address from a file (ifstream)
		LineTracker is used to be able to produce a better error message in case something breaks
		Returns false if the file ends or an unexpected input is found
	*/
	bool readFromFile(std::ifstream& fin, unsigned int& lineTracker);

private:
	std::string street, doorNumber, floor, zipCode, locality;
};

#endif // ADDRESS_H