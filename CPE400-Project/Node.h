#pragma once
#ifndef _NODE_H
#define _NODE_H
#include <thread>

class Node
{
public:

	Node();
	Node(char ID);
	~Node();

	void run(Node* n);
	void terminate();

	void printData();
	void setRouteTableSize(int size);

	char getID();
	bool setID(char ID);
	bool addNeighbor(char ID);

	void thread_run(Node* n);

private:

	char ID;
	char Neighbors[4] = { 0,0,0,0 };
	int routeTableSize = 0;
	int* routeTable; //List of nodes in the map.  Can be pretty lengthy, but it's still relatively small.
	int* routeHash; //List of nodes that go to the table's nodes, corresponding with the id - 1.

	bool running;

	std::thread personal_thread;
};

#endif