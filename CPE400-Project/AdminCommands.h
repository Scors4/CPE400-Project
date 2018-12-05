#pragma once
#ifndef _ADMIN_COMMANDS_H
#define _ADMIN_COMMANDS_H
#include <string>
#include "NodeManager.h"

using namespace std;

class AdminCommands
{
public:
	
	static bool parseCommand(string st, NodeManager* nm);


private:

	static string* explodeString(string in, int* count);
	static bool testID(string in, NodeManager* nm);
	static void listNodes(string in, NodeManager* nm);
};

#endif