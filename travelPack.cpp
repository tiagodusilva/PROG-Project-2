#include "travelPack.h"

#include <iostream>

using namespace std;

TravelPack::TravelPack()
{
	id = 0;
	nif = 0;
	destinations = {};
	departureDate = Date();
	returnDate = Date();
	price = 0;
	maxBookings = 0;
	currentBookings = 0;
}

