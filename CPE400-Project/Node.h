#pragma once
#ifndef _NODE_H
#define _NODE_H
#include <thread>
#include "Packet.h"

class Node
{
public:

	Node();
	Node(char ID);
	~Node();

	void run(Node* n);
	void terminate();
	void kill();
	void revive();

	void printData();
	void setRouteTableSize(int size);

	char getID();
	bool setID(char ID);
	bool addNeighbor(char ID);

	void addPacketToBuffer(Packet* p, Node* sender);

	void thread_run(Node* n);

private:

	char ID = 0;
	char Neighbors[4] = { 0,0,0,0 };
	int routeTableSize = 0;
	int* routeTable = nullptr; //List of nodes in the map.  Can be pretty lengthy, but it's still relatively small.
	int* routeHash = nullptr; //List of nodes that go to the table's nodes, corresponding with the id - 1 in this iteration.

	int p_buffer_size = 3; // Default buffer size of 3.
	Packet * p_buffer = new Packet[p_buffer_size]; // Buffer of received packets.
	int packets_received = 0; //Number of packets currently waiting in the buffer.\

	void advance_Buffer();

	bool running = true;
	bool active = true;

	std::thread personal_thread;
};

#endif