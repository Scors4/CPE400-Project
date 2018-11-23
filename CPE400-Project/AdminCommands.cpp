#include "AdminCommands.h"
#include <iostream>


bool AdminCommands::parseCommand(string st, NodeManager* nm)
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
		if (commands[i]._Equal("ping"))
		{
			i++;
			if (testID(commands[i], nm))
			{
				continue;
			}

			int node_id = stoi(commands[i]);
			nm->printNodeData(node_id);
		}
		else if (commands[i]._Equal("kill"))
		{
			i++;
			if (testID(commands[i], nm))
			{
				continue;
			}
			int node_id = stoi(commands[i]);
			nm->getNode(node_id)->kill();
		}
		else if (commands[i]._Equal("revive"))
		{
			i++;
			if (testID(commands[i], nm))
			{
				continue;
			}

			int node_id = stoi(commands[i]);
			nm->getNode(node_id)->revive();
		}
		else if (commands[i]._Equal("send"))
		{
			if (testID(commands[i + 1], nm) || testID(commands[i + 2], nm))
			{
				i += 2;
				continue;
			}
		}
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

bool AdminCommands::testID(string in, NodeManager* nm)
{
	int node_id = 0;
	try {
		node_id = stoi(in);
	}
	catch (exception e) {

		cout << "ID unrecognized: " << in << endl;
		return true;
	}

	if (node_id > nm->number_of_nodes)
	{
		cout << "Unable to process request.  Node ID given (" << node_id << ") does not exist." << endl;
		return true;
	}

	return false;
}