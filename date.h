#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <fstream>

class Date
{
public:
	Date();
	Date(unsigned int aYear, unsigned int aMonth, unsigned int aDay);
	unsigned int getDay() const;
	unsigned int getMonth() const;
	unsigned int getYear() const;
	bool isLeapYear() const;
	bool isValid() const;
	bool operator< (const Date& d) const;
	bool operator>= (const Date& d) const;
	bool operator> (const Date& d) const;
	bool operator<= (const Date& d) const;
	bool operator== (const Date& d) const;
	bool operator!= (const Date& d) const;

private:
    unsigned int day, month, year;
};

std::ostream& operator<< (std::ostream& stream, const Date& date);

#endif // DATE_H