#ifndef CUSTOMUTILITIES_H
#define CUSTOMUTILITIES_H

#include <string>
#include <fstream>

namespace cu
{
	/**
		Changes all ASCII letters in string s to lowecase
	*/
	void strLower(std::string & s);

	/**
		Changes all ASCII letters in string s to uppercase
	 */
	void strUpper(std::string& s);

	/**
		Trims all chars c from the left of string s
	*/
	void strTrimLeft(std::string& s, char c);

	/**
		Trims all whitespace from the left of string s
	*/
	void strTrimLeft(std::string & s);

	/**
		Trims all chars c from the right of string s
	*/
	void strTrimRight(std::string& s, char c);

	/**
		Trims all whitespace from the right of string s
	*/
	void strTrimRight(std::string & s);

	/**
		Trims all char c from the right and left of string s
	*/
	void strTrim(std::string& s, char c);

	/**
		Trims all whitespace from the right and left of string s
	*/
	void strTrim(std::string & s);

	/**
		Returns trrue if a given filestream is empty
		Meant to be used right after opening an ifstream
	*/
	bool isFileEmpty(std::ifstream & fin);
}

#endif