#ifndef TRAVELPACK_H
#define TRAVELPACK_H

#include "date.h"
#include <vector>
#include <string>

static const int PACK_OUTPUT_ALIGNMENT = 25;

class TravelPack
{
public:
	TravelPack();

	// GETTERS

	int getId() const;
	std::vector<std::string> getDestinations() const;
	Date getDeparture() const;
	Date getReturn() const;
	int getPrice() const;
	int getMaxBookings() const;
	int getCurrentBookings() const;

	// SETTERS

	bool setDestinations(const std::vector<std::string> & destinations);
	bool setDeparture(const Date departureDate);
	bool setReturn(const Date returnDate);
	bool setPrice(const int price);
	bool setMaxBookings(const int maxBookings);
	bool setCurrentBookings(const int currentBookings);

	// Other Methods

	void printSummary();
	friend std::ostream& operator<<(std::ostream& stream, const TravelPack& pack);
	friend std::ofstream& operator<<(std::ofstream& stream, const TravelPack& pack);

private:
	int id;
	std::vector<std::string> destinations;
	Date departureDate, returnDate;
	int price, maxBookings, currentBookings;
};

#endif // TRAVELPACK_H
