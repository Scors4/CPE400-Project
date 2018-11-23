#include "NodeManager.h"
#include "AdminCommands.h"
#include <iostream>

using namespace std;

int main()
{
	NodeManager* nm = new NodeManager();

	nm->init(5, false);
	
	string st;
	do {
		getline(cin, st);
	} while (AdminCommands::parseCommand(st, nm));


	delete nm;
	nm = nullptr;
}