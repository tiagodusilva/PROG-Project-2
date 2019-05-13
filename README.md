# PROG-Trabalho-2
Console application with the purpose of managing a travel agency.
Written in c++ and using the required classes.

-> All given objectives were fulfilled;

-> There are some functionalities beyond what was requested:
	- Everytime a Pack or Client needs to be selected, a menu will prompt
		the user if they would like to insert id/Vat directly or select them
		from a list of all possible options. The list shown for Packs can contain
		all packs or only the ones that are available. If the first line of the
		Packs file is inconsistent with the Packs (Pack has an Id larger than 
		maxPackId), the program will detect it and cancel the reading operation.
	- The verification of the consistency of the data contained in the files is very
		thorough, crosschecking pack Ids and Client VATs with each other. As an
		example, if a client has a registered a pack that is missing from the packFile,
		the program will detect it and abort execution.
	- As our printPack methods automatically included a version where they showed all
		Packs or only available Packs, we included both in the final menu. As a result
		all the filters (destination and dates) can be applied to either all or only
		the available Packs.
	- As "generate and print sold to all clients" could have 2 different interpretations,
		both were included as an option: Viewing packs that every client bough; and all
		the Packs that were bought at least once by at least one of the Clients.

-> Some details about our implementation:
	- Most comments about what a method/function does is present in its header file, so
		in case what one of them does, the comments on the header file will probably be
		enough to clear any doubts.
	- At any point in the menu, by typing Ctrl+Z, it will always either cancel the
		current operation and go back to the most appropriate menu, or if already in
		a menu, it will bring the most appropriate menu before it. For example,
		if Ctrl+Z is inserted in the "View Packs" menu, it will bring you back to the
		"Main Menu". If you are purchasing a pack and insert Ctrl+Z, it will cancel the
		purchase operation.
	- As it was not specified in the project guidelines, whenever a Client hasn't bought
		a single pack, the value written in the client file for "Packs bought" is "-"
