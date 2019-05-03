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

Client::Client(string name, unsigned vat, unsigned short household, Address address, vector<int> & travelPacks, unsigned totalSpent) {
	this->name = name;
	this->vat = vat;
	this->household = household;
	this->address = address;
	this->travelPacks = travelPacks;
	this->totalSpent = totalSpent;
}

vector<int> Client::readTravelPacksID(string travelPacks, unsigned int & lineTracker) {
	int id;
	stringstream sstream(travelPacks);
	vector<int> travelPacksVector;
	while (sstream >> id) {
		travelPacksVector.push_back(id);
		if (sstream.peek() == ';')
			sstream.ignore();
	}
	lineTracker++;
	return travelPacksVector;
}

bool Client::readFromFile(ifstream & file, unsigned int & lineTracker) {
	string name, vat, household, totalSpent, aux, travelPacks, address; // cons
	vector<int> travelPacksVector;

	Agency ag; // para teste

	while (!cu::isFileEmpty(file))
	{
		getline(file, name); lineTracker++;
		getline(file, vat); lineTracker++;
		getline(file, household); lineTracker++;
		getline(file, address); lineTracker++;
		getline(file, travelPacks); lineTracker++;
		getline(file, totalSpent); lineTracker++;

		if (!cu::isFileEmpty(file)) {
			getline(file, aux); lineTracker++;
		}

		travelPacksVector = readTravelPacksID(travelPacks, lineTracker);
		Client newClient(name, stoi(vat), stoi(household), Address::Address(), travelPacksVector, stoi(totalSpent));

		ag.clientList.push_back(newClient);
	}

	for (int i = 0; i < ag.clientList.size(); i++) {  // para teste
		cout << ag.clientList[i].getName() << endl;
	}

	lineTracker++;
	return true;
}

// GET methods

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

// set methods

bool Client::setName(string new_name) {
	if (new_name.size() == 0) return false;
	this->name = new_name;
	return true;
}

bool Client::setVAT(unsigned new_vat) {
	pair <map<unsigned int, string>::iterator, bool> p;
	p = this->vatNumbersInUse.insert(pair<unsigned int, string>(new_vat, this->name));
	if (p.second == false) {
		cout << "VAT number already in use.\n";
		return false;
	}
	this->vat = new_vat;
	return true;
}

bool Client::setHousehold(unsigned short new_household) {
	this->household = new_household;
	return true;
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

ostream& operator<<(ostream& os, const Client & client) {
	return os;
}


