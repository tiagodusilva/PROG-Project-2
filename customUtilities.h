#ifndef CUSTOMUTILITIES_H
#define CUSTOMUTILITIES_H

#include <string>
#include <fstream>

namespace cu
{
	void strLower(std::string & s);

	void strTrimLeft(std::string & s);

	void strTrimRight(std::string & s);

	void strTrim(std::string & s);

	bool isFileEmpty(std::ifstream & fin);
}

#endif