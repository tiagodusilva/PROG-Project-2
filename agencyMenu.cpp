#include "agencyMenu.h"
#include "agency.h"
#include "customUtilities.h"
#include <map>

using namespace std;

/**
	Prints all options to the screen
	To not include an option 0, the first string should be ""
*/
void printMenu(const vector<string> & options)
{
	for (size_t i = 1; i < options.size(); i++)
	{
		cout << "\t" << i << ". " << options.at(i) << "\n";
	}

	if (!options.front().empty())
		cout << "\t0. " << options.at(0) << "\n" << endl;
	else
		cout << endl;
}

void confirmSaveData(const Agency & agency)
{
	char c = '\0';
	if (!cu::readConfirmation(c, "Do you wish to save all changes"))
	{
		cout << "Ctrl + Z detected\nWARNING: Data was not saved" << endl;
		return;
	}
	else
	{
		switch (c)
		{
		case 'y':
			agency.saveData();
			cout << "All data was saved successfully" << endl;
			break;
		default:
			cout << "WARNING: Data was not saved" << endl;
			break;
		}
	}
	return;
}

void viewPacksSoldToClient(Agency & agency)
{
	cout << "--------  Packs sold to Client  --------" << endl;

	int vat;

	if (!cu::readInt(vat, "Client's VAT"))
		return;

	agency.printPacksByClient(vat);

	cout << endl;
	cu::pauseConsole();
}

void viewPacks(const Agency & agency)
{
	int op = -1;
	bool back = false;
	string s;
	Date start, end;

	while (!back)
	{
		cout << "--------  View Packs  --------" << endl << endl;
		printMenu({ "Previous Menu", // 0
			"Only Avaiable", // 1
			"Avaiable Sorted by Destination", // 2
			"Avaiable Sorted by Dates", // 3
			"Avaiable Sorted by Destination and Dates", // 4
			"All", // 5
			"All sorted by Destination", // 6
			"All sorted by Dates", // 7
			"All sorted by Destination and Dates"}); // 8

		if (!cu::readInt(op, "Select Option"))
			return;

		if (op >= 0 && op <= 8)
			system("cls");

		switch (op)
		{
		case 0:
			back = true;
			break;
		case 1:
		case 5:
			if (op == 1)
			{
				cout << "Only avaiable Packs:" << endl;
				agency.printPacks(true);
			}
			else
			{
				cout << "All Packs:" << endl;
				agency.printPacks(false);
			}

			cout << endl;
			cu::pauseConsole();
			break;
		case 2:
		case 6:
			if (op == 2)
				cout << "Avaiable Packs sorted by Destination:" << endl;
			else
				cout << "All Packs sorted by Destination:" << endl;

			cu::readStr(s, "Destination to search");

			cout << endl;
			if (op == 2)
				agency.printPacksByDestination(s, true);
			else
				agency.printPacksByDestination(s, false);
			cout << endl;
			cu::pauseConsole();
			break;
		case 3:
		case 7:
			if (op == 2)
				cout << "Avaiable Packs sorted by Dates:" << endl;
			else
				cout << "All Packs sorted by Dates:" << endl;

			cout << "Date Interval to sort by:" << endl;
			cout << "Starting Date:" << endl;
			start.readUserInput();
			while (end < start)
			{
				if (end < start)
					cout << "Ending Date must not be before Starting Date" << endl;
				cout << "Ending Date:" << endl;
				end.readUserInput();
			}

			cout << endl;
			if (op == 3)
				agency.printPacksByDate(start, end, true);
			else
				agency.printPacksByDate(start, end, false);
			cout << endl;
			cu::pauseConsole();
			break;
		case 4:
		case 8:
			if (op == 2)
				cout << "Avaiable Packs sorted by Destination and Dates:" << endl;
			else
				cout << "All Packs sorted by Destination and Dates:" << endl;

			cu::readStr(s, "Destination to search");

			cout << "Date Interval to sort by:" << endl;
			cout << "Starting Date:" << endl;
			start.readUserInput();

			while (end < start)
			{
				if (end < start)
					cout << "Ending Date must not be before Starting Date" << endl;
				cout << "Ending Date:" << endl;
				end.readUserInput();
			}

			cout << endl;
			if (op == 4)
				agency.printPacksByDestinationAndDate(s, start, end, true);
			else
				agency.printPacksByDestinationAndDate(s, start, end, false);
			cout << endl;
			cu::pauseConsole();
			break;
		default:
			cout << "Wrong Input" << endl;
			cu::pauseConsole();
			break;
		}

		system("cls");
	} // End while (!back)
}

