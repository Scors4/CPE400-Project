#include "NodeManager.h"
#include "AdminCommands.h"
#include <iostream>

using namespace std;

int main()
{
	NodeManager* nm = new NodeManager();

	nm->init(50, true);
	cout << "Nodes now initialized.  Awaiting commands: ";
	
	string st;
	do {
		getline(cin, st);
	} while (AdminCommands::parseCommand(st, nm));


	delete nm;
	nm = nullptr;
}