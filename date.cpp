#include "date.h"
#include "customUtilities.h"
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

Date::Date(const string& s)
{
	int start = 0, end = s.find('/');
	string sub;
	if (end != string::npos)
	{
		sub = s.substr(0, end);
		cu::strTrim(sub);
		this->year = stoi(s);

		start = end + 1;
		end = s.find('/', start);
		if (end != string::npos)
		{
			sub = s.substr(start, end - start);
			cu::strTrim(sub);
			this->month = stoi(sub);

			sub = s.substr(end + 1, string::npos);
			cu::strTrim(sub);
			this->day = stoi(sub);

			return;
		}
	}

	// Only executes if a verification has failed above
	this->year = 0;
	this->month = 0;
	this->day = 0;
}

Date Date::now()
{

		// God Bless Marantes

		/* current date/time based on current system */

		/* LocalTime for MinGW compiler */
		//time_t now = time(0);
		//tm* ltm = localtime(&now);

		/* LocalTime for Microsoft Visual Studio compiler */
		time_t now = time(0);
		tm* ltm = new tm();
		localtime_s(ltm, &now);

		Date currentDate = Date( 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday );
		return currentDate;
}

bool Date::readUserInput()
{
	int y, m, d;
	bool fail = false;

	do
	{
		if (fail)
			cout << "Invalid Date!" << endl;
		if (!cu::readInt(y, "Year"))
			return false;		
		if (!cu::readInt(m, "Month"))
			return false;		
		if (!cu::readInt(d, "Day"))
			return false;

		*this = Date(y, m, d);
		fail = !this->isValid();
	} while (fail);

	return true;

}

bool Date::readFromFile(ifstream& fin, unsigned int& lineTracker)
{
	string s;
	getline(fin, s);
	if (fin.eof()) return false;

	*this = Date(s);

	if (!this->isValid())
		return false;
	
	lineTracker++;
	return true;
}

#pragma region GETTERS

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

#pragma endregion

ostream& operator<< (ostream& stream, const Date& date)
{
    stream << setfill('0') <<
			setw(4) << date.getYear() << "/" <<
			setw(2) << date.getMonth() << "/" <<
			setw(2) << date.getDay() <<
			setfill(' ');
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
	const unsigned int daysInMonth[13] = { -1, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
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
			if (this->day >= (unsigned) 1 && this->day <= daysInMonth[this->month])
				return true;
			else
				return false;
		}
	}
	else
		return false;
}

#pragma region OPERATOR OVERRIDES

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

bool Date::operator>=(const Date & d) const
{
	return !(*this < d);
}

bool Date::operator>(const Date & d) const
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

bool Date::operator<=(const Date & d) const
{
	return !(*this > d);
}

bool Date::operator==(const Date & d) const
{
	if (this->year != d.getYear() || this->month != d.getMonth() || this->day != d.getDay())
		return false;
	return true;
}

bool Date::operator!=(const Date & d) const
{
	return !(*this == d);
}

#pragma endregion