void viewClients(const Agency & agency)
{
	int op = -1, vat;
	bool back = false;

	while (!back)
	{
		cout << "--------  View Clients  --------" << endl << endl;
		printMenu({ "Previous Menu", // 0
			"All", // 1
			"Specific Client"}); // 2

		if (!cu::readInt(op, "Select Option"))
			return;

		if (op >= 0 && op <= 2)
			system("cls");

		switch (op)
		{
		case 0:
			back = true;
			break;
		case 1:
			cout << "All Clients:" << endl;
			agency.printClients();
			cout << endl;
			cu::pauseConsole();
			break;
		case 2:
			if (!cu::readInt(vat, "VAT of the Client to view"))
			{
				cout << "Operation aborted" << endl;
				cu::pauseConsole();
				break;
			}

			agency.printClientByVAT(vat);
			cout << endl;
			cu::pauseConsole();
			break;
		default:
			cout << "Wrong Input" << endl;
			cu::pauseConsole();
			break;
		}

		system("cls");
	} // End while (!back)
}

void removeClientMenu(Agency & agency)
{
	int op = -1, vat;
	bool back = false;

	while (!back)
	{
		cout << "--------  Remove Client  --------" << endl << endl;
		printMenu({ "Previous Menu", // 0
			"Remove Client by VAT", // 1
			"Choose from List" }); // 2

		if (!cu::readInt(op, "Select Option"))
			return;

		if (op >= 0 && op <= 2)
			system("cls");

		switch (op)
		{
		case 0:
			back = true;
			break;
		case 1:
			if (!cu::readInt(vat, "VAT of the client to remove"))
			{
				cout << "Operation aborted" << endl;
				cu::pauseConsole();
				break;
			}

			agency.removeClientByVat(vat);
			cu::pauseConsole();
			break;
		case 2:
			cout << "--------  Remove Client  --------";
			cout << endl << "Client List:" << endl;
			agency.printClientList();
			
			// In this case, vat means index
			if (!cu::readInt(vat, "Select option"))
			{
				cout << "Operation aborted" << endl;
				cu::pauseConsole();
				return;
			}

			agency.removeClientByIndex(vat - 1);

			cu::pauseConsole();
			break;
		default:
			cout << "Wrong Input" << endl;
			cu::pauseConsole();
			break;
		}

		system("cls");
	} // End while (!back)
}

void manageClients(Agency & agency)
{
	int op = -1, vat;
	bool back = false;

	while (!back)
	{
		cout << "--------  Manage Clients  --------" << endl << endl;
		printMenu({ "Previous Menu", // 0
			"Add new Client", // 1
			"Change Client", // 2
			"Remove Client"}); // 3

		if (!cu::readInt(op, "Select Option"))
			return;

		if (op >= 0 && op <= 3)
			system("cls");

		switch (op)
		{
		case 0:
			back = true;
			break;
		case 1: // New Client
			if (agency.readNewClientUserInput())
				cout << "Client added successfully" << endl;
			else
				cout << "Operation aborted" << endl;
			cu::pauseConsole();
			break;
		case 2: // Change Client
			if (!cu::readInt(vat, "Insert Client's VAT number"))
			{
				cout << "Operation aborted" << endl;
				cu::pauseConsole();
				break;
			}

			cout << endl;
			if (agency.changeClient(vat))
				cout << "Client changed successfully" << endl;
			else
				cout << "Operation aborted" << endl;
			cu::pauseConsole();
			break;
		case 3: // Remove Client
			removeClientMenu(agency);
			break;
		default:
			cout << "Wrong Input" << endl;
			cu::pauseConsole();
			break;
		}

		system("cls");
	} // End while (!back)
}

