#include "client.h"
#include "agency.h"
#include "customUtilities.h"
#include <fstream>
#include <sstream>

using namespace std;

const int CLIENT_OUTPUT_ALIGNMENT = 25;

Client::Client()
{
	name = "John Doe";
	vat = 0;
	household = 1;
	address = Address();
	totalSpent = 0;
	travelPacks = {};
}

Client::Client(string name, unsigned vat, unsigned short household, Address address) {
	this->travelPacks = {};
	this->totalSpent = 0;
	this->name = name;
	this->vat = vat;
	this->household = household;
	this->address = address;
}

Client::Client(string name, unsigned vat, unsigned short household, Address address, vector<int> & travelPacks, unsigned totalSpent) {
	this->name = name;
	this->vat = vat;
	this->household = household;
	this->address = address;
	this->travelPacks = travelPacks;
	this->totalSpent = totalSpent;
}


//    GET METHODS

string Client::getName() const {
	return name;
}

unsigned Client::getVAT() const {
	return vat;
}

unsigned short Client::getHousehold() const {
	return household;
}

Address Client::getAddress() const {
	return address;
}

vector<int> Client::getTravelPacksList() const {
	return travelPacks;
}

unsigned Client::getTotalSpent() const {
	return totalSpent;
}

//    SET METHODS

bool Client::setName(string new_name) {
	if (new_name.size() == 0) return false;
	this->name = new_name;
	return true;
}

bool Client::setVAT(unsigned new_vat, vector<Client> clients) {

	for (size_t i = 0; i < clients.size(); i++)
		{
			if (clients.at(i).vat == new_vat)
			{
				cout << "VAT number already in use!\n";
				return false;
			}
	}
	this->vat = new_vat;
	
	return true;
}

bool Client::setHousehold(unsigned short new_household) {
	if (new_household >= 1) {
		this->household = new_household;
		return true;
	}
	return false;
}

bool Client::setAddress(Address new_address) {
	this->address = new_address;
	return true;
}

bool Client::setTravelPacksList(vector<int> & new_travelPacks) {
	this->travelPacks = new_travelPacks;
	return true;
}

bool Client::setTotalSpent(unsigned new_totalSpent) {
	this->totalSpent = new_totalSpent;
	return true;
}

// not used 
ostream& operator<<(ostream& os, const Client & client) {
	return os;
}

