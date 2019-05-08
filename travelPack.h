#ifndef TRAVELPACK_H
#define TRAVELPACK_H

#include "date.h"
#include <vector>
#include <string>

static const int PACK_OUTPUT_ALIGNMENT = 20;

class TravelPack
{
public:
	TravelPack();

	// GETTERS

	int getId() const;
	std::vector<std::string> getDestinations() const;
	// Returns "" if pos is out of range
	std::string getDestinationAt(const int pos) const;
	int getDestinationsSize() const;
	Date getDeparture() const;
	Date getReturn() const;
	int getPrice() const;
	int getMaxBookings() const;
	int getCurrentBookings() const;

	// SETTERS

	/**
		Use with caution, so that ID never overlap!
	*/
	bool setId(const int id);
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
	void makeAvaiable();
	void makeUnavaiable();
	/**
		Never sets packs to avaiable, only to unavaiable
		Checks if the departureDate has already passed and if the capacity is full
	*/
	void updateAvaiability();

private:
	int id;
	std::vector<std::string> destinations;
	Date departureDate, returnDate;
	int price, maxBookings, currentBookings;

};

#endif // TRAVELPACK_H