void makePackUnavaiableMenu(Agency & agency)
{
	int op = -1, id;
	bool back = false;

	while (!back)
	{
		cout << "--------  Make Pack Unavaiable  --------" << endl << endl;
		printMenu({ "Previous Menu", // 0
			"Remove Pack by Id", // 1
			"Choose from List" }); // 2

		if (!cu::readInt(op, "Select Option"))
			return;

		if (op >= 0 && op <= 2)
			system("cls");


		vector<int> avaiablePacks = {};
		switch (op)
		{
		case 0:
			back = true;
			break;
		case 1:
			if (!cu::readInt(id, "Id of the Pack to make unavaiable"))
			{
				cout << "Operation aborted" << endl;
				cu::pauseConsole();
				break;
			}

			agency.makePackUnavaiableById(id);
			cu::pauseConsole();
			break;
		case 2:
			cout << "--------  Make Pack Unavaiable  --------";
			cout << endl << "Pack List:" << endl;
			
			for (size_t i = 0; i < agency.getPackListSize(); i++)
			{
				if (agency.getAvaiabilityOfPackAtIndex(i))
				{

				}
			}

			// In this case, id means index
			if (!cu::readInt(id, "Select option"))
			{
				cout << "Operation aborted" << endl;
				cu::pauseConsole();
				return;
			}

			agency.makePackUnavaiableByIndex(id - 1);

			cu::pauseConsole();
			break;
		default:
			cout << "Wrong Input" << endl;
			cu::pauseConsole();
			break;
		}

		system("cls");
	} // End while (!back)
}

void managePacks(Agency & agency)
{
	int op = -1, id;
	bool back = false;

	while (!back)
	{
		cout << "--------  Manage Packs  --------" << endl << endl;
		printMenu({ "Previous Menu", // 0
			"Add new Pack", // 1
			"Change Pack", // 2
			"Make Pack Unavaiable" }); // 3

		if (!cu::readInt(op, "Select Option"))
			return;

		if (op >= 0 && op <= 3)
			system("cls");

		switch (op)
		{
		case 0:
			back = true;
			break;
		case 1: // New Pack
			if (agency.readNewPackUserInput())
				cout << "Pack added successfully" << endl;
			else
				cout << "Operation aborted" << endl;
			cu::pauseConsole();
			break;
		case 2: // Change Pack
			if (!cu::readInt(id, "Insert Pack's Id number"))
			{
				cout << "Operation aborted" << endl;
				cu::pauseConsole();
				break;
			}

			cout << endl;
			if (agency.changePack(id))
				cout << "Pack changed successfully" << endl;
			else
				cout << "Operation aborted" << endl;
			cu::pauseConsole();
			break;
		case 3: // Make Pack Unavaiable
			makePackUnavaiableMenu(agency);
			break;
		default:
			cout << "Wrong Input" << endl;
			cu::pauseConsole();
			break;
		}

		system("cls");
	} // End while (!back)
}

void printAgencyStatistics(Agency & agency)
{
	cout << "Agency statistics:" << endl << endl;
	agency.printStatistics();
	cout << endl;
	cu::pauseConsole();
	return;
}


void agencyMainMenu(Agency & agency)
{
	bool stopProgram = false;
	int op = -1;

	while (!stopProgram)
	{
		system("cls");

		// Imprime o menu principal
		cout << "--------  Main Menu  --------" << endl << endl;
		printMenu({ "Terminate program", // 0
			"Save changes", // 1
			"Manage Clients", // 2
			"Manage Packs", // 3
			"View Clients", // 4
			"View Avaiable Packs", // 5
			"View Packs sold to a specific Client", // 6
			"Purchase a pack", // 7
			"View Sales Statistics", // 8
			"Most Visited Destinations"}); // 9

		cout << "At any point enter Ctrl+Z to cancel or to go back to a previous menu\n" << endl;

		if (!cu::readInt(op, "Insert option"))
		{
			confirmSaveData(agency);
			return;
		}

		if (op >= 0 && op <= 9)
			system("cls");

		// SWITCH STATEMENT HERE
		switch (op)
		{
		case 1:
			confirmSaveData(agency);
			break;
		case 2:
			manageClients(agency);
			break;
		case 3:
			managePacks(agency);
			break;
		case 4:
			viewClients(agency);
			break;
		case 5:
			viewPacks(agency);
			break;
		case 6:
			viewPacksSoldToClient(agency);
			break;
		case 7:
			//purchasePack(agency);
			cout << "Sorry, to do" << endl;
			cu::pauseConsole();
			break;
		case 8:
			printAgencyStatistics(agency);
			break;
		case  9:
			// TODO
			cout << "Sorry, to do" << endl;
			cu::pauseConsole();
			break;
		case 0:
			stopProgram = true;
			break;
		default:
			cout << "Invalid Input" << endl;
			cu::pauseConsole();
			break;
		}

	} // End while(!stopProgram)

	confirmSaveData(agency);
}

