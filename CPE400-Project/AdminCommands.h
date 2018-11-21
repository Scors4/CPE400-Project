#pragma once
#ifndef _ADMIN_COMMANDS_H
#define _ADMIN_COMMANDS_H
#include <string>

using namespace std;

class AdminCommands
{
public:
	
	static bool parseCommand(string st);


private:

	static string* explodeString(string in, int* count);
};

#endif