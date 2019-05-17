#ifndef CUSTOMUTILITIES_H
#define CUSTOMUTILITIES_H

#include <string>
#include <fstream>

namespace cu
{
	/**
		Returns true if all characters from string s are digits
	*/
	bool isNum(const std::string& s);
	
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

	/**
		Read an int from user input, taking care of invalid inputs (not ints)
		Presents msg + " ? "
		Return false if user interrupts with Ctrl+Z
	*/
	bool readInt(int& n, const std::string msg);

	/**
		Read an unsigned from user input, taking care of invalid inputs (not unsiged, including negatives)
		Presents msg + " ? "
		Return false if user interrupts with Ctrl+Z	
	*/
	bool readUnsigned(unsigned & n, const std::string msg);

	/**
		Read a string from user input, waits for a not empty string
		Presents msg + " ? "
		Return false if user interrupts with Ctrl+Z
	*/
	bool readStr(std::string& s, const std::string msg);

	/**
		Read a confirmation from user input (expects y/n answer)
		Presents msg + " (y/n) ? "
		If it return true, c will always "y" or "n" (lowercase)
		Return false if user interrupts with Ctrl+Z
	*/
	bool readConfirmation(char& c, const std::string msg);

	/**
		Pauses the program's execution and waits for any user input
		Actual method for that may change in the future
	*/
	void pauseConsole();

	/**
		Clear the console... or somthing similar in effect
		Method may change in the future
	*/
	void clearConsole();

	/**
		Checks stream.eof() and stream.fail()
		When it returns false, a more detailed message is given in the error string
	*/
	bool checkStream(std::istream & stream, std::string & error);
}

#endif

// T1G02
// up201800170 Breno Accioly
// up201806516 Tiago Silva