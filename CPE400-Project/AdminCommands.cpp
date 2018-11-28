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

			string data;
			for (int j = i + 3; j < command_count; j++)
			{
				data.append(commands[j]);
				if (j < command_count - 1)
				{
					data.append(" ");
				}
			}
			
			char* c = new char[data.length() + 1];
			int c_i = 0;
			for (c_i = 0; c_i < data.length(); c_i++)
			{
				c[c_i] = data[c_i];
			}
			c[c_i] = '\0';

			int to = stoi(commands[i + 2]);
			int from = stoi(commands[i + 1]);

			Packet p(to, from, data.length(), 0x00, c);
			nm->getNode(from)->addPacketToBuffer(p);

			delete c;
			c = nullptr;

		}
		else if (commands[i]._Equal("verbose"))
		{
			i++;
			if (testID(commands[i], nm))
			{
				continue;
			}
			int node_id = stoi(commands[i]);
			nm->getNode(node_id)->toggleVerbose();
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

//Ensures the given ID is readable and valid (within the number of nodes the Manager
//has available.)
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

	if (node_id > nm->number_of_nodes || node_id <= 0)
	{
		cout << "Unable to process request.  Node ID given (" << node_id << ") does not exist." << endl;
		return true;
	}

	return false;
}