#include "date.h"
#include <iostream>
#include <iomanip>

using namespace std;

Date::Date()
{
	day = 1;
	month = 1;
	year = 1970;
}

Date::Date(unsigned int aYear, unsigned int aMonth, unsigned int aDay)
{
    year = aYear;
    month = aMonth;
    day = aDay;
}

unsigned int Date::getDay() const
{
    return day;
}

unsigned int Date::getMonth() const
{
    return month;
}

unsigned int Date::getYear() const
{
    return year;
}

ostream& operator<< (ostream& stream, const Date& date)
{
    stream << date.getYear() << "/"
            << setw(2) << setfill('0')
            << date.getMonth() << "/"
            << date.getDay();
    return stream;
}

bool Date::isLeapYear() const // Evaluate if a given year is a leap year
{
	if (this->year % 4 != 0)
		return false;
	if (this->year % 100 != 0)
		return true;
	if (this->year % 400 == 0)
		return true;
	return false;
}

bool Date::isValid() const
{
	const int daysInMonth[13] = { -1, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if (this->month >= 1 && this->month <= 12)
	{
		if (this->month == 2 && this->isLeapYear())
		{
			if (this->day >= 1 && this->day <= 29)
				return true;
			else
				return false;
		}
		else
		{
			if (this->day >= 1 && this->day <= daysInMonth[this->month])
				return true;
			else
				return false;
		}
	}
	else
		return false;
}

bool Date::operator<(const Date& d) const
{
	if (this->year > d.getYear())
		return false;
	if (this->year < d.getYear())
		return true;
	// Both years are the same
	if (this->month > d.getMonth())
		return false;
	if (this->month < d.getMonth())
		return true;
	// Both months are the same
	if (this->day >= d.getDay())
		return false;
    return true; // if (this->day < d.getDay())
}

bool Date::operator>=(const Date& d) const
{
    return !(*this < d);
}

bool Date::operator>(const Date& d) const
{
    if (this->year > d.getYear())
		return true;
	if (this->year < d.getYear())
		return false;
	// Both years are the same
	if (this->month > d.getMonth())
		return true;
	if (this->month < d.getMonth())
		return false;
	// Both months are the same
	if (this->day > d.getDay())
		return true;
    return false; // if (this->day <= d.getDay())
}

bool Date::operator<=(const Date& d) const
{
    return !(*this > d);
}

bool Date::operator==(const Date& d) const
{
    if (this->year != d.getYear() || this->month != d.getMonth() || this->day != d.getDay())
		return false;
    return true;
}

bool Date::operator!=(const Date& d) const
{
    return !(*this == d);
}
