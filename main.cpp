#include "agency.h"
#include <iostream>

using namespace std;

int main()
{
	// Currently contains random stuff meant for testing
	
	// Right now reads all the Agency data and prints all packs to the screen
	Agency agency;
	agency.loadData("agencyTest.txt", true);

	agency.printPacks();

}
