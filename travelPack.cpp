#include "travelPack.h"

#include <iostream>

using namespace std;

TravelPack::TravelPack()
{
	id = 0;
	destinations = {};
	departureDate = Date();
	returnDate = Date();
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
