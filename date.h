#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <fstream>

class Date
{
public:
	Date();
	/**
		WARNING: Does not check if crated Date is valid!
	*/
	Date(unsigned int aYear, unsigned int aMonth, unsigned int aDay);
	/**
		If the string is in a wrong format, it will set Date to an invalid state
		(y = 0, m = 0, d = 0)
		WARNING: Does not check if crated Date is valid!
	*/
	Date(const std::string& s);
	
	/**
		Returns a Date object with the current system date
		May only work in visual studio, requires additional testing
	*/
	Date now();

	/*
		Reads a Date from user input
		Guarentees a valid Date
		Ctrl + Z to abort input (returns false)
	*/
	bool readUserInput();
	bool readFromFile(std::ifstream& fin, unsigned int& lineTracker);

	unsigned int getDay() const;
	unsigned int getMonth() const;
	unsigned int getYear() const;

	/**
		Returns true if it's a leap year
	*/
	bool isLeapYear() const;
	/*
		Returns true if the Date object is valid
		Checks days in months, month belonging [1, 12] and leap yearss 
	*/
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