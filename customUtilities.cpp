#include "customUtilities.h"
#include <iostream>

using namespace std;

namespace cu
{
	bool isNum(const string& s)
	{
		for (size_t i = 0; i < s.size(); i++)
			if (s.at(i) < '0' || s.at(i) > '9')
				return false;
		return true;
	}

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

	bool readInt(int & n, const string msg)
	{
		cout << msg << " ? ";
		while (!(cin >> n))
		{
			if (cin.eof())
			{
				cin.clear();
				return false;
			}
			else
			{
				cin.clear();
				cin.ignore(1000, '\n');
			}
			cout << msg << " ? ";
		}

		cin.ignore(1000, '\n');
		return true;
	}

	bool readUnsigned(unsigned & u, const string msg)
	{
		string s;
		do
		{
			try
			{	
				cout << msg << " ? ";
				getline(cin, s);
				if (cin.eof())
				{
					cin.clear();
					return false;
				}
				if (s.find('-') == string::npos)
				{
					u = stoul(s);
					return true;
				}
			}
			catch (...) {}
		} while (true);
	}

	bool readStr(string & s, const string msg)
	{
		do
		{
			cout << msg << " ? ";
			getline(cin, s);
			if (cin.eof())
			{
				cin.clear();
				return false;
			}
		} while (s.empty());
		return true;
	}

	bool readConfirmation(char & c, const string msg)
	{
		string s;
		do
		{
			cout << msg << " (y/n) ? ";
			getline(cin, s);
			if (cin.eof())
			{
				cin.clear();
				return false;
			}
		} while (s.size() != 1 || !(s.at(0) == 'y' || s.at(0) == 'Y' || s.at(0) == 'n' || s.at(0) == 'N'));

		strLower(s);
		c = s.at(0);
		return true;
	}

	void pauseConsole()
	{
		string trash;
		cout << "Press Enter to continue..." << endl;
		getline(cin, trash);
	}

	void clearConsole()
	{
		// Most portable way without using system(), conio.h, ncurses or an OS specific implementation
		// Saldy just moves everything up 40 lines...
		cout << string(40, '\n');
	}

	bool checkStream(std::istream & stream, std::string & error)
	{
		if (stream.eof())
		{
			error = "Reached end of file unexpectedly";
			return false;
		}
		if (stream.fail())
		{
			error = "Unexpected input";
			return false;
		}
		return true;
	}

}

// T1G02
// up201800170 Breno Accioly
// up201806516 Tiago Silva