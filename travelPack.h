#ifndef TRAVELPACK_H
#define TRAVELPACK_H

#include "date.h"
#include <vector>
#include <string>


class TravelPack
{
public:
	TravelPack();

private:
	static int lastId;
	int id, nif;
	std::vector<std::string> destinations;
	Date departureDate, returnDate;
	int price, maxBookings, currentBookings;
	
	friend class Agency;
};

#endif // TRAVELPACK_H
