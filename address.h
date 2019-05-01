#ifndef ADDRESS_H
#define ADDRESS_H

#include <string>

class Address
{
public:
	Address();

private:
	std::string street, doorNumber, floor, zipCode, locality;
};

#endif // ADDRESS_H