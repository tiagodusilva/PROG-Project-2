#include "customUtilities.h"

using namespace std;

namespace cu
{
	void strLower(string & s)
	{
		for (size_t i = 0; i < s.size(); i++)
		{
			if (s.at(i) >= 'A' && s.at(i) <= 'Z')
				s.at(i) = s.at(i) + ('a' - 'A');
		}
	}

	void strUpper(string& s)
	{
		for (size_t i = 0; i < s.size(); i++)
		{
			if (s.at(i) >= 'a' && s.at(i) <= 'z')
				s.at(i) = s.at(i) + ('A' - 'a');
		}
	}

	void strTrimLeft(string & s, char c)
	{
		int index = s.find_first_not_of(c);
		s.erase(0, index);
	}

	void strTrimLeft(string& s)
	{
		strTrimLeft(s, ' ');
	}

	void strTrimRight(string & s, char c)
	{
		int index = s.find_last_not_of(c);
		s.erase(index + 1);
	}

	void strTrimRight(string& s)
	{
		strTrimRight(s, ' ');
	}

	void strTrim(string & s, char c)
	{
		strTrimLeft(s, c);
		strTrimRight(s, c);
	}

	void strTrim(string& s)
	{
		strTrimLeft(s, ' ');
		strTrimRight(s, ' ');
	}

	bool isFileEmpty(ifstream & fin)
	{
		return fin.peek() == ifstream::traits_type::eof();
	}

}
