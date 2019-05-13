#include "agencyMenu.h"
#include "agency.h"
#include "customUtilities.h"
#include <map>

using namespace std;

// Private Functions

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
	cout << "--------  Save Changes  --------" << endl << endl;
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

bool selectPackMenu(const Agency & agency, int & id, const string & menuTitle, const bool onlyAvailable)
{
	int op = -1;

	while (true)
	{
		cout << "--------  " << menuTitle << "  --------" << endl << endl;
		printMenu({ "Previous Menu", // 0
			"Insert Pack's Id", // 1
			"Select Pack from List" }); // 2

		if (!cu::readInt(op, "Select Option"))
			return false;

		if (op >= 0 && op <= 2)
			system("cls");

		map<int, int> packMap;
		int packsFound;
		switch (op)
		{
		case 0:
			return false;
			break;
		case 1:
			if (!cu::readInt(id, "Pack's Id"))
			{
				cout << "Operation aborted" << endl;
				cu::pauseConsole();
				break;
			}
			if (onlyAvailable && agency.getAvailabilityOfPack(id))
			{
				cout << "Pack is unavaiable, which is not suitable for this operation" << endl;
				cu::pauseConsole();
				break;
			}

			return true;
			break;
		case 2:
			cout << "--------  Pack List  --------" << endl << endl;

			if (!agency.packMap(packMap, packsFound, onlyAvailable, true))
			{
				cout << "Operation aborted" << endl;
				cu::pauseConsole();
				break;
			}
			else
			{
				cout << "\t0. Cancel\n" << endl;
				// In this case, id means option
				if (!cu::readInt(id, "Select option") || id == 0)
				{
					cout << "Operation aborted" << endl;
					cu::pauseConsole();
					break;
				}

				if (id < 0 || id > packsFound)
				{
					cout << "Option out of range\nOperation aborted" << endl;
					cu::pauseConsole();
					break;
				}

				id = packMap[id];
				return true;
			}
			break;
		default:
			cout << "Wrong Input" << endl;
			cu::pauseConsole();
			break;
		}

		system("cls");
	} // End while (!back)}

	return false; // Should never get here
}

bool selectClientMenu(const Agency & agency, int & vat, const string & menuTitle)
{
	int op = -1;

	while (true)
	{
		cout << "--------  " << menuTitle << "  --------" << endl << endl;
		printMenu({ "Previous Menu", // 0
			"Insert Client's VAT", // 1
			"Select from List" }); // 2

		if (!cu::readInt(op, "Select Option"))
			return false;

		if (op >= 0 && op <= 2)
			system("cls");

		// map <option, vat>
		map<int, int> clientMap;
		int clientCounter;

		switch (op)
		{
		case 0:
			return false;
			break;
		case 1:
			if (!cu::readInt(vat, "Client's VAT"))
			{
				cout << "Operation aborted" << endl;
				cu::pauseConsole();
				break;
			}

			return true;
			break;
		case 2:
			cout << "--------  Client List  --------" << endl << endl;
			if (!agency.clientMap(clientMap, clientCounter, true))
			{
				cout << "Operation aborted" << endl;
				cu::pauseConsole();
				break;
			}
			else
			{
				cout << "\t0. Cancel\n" << endl;
				// In this case, vat means option
				if (!cu::readInt(vat, "Select option") || vat == 0)
				{
					cout << "Operation aborted" << endl;
					cu::pauseConsole();
					return false;
				}

				if (vat < 0 || vat > clientCounter)
				{
					cout << "Option out of range\nOperation aborted" << endl;
					cu::pauseConsole();
					return false;
				}

				// Converts option to client VAT
				vat = clientMap[vat];
				return true;
			}


			break;
		default:
			cout << "Wrong Input" << endl;
			cu::pauseConsole();
			break;
		}

		system("cls");
	} // End while (!back)
}

void viewPacksSoldMenu(Agency & agency)
{
	int op = -1;
	bool back = false;

	while (!back)
	{
		cout << "--------  View Packs sold to Clients  --------" << endl << endl;
		printMenu({ "Previous Menu", // 0
			"Packs sold to all Clients", // 1
			"Packs sold at least once", // 2
			"Packs sold to a specific Client", }); // 3

		if (!cu::readInt(op, "Select Option"))
			return;

		if (op >= 0 && op <= 3)
			system("cls");

		int vat;
		switch (op)
		{
		case 0:
			back = true;
			break;

		case 1:
			cout << "Packs sold to all Clients:" << endl;
			agency.printPacksSoldToEveryone();
			cout << endl;
			cu::pauseConsole();
			break;

		case 2:
			cout << "Packs sold at least once:" << endl;
			agency.printPacksSold();
			cout << endl;
			cu::pauseConsole();
			break;

		case 3:
			if (selectClientMenu(agency, vat, "Packs sold to a Client"))
			{
				system("cls");
				cout << "Packs sold to Client:" << endl << endl;
				agency.printPacksByClient(vat);
				cout << endl;
				cu::pauseConsole();
			}
			break;

		default:
			cout << "Wrong Input" << endl;
			cu::pauseConsole();
			break;
		}

		system("cls");

	} // End while (!back)

}

