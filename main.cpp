#include "date.h"
#include "travelPack.h"
#include <iostream>

using namespace std;

int main()
{
	// Currently contains random stuff meant for testing
	string a, b, c;
	getline(cin, a, '/');
	getline(cin, b, '/');
	if (cin.fail())
		cout << "Failed\n";
	getline(cin, c);
	cout << a << '_' << b << '_' << c << endl;
}
