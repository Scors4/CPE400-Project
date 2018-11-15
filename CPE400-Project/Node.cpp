#include "Node.h"
#include <iostream>
#include "ScThreadManager.h"

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

}

void Node::terminate()
{
	running = false;
}

void thread_access(Node* n)
{
	n->thread_run(n);
}

void Node::run(Node* n)
{
	running = true;
	thread t(thread_access, n);
	t.detach();
}

void Node::thread_run(Node* n)
{

}

void Node::printData()
{
	std::cout << "Node ID: " << (int)ID << std::endl;
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

char Node::getID()
{
	return ID;
}

bool Node::setID(char ID)
{
	if (this->ID != 0)
		return false;

	this->ID = ID;
	return true;
}

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