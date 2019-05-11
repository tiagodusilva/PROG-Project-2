#include "travelPack.h"

#include <iostream>
#include <iomanip>

using namespace std;

TravelPack::TravelPack()
{
	id = 0;
	destinations = {};
	departureDate = Date(0, 0, 0);
	returnDate = Date(999999, 12, 31);
	price = 0;
	maxBookings = 0;
	currentBookings = 0;
}

#pragma region GETTERS

int TravelPack::getId() const
{
	return this->id;
}

std::vector<std::string> TravelPack::getDestinations() const
{
	return this->destinations;
}

std::string TravelPack::getDestinationAt(const int index) const
{
	if (index >= (int) this->destinations.size())
		return "";
	return this->destinations.at(index);
}

size_t TravelPack::getDestinationsSize() const
{
	return this->destinations.size();
}

Date TravelPack::getDeparture() const
{
	return this->departureDate;
}

Date TravelPack::getReturn() const
{
	return this->returnDate;
}

int TravelPack::getPrice() const
{
	return this->price;
}

int TravelPack::getMaxBookings() const
{
	return this->maxBookings;
}

int TravelPack::getCurrentBookings() const
{
	return this->currentBookings;
}

#pragma endregion

#pragma region SETTERS

bool TravelPack::setId(const int id)
{
	this->id = id;
	return true;
}

bool TravelPack::setDestinations(const std::vector<std::string>& destinations)
{
	if (destinations.empty())
		return false;

	this->destinations = vector<string>(destinations);
	return true;;
}

bool TravelPack::setDeparture(const Date departureDate)
{
	if (departureDate > this->returnDate)
		return false;

	this->departureDate = departureDate;
	return true;
}

bool TravelPack::setReturn(const Date returnDate)
{
	if (returnDate < this->departureDate)
		return false;

	this->returnDate = returnDate;
	return true;
}

bool TravelPack::setPrice(const int price)
{
	if (price < 0)
		return false;

	this->price = price;
	return true;
}

bool TravelPack::setMaxBookings(const int maxBookings)
{
	if (maxBookings <= 0 || maxBookings < this->currentBookings)
		return false;

	this->maxBookings = maxBookings;
	return true;
}

bool TravelPack::setCurrentBookings(int currentBookings)
{
	if (currentBookings < 0 || currentBookings > this->maxBookings)
		return false;

	this->currentBookings = currentBookings;
	return true;
}

#pragma endregion

// OTHER PUBLIC METHODS

void TravelPack::printSummary() const
{
	cout << left << setw(15) << this->destinations.front() << right
		<< "    - " << this->departureDate
		<< " ~ " << this->returnDate;
}

bool TravelPack::isAvailable() const
{
	if (this->id >= 1)
		return true;
	else
		return false;
}

void TravelPack::makeAvailable()
{
	this->id = abs(this->id);
}

void TravelPack::makeUnavailable()
{
	this->id = -abs(this->id);
}

void TravelPack::updateAvailability()
{
	Date now = Date().now();
	if (this->departureDate < now || this->currentBookings >= this->maxBookings)
		this->makeUnavailable();
}

// OUTPUT STREAM OPERATOR OVERRIDES

std::ostream& operator<<(std::ostream& stream, const TravelPack& pack)
{
	stream << setw(PACK_OUTPUT_ALIGNMENT) << "Id: " << pack.id << endl;

	stream << setw(PACK_OUTPUT_ALIGNMENT) << "Destinations: " << pack.destinations.front();
	if (pack.destinations.size() != 1)
	{
		stream << " - ";

		for (size_t i = 1; i < pack.destinations.size() - 1; i++)
		{
			stream << pack.destinations.at(i) << ", ";
		}

		stream << pack.destinations.back();
	}
	stream << endl;

	stream << setw(PACK_OUTPUT_ALIGNMENT) << "Departure: " << pack.departureDate << endl;
	stream << setw(PACK_OUTPUT_ALIGNMENT) << "Fim: " << pack.returnDate << endl;

	stream << setw(PACK_OUTPUT_ALIGNMENT) << "Price: " << pack.price << endl;
	stream << setw(PACK_OUTPUT_ALIGNMENT) << "Max Bookings: " << pack.maxBookings << endl;
	stream << setw(PACK_OUTPUT_ALIGNMENT) << "Current Bookings: " << pack.currentBookings;

	return stream;
}

std::ofstream& operator<<(std::ofstream& stream, const TravelPack& pack)
{
	stream << pack.id << endl;

	stream << pack.destinations.front();
	if (pack.destinations.size() != 1)
	{
		stream << " - ";

		for (size_t i = 1; i < pack.destinations.size() - 1; i++)
		{
			stream << pack.destinations.at(i) << ", ";
		}

		stream << pack.destinations.back();
	}
	stream << endl;

	stream << pack.departureDate << endl;
	stream << pack.returnDate << endl;
	stream << pack.price << endl;
	stream << pack.maxBookings << endl;
	stream << pack.currentBookings;
	return stream;
}
