#include "AdminCommands.h"
#include <iostream>


bool AdminCommands::parseCommand(string st)
{
	if (st._Equal("exit"))
	{
		return false;
	}
	if (st.empty()) { return true; }

	int command_count = 0;
	string* commands = explodeString(st, &command_count);

	for (int i = 0; i < command_count; i++)
	{
		
	}


	delete[] commands;
	commands = nullptr;
	return true;
}

string* AdminCommands::explodeString(string in, int* container)
{
	int segments = 1;
	for (int i = 0; i < in.length(); i++)
	{
		if (in[i] == ' ')
		{
			segments++;
		}
	}

	string * temp = new string[segments];
	int curSegment = 0;

	for (int i = 0; i < in.length(); i++)
	{
		if (in[i] != ' ')
		{
			temp[curSegment].push_back(in[i]);
		}
		else
		{
			curSegment++;
		}
	}

	*container = segments;
	return temp;
}