void viewPacks(const Agency & agency)
{
	int op = -1;
	bool back = false;

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
			"All sorted by Destination and Dates", // 8
			"Sold to a specific Client" }); // 9

		if (!cu::readInt(op, "Select Option"))
			return;

		if (op >= 0 && op <= 8)
			system("cls");

		string s;
		Date start, end;
		int vat;
		bool aborted = false;
		switch (op)
		{
		case 0:
			back = true;
			break;

		case 1:
		case 5:
			if (op == 1)
			{
				cout << "--------  Only avaiable Packs  --------" << endl;
				agency.printPacks(true);
			}
			else
			{
				cout << "--------  All Packs  --------" << endl;
				agency.printPacks(false);
			}

			cout << endl;
			cu::pauseConsole();
			break;

		case 2:
		case 6:
			if (op == 2)
				cout << "--------  Available Packs sorted by Destination  --------" << endl << endl;
			else
				cout << "--------  All Packs sorted by Destination  --------" << endl << endl;

			if (!cu::readStr(s, "Destination to search"))
			{
				cout << "Operation aborted" << endl;
				cu::pauseConsole();
				break;
			}

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
				cout << "--------  Available Packs sorted by Dates  --------" << endl << endl;
			else
				cout << "--------  All Packs sorted by Dates  --------" << endl << endl;

			cout << "Date Interval to sort by:" << endl;
			cout << "Starting Date:" << endl;
			if (!start.readUserInput())
			{
				cout << "Operation aborted" << endl;
				cu::pauseConsole();
				break;
			}

			end = Date(0, 0, 0); // To ensure it enters while Loop
			while (end < start && !aborted)
			{
				cout << "Ending Date:" << endl;
				if (!end.readUserInput())
					aborted = true;
				else
				{
					if (end < start)
						cout << "Ending Date must not be before Starting Date" << endl;
				}
			}

			if (aborted)
			{
				cout << "Operation aborted" << endl;
				cu::pauseConsole();
				break;
			}

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
				cout << "--------  Available Packs sorted by Destination and Dates  --------" << endl << endl;
			else
				cout << "--------  All Packs sorted by Destination and Dates  --------" << endl << endl;

			if (!cu::readStr(s, "Destination to search"))
			{
				cout << "Operation aborted" << endl;
				cu::pauseConsole();
				break;
			}

			cout << "Date Interval to sort by:" << endl;
			cout << "Starting Date:" << endl;
			if (!start.readUserInput())
			{
				cout << "Operation aborted" << endl;
				break;
			}

			end = Date(0, 0, 0); // To ensure it enters while Loop
			while (end < start && !aborted)
			{
				cout << "Ending Date:" << endl;
				if (!end.readUserInput())
					aborted = true;
				else
				{
					if (end < start)
						cout << "Ending Date must not be before Starting Date" << endl;
				}
			}

			if (aborted)
			{
				cout << "Operation aborted" << endl;
				cu::pauseConsole();
				break;
			}

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
			"Specific Client" }); // 2

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
			cout << "--------  All Clients  --------" << endl;
			agency.printClients();
			cout << endl;
			cu::pauseConsole();
			break;
		case 2:
			if (selectClientMenu(agency, vat, "Client to View"))
			{
				agency.printClientByVAT(vat);
				cout << endl;
				cu::pauseConsole();
			}

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
			"Remove Client" }); // 3

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
			cout << "--------  Add new Client  --------" << endl;
			if (agency.readNewClientUserInput())
				cout << "Client added successfully" << endl;
			else
				cout << "Operation aborted" << endl;
			cu::pauseConsole();
			break;

		case 2: // Change Client
			if (selectClientMenu(agency, vat, "Remove Client"))
			{
				cout << endl;
				if (agency.changeClient(vat))
					cout << "Client changed successfully" << endl;
				else
					cout << "Operation aborted" << endl;
				cu::pauseConsole();
			}
			break;

		case 3: // Remove Client
			if (selectClientMenu(agency, vat, "Remove Client"))
			{
				agency.removeClientByVat(vat);
				cu::pauseConsole();
			}
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
			"Make Pack Unavailable" }); // 3

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
			cout << "--------  Add new Pack  --------" << endl;
			if (agency.readNewPackUserInput())
				cout << "Pack added successfully" << endl;
			else
				cout << "Operation aborted" << endl;
			cu::pauseConsole();
			break;

		case 2: // Change Pack
			if (selectPackMenu(agency, id, "Change Pack", false))
			{
				cout << endl;
				agency.changePack(id);
				cu::pauseConsole();
			}
			break;

		case 3: // Make Pack Unavaiable
			if (selectPackMenu(agency, id, "Make Pack Unavailable", true))
			{
				agency.makePackUnavailableById(id);
				cu::pauseConsole();
			}
			break;

		default:
			cout << "Wrong Input" << endl;
			cu::pauseConsole();
			break;
		}

		system("cls");
	} // End while (!back)
}

void purchasePackMenu(Agency & agency)
{
	int id, vat, tickets;
	char op;
	if (selectClientMenu(agency, vat, "Client responsible for the Purchase"))
	{
		system("cls");
		if (selectPackMenu(agency, id, "Pack to buy", true))
		{
			system("cls");
			if (cu::readInt(tickets, "Number of tickets to buy"))
			{
				// Only enters if all operations were successful
				system("cls");
				cout << "Purchase details:" << endl << endl;
				cout << "CLIENT:" << endl;
				if (!agency.printClientByVAT(vat))
				{
					cout << "Operation aborted";
					cu::pauseConsole();
					return;
				}
				cout << endl << "PACK:" << endl;
				if (!agency.printPackById(id))
				{
					cout << "Operation aborted";
					cu::pauseConsole();
					return;
				}
				cout << "TICKETS: " << tickets << endl << endl;

				if (!cu::readConfirmation(op, "Do you want to proceed with the Purchase"))
				{
					cout << "Operation aborted" << endl;
					cu::pauseConsole();
					return;
				}
				if (op == 'y')
				{
					agency.purchasePack(vat, id, tickets);
					cu::pauseConsole();
					return;
				}
			}
		}
	}

	// Ends up here if any of the operations were unsuccessful
	cout << "Operation aborted" << endl;
	cu::pauseConsole();
	return;
}

void statisticsMenu(Agency & agency)
{
	int op = -1, vat;
	bool back = false;

	while (!back)
	{
		cout << "--------  Agency Statistics  --------" << endl << endl;
		printMenu({ "Previous Menu", // 0
			"Show statistics", // 1
			"Most visited Destinations", // 2
			"Client Recommendations" }); // 3

		if (!cu::readInt(op, "Select Option"))
			return;

		if (op >= 0 && op <= 3)
			system("cls");


		int n;
		switch (op)
		{
		case 0:
			back = true;
			break;

		case 1: // Show Agency Statistics
			cout << "--------  Statistics  --------" << endl << endl;
			agency.printStatistics();
			cout << endl;
			cu::pauseConsole();
			break;

		case 2: // Most Visited Destinations
			cout << "--------  Most visited Destinations  --------" << endl << endl;
			if (!cu::readInt(n, "How many destinations do you wish to see"))
			{
				cout << "Operation aborted" << endl;
				cu::pauseConsole();
				break;
			}

			cout << endl;
			agency.printMostVisitedDestinations(n);
			cu::pauseConsole();
			break;

		case 3: // Client Pack Recommendations
			cout << "--------  Client Recommendations  --------" << endl << endl;
			if (!cu::readInt(n, "How many destinations to focus the search"))
			{
				cout << "Operation aborted" << endl;
				cu::pauseConsole();
				break;
			}

			cout << endl;
			agency.printClientRecommendations(n);
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

bool loadAgencyDataMenu(Agency & agency)
{
	char op;
	string s;

	if (!cu::readConfirmation(op, "Do you wish to load Agency Data"))
	{
		cout << "Operation aborted by user" << endl;
		return false;
	}
	if (op == 'n')
	{
		cout << "If the Agency has incorrect Data loaded, it may lead to unexpected behaviour" << endl
			<< "Proceed at own risk" << endl;
		cu::pauseConsole();
		return true;
	}

	if (!cu::readStr(s, "Agency file name"))
	{
		cout << "Operation aborted by user" << endl;
		return false;
	}

	return agency.loadData(s, true);
}

// Public Function

void agencyMainMenu(Agency & agency)
{
	bool stopProgram = false;
	int op = -1;

	if (!loadAgencyDataMenu(agency))
	{
		cout << "Something went wrong, exiting..." << endl;
		cu::pauseConsole();
		return;
	}

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
			"View Packs", // 5
			"View Packs Sold", // 6
			"Purchase a Pack", // 7
			"Statistics" }); // 8

		cout << "At any point enter Ctrl+Z to cancel or to go back to a previous menu\n" << endl;

		if (!cu::readInt(op, "Insert option"))
		{
			confirmSaveData(agency);
			return;
		}

		if (op >= 0 && op <= 9)
			system("cls");

		int n;
		// SWITCH STATEMENT HERE
		switch (op)
		{
		case 1: // Save Changes
			confirmSaveData(agency);
			break;

		case 2: // Manage Clients
			manageClients(agency);
			break;

		case 3: // Manage Packs
			managePacks(agency);
			break;

		case 4: // View Clients
			viewClients(agency);
			break;

		case 5: // View Packs
			viewPacks(agency);
			break;

		case 6: // View Packs Sold
			viewPacksSoldMenu(agency);
			break;

		case 7: // Purchase Pack
			purchasePackMenu(agency);
			break;

		case 8: // Statistics
			statisticsMenu(agency);
			break;

		case 0: // Finish Program
			stopProgram = true;
			break;

		default:
			cout << "Invalid Input" << endl;
			cu::pauseConsole();
			break;
		}

	} // End while(!stopProgram)

	system("cls");
	confirmSaveData(agency);
}

