#include "client.h"

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
