#include "agencyMenu.h"
#include "agency.h"
#include "customUtilities.h"

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
}

void viewPacks(const Agency & agency)
{
	int op = -1;
	bool back = false;
	string s;
	Date start, end;

	while (!back)
	{
		cout << "--------  View Available Packs  --------" << endl << endl;
		printMenu({ "Previous Menu", // 0
			"All", // 1
			"Sort by Destination", // 2
			"Sort by Dates", // 3
			"Sort by Destination and Dates" }); // 4

		if (!cu::readInt(op, "Select Option"))
			return;

		system("cls");

		switch (op)
		{
		case 0:
			back = true;
			break;
		case 1:
			cout << "All avaible Packs:" << endl;
			agency.printPacks(true);
			cout << endl;
			cu::pauseConsole();
			break;
		case 2:
			cout << "All available Packs sorted by Destination:" << endl;

			cu::readStr(s, "Destination to search");

			cout << endl;
			agency.printPacksByDestination(s, true);
			cout << endl;
			cu::pauseConsole();
			break;
		case 3:
			cout << "All available Packs sorted by Date" << endl;

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
			agency.printPacksByDate(start, end, true);
			cout << endl;
			cu::pauseConsole();
			break;
		case 4:
			cout << "All available Packs sorted by Destination and Date:" << endl;

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
			agency.printPacksByDestinationAndDate(s, start, end, true);
			cout << endl;
			cu::pauseConsole();
			break;
		default:
			cout << "Wrong Input" << endl;
			cu::pauseConsole();
			break;
		}
		cout << endl;
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
			"Specific Client" }); // 2

		if (!cu::readInt(op, "Select Option"))
			return;

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
			cu::readInt(vat, "Insert Client's VAT number");

			agency.printClientByVAT(vat);
			cout << endl;
			cu::pauseConsole();
			break;
		default:
			cout << "Wrong Input" << endl;
			cu::pauseConsole();
			break;
		}
		cout << endl;
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
			"Remove Client" }); // 3

		if (!cu::readInt(op, "Select Option"))
			return;

		system("cls");

		switch (op)
		{
		case 0:
			back = true;
			break;
		case 1:
			if (agency.readNewClientUserInput())
				cout << "Client added successfully" << endl;
			else
				cout << "Operation aborted" << endl;
			cu::pauseConsole();
			break;
		case 2:
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
		case 3:
			agency.removeClient();
			cu::pauseConsole();
			break;
		default:
			cout << "Wrong Input" << endl;
			cu::pauseConsole();
			break;
		}
		cout << endl;
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
			"Make Pack Unavailable" }); // 3

		if (!cu::readInt(op, "Select Option"))
			return;

		system("cls");

		switch (op)
		{
		case 0:
			back = true;
			break;
		case 1:
			if (agency.readNewPackUserInput())
				cout << "Pack added successfully" << endl;
			else
				cout << "Operation aborted" << endl;
			cu::pauseConsole();
			break;
		case 2:
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
		case 3:
			cout << "Temp :D" << endl;
			cu::pauseConsole();
			break;
		default:
			cout << "Wrong Input" << endl;
			cu::pauseConsole();
			break;
		}
		cout << endl;
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
			"View Available Packs", // 5
			"View Packs sold to a specific Client", // 6
			"Purchase a pack", // 7
			"View Sales Statistics", // 8
			"Most Visited Destinations" }); // 9

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

