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

bool verifyVAT(unsigned int vat, vector<Client> clients) {
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients.at(i).getVAT() == vat)
		{
			cout << "VAT number already in use!\n";
			return false;
		}
	}
	return true;
}

bool Client::changeClient(vector<Client> & clients) {
	int num;
	string str;
	Address auxAddress;

	cout << "1. Name"<< endl << "2. VAT" << endl << "3. Household" << endl
		<<"4. Address" << endl << "0. Cancel"<< endl;
	
	while(true){
	cu::readInt(num, "What do you want to change");
	if (num >= 0 && num <= 4) break;
	else cout << "Not a valid option!" << endl;
	}

	switch (num)
	{
	case 1:
		cu::readStr(str, "Name");
		this->name = str; break;
	case 2:
		while (true) {
			cu::readInt(num, "VAT");
			if(verifyVAT(num, clients)) break;
		}
		this->vat = num; break;
	case 3:
		while (true) {
			cu::readInt(num, "Household");
			if (num >= 1) break;
			cout << "Insert a valid household number" << endl;
		}
		this->household = num; break;
	case 4:
		address.readUserInput();
		this->address = auxAddress; break;
	case 0:
		return false;
	default:
		return false;
	}

	cout << "Client changed sucessful" << endl;

	return true;
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
	if (new_name.empty()) return false;
		this->name = new_name;
		return true;
}

bool Client::setVAT(unsigned new_vat, vector<Client> clients) {

	if (verifyVAT(new_vat, clients)) {
		this->vat = new_vat;
		return true;
	}

	return false;
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
