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

	void strTrimLeft(string & s)
	{

	}

	void strTrimRight(string & s)
	{

	}

	void strTrim(string & s)
	{
		cu::strTrimLeft(s);
		cu::strTrimRight(s);
	}

	bool isFileEmpty(ifstream & fin)
	{
		return fin.peek() == std::ifstream::traits_type::eof();
	}

}
