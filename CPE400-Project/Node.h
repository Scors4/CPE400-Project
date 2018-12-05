#pragma once
#ifndef _NODE_H
#define _NODE_H

#define NEIGHBOR_COUNT 4
#define RREQ_COUNT 4
#include <thread>
#include "Packet.h"

class Node
{
public:

	Node();
	Node(char ID);
	~Node();

	bool operator == (int in);

	void run(Node* n);
	void terminate();
	void kill();
	void revive();

	void printData();
	void setRouteTableSize(int size);

	char getID();
	bool setID(char ID);
	bool getActive();
	void toggleVerbose();
	bool addNeighbor(char ID);
	void randomizeNeighbors(int max_node_id);

	bool addPacketToBuffer(Packet p, Node* sender);
	void addPacketToBuffer(Packet p);

	bool canAcceptPackets();

	void thread_run(Node* n);

private:

	char ID = 0;
	char Neighbors[NEIGHBOR_COUNT] = { 0,0,0,0 };
	int routeTableSize = 0;
	char* routeTable = nullptr; //List of nodes in the map.  Can be pretty lengthy, but it's still relatively small.
	char* routeHash = nullptr; //List of nodes that go to the table's nodes, corresponding with the id - 1 in this iteration.

	int p_buffer_size = 3; // Default buffer size of 3.
	Packet * p_buffer = new Packet[p_buffer_size]; // Buffer of received packets.
	int packets_received = 0; //Number of packets currently waiting in the buffer.
	bool* rreq = new bool[p_buffer_size];
	char* delay = new char[p_buffer_size];

	char* rreq_invalid = new char[RREQ_COUNT];

	//Build an ordered-pair check for Control packets to avoid dropped packets building new node routes.

	bool addRandomNeighbor(char id);
	bool hasRoute(char ID);
	char getNextInChain(char ID);
	bool isNeighbor(char ID);
	bool cleanRouteHash(char ID);

	void advance_Buffer(bool deleting);
	void shift_Buffer();
	bool send_rreq_p(char target);
	void forward_rreq_p();
	bool send_rack_p(char target);
	void send_rack_p(char target, char from);
	void forward_rack_p();
	void send_fail_p(char ID);

	bool running = true;
	bool active = true;
	bool verbose = false;

	bool isRREQvalid(char ID);
	void addInvalidRREQ(char ID);
	void clearInvalidRREQs();

	void addPacketToBuffer(Packet p, int delay);

	std::thread personal_thread;
};

#endif