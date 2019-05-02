#ifndef ADDRESS_H
#define ADDRESS_H

#include <string>
#include <iostream>
#include <fstream>

class Address
{
public:
	Address();
	friend std::ostream& operator<< (std::ostream& stream, const Address& address);
	bool readUserInput();
	bool readFromFile(std::ifstream& fin, unsigned int& lineTracker);
private:
	std::string street, doorNumber, floor, zipCode, locality;
};

#endif // ADDRESS_H