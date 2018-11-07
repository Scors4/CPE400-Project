#pragma once

class Node
{
public:

	Node();
	Node(char ID);
	~Node();

	void printData();
	void setRouteTableSize(int size);

	char getID();
	bool setID(char ID);
	bool addNeighbor(char ID);

private:

	char ID;
	char Neighbors[4] = { 0,0,0,0 };
	int routeTableSize = 0;
	int* routeTable;
	int* routeHash;
	//Packet* packet;
};