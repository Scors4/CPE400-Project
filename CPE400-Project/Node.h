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
	int* routeTable;
	int* routeHash;

	bool running;

	std::thread personal_thread;
};

#endif