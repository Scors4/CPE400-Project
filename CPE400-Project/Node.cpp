#include "Node.h"
#include <iostream>
#include "ScThreadManager.h"

#define _SLEEP_TIME 100ms

using namespace std;

Node::Node()
{
	ID = 0;
}

Node::Node(char ID)
{
	this->ID = ID;
}

Node::~Node()
{
	if (routeTableSize)
	{
		delete[] routeTable;
		routeTable = nullptr;

		delete[] routeHash;
		routeHash = nullptr;
	}
}

//Terminates the thread by ending the runnable loop in thread_run.
void Node::terminate()
{
	running = false;
}

//Function the thread can use to start the node it is assigned.  Passes in this node from a long chain.
void thread_access(Node* n)
{
	n->thread_run(n);
}

//Creates this node's thread and detaches from the main process.
void Node::run(Node* n)
{
	running = true;
	thread t(thread_access, n);
	t.detach();
}

//Thread runnable.  Holds the actual logic of the node.  That's the plan, anyway.
void Node::thread_run(Node* n)
{
	do
	{
		if (!active) { std::this_thread::sleep_for(_SLEEP_TIME); }
		if (!running) { break; }
		if (packets_received)
		{
			cout << "Node " << ID << " packet count: " << packets_received << endl;
			advance_Buffer();
		}
		std::this_thread::sleep_for(_SLEEP_TIME);
	} while (running);
}

//Prints the node data, such as ID, its Neighbors and the Routing table and hash.
void Node::printData()
{
	std::cout << "Node ID: " << (int)ID << std::endl;
	std::cout << "Current status: " << (active ? "Active." : "Not Active.") << endl;
	std::cout << "Neighbors: ";
	for (int i = 0; i < 4; i++)
	{
		std::cout << (int)Neighbors[i];
		if (i < 3)
			std::cout << ",";
	}
	std::cout << std::endl;
	cout << "Number of known routes: " << routeTableSize << endl;
	for (int i = 0; i < routeTableSize; i++)
	{
		std::cout << "Route to " << (int)routeTable[i] << " is via " << (int)routeHash[i] << std::endl;
	}
	std::cout << std::endl;
}

//Sets the route table size.  For now, it's the size of the network.
void Node::setRouteTableSize(int size)
{
	routeTableSize = size;
	int * tempTable = new int[size];
	int * tempHash = new int[size];
	for (int i = 0; i < size; i++)
	{
		tempTable[i] = i + 1;
		tempHash[i] = 0;
	}

	routeTable = tempTable;
	routeHash = tempHash;

	tempTable = nullptr;
	tempHash = nullptr;
}

//Gets this node's ID, if necessary.
char Node::getID()
{
	return ID;
}

//Sets this node's ID.
bool Node::setID(char ID)
{
	if (this->ID != 0)
		return false;

	this->ID = ID;
	return true;
}

//Adds the ID in the neighbor list if there is room. Returns true if the ID is already a neighbor
//or is set as a neighbor.  Returns false if no room for a new neighbor.
bool Node::addNeighbor(char ID)
{
	if (ID == this->ID || ID == 0)
		return false;

	int i = 0;
	for (i; i < 4; i++)
	{
		if (Neighbors[i] == 0)
			break;
		if (Neighbors[i] == ID)
			return true;
	}

	if (i == 4)
		return false;

	Neighbors[i] = ID;
	return true;
}

//Adds a packet to the buffer if there is room and the node is currently active.
//If the buffer is full, it sends a failed response packet.
//If the node is inactive, no response is sent.
void Node::addPacketToBuffer(Packet* p, Node* sender)
{
	if (!active) { return; }
	if (packets_received >= p_buffer_size)
	{
		Packet* r = new Packet();
		if(sender != nullptr)
			sender->addPacketToBuffer(r, this);

		return;
	}

	p_buffer[packets_received] = *p;
	packets_received++;
}

void Node::advance_Buffer()
{
	for (int i = 1; i < packets_received; i++ )
	{
		p_buffer[i - 1] = p_buffer[i];
	}
	packets_received--;
}

void Node::kill()
{
	active = false;
}

void Node::revive()
{
	active = true;
}