#include "Node.h"
#include <iostream>

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

void Node::printData()
{
	cout << "Node ID: " << (int)ID << endl;
	cout << "Neighbors: ";
	for (int i = 0; i < 4; i++)
	{
		cout << (int)Neighbors[i];
		if (i < 3)
			cout << ",";
	}
	cout << endl;
	for (int i = 0; i < routeTableSize; i++)
	{
		cout << "Route to " << (int)routeTable[i] << " is via " << (int)routeHash[i] << endl;
	}
	cout << endl;
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