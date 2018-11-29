#include "Node.h"
#include <iostream>
#include <random>
#include "ScThreadManager.h"

#define _SLEEP_TIME 2000ms

using namespace std;

Node::Node()
{
	ID = 0;
	std::this_thread::sleep_for(250ms);
	for (int i = 0; i < p_buffer_size; i++)
		rreq[i] = 0;
}

Node::Node(char ID)
{
	this->ID = ID;
	std::this_thread::sleep_for(250ms);
	for (int i = 0; i < p_buffer_size; i++)
		rreq[i] = 0;
}

Node::~Node()
{
	if (routeTableSize)
	{
		delete[] routeTable;
		routeTable = nullptr;

		delete[] routeHash;
		routeHash = nullptr;

		delete[] rreq;
		rreq = nullptr;
	}
}

bool Node::operator== (int in)
{
	return ID == in;
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
		bool adv_del = false;
		bool adv = true;
		if (!active) { std::this_thread::sleep_for(_SLEEP_TIME); }
		if (packets_received)
		{
			if (verbose)
			{
				cout << "Node " << (int)ID << " has " << packets_received << " " << ((packets_received > 1) ? "packets." : "packet.") << endl;
				if (!rreq[0])
				{
					cout << "Current packet data: " << endl << endl;
					cout << "Control flag: " << (p_buffer[0].getFlag(CTRL_FLAG) ? "set" : "unset") << "." << endl;
					p_buffer[0].printPacket();
					cout << endl;
				}
				else
				{
					cout << "Waiting on route request data." << endl;
				}
			}

			if (p_buffer[0].getFlag(CTRL_FLAG))
			{

			}
			else
			{
				if (ID == p_buffer[0].getTo())
				{
					cout << "Packet received by Node " << (int)ID << ": " << endl;
					cout << p_buffer[0].getData() << endl;
					adv_del = true;
				}
				else if (!hasRoute(p_buffer[0].getTo()))
				{
					if (!rreq[0])
					{
						if (verbose)
							cout << "Route to node " << (int)p_buffer[0].getTo() << " from Node " << (int)ID << " is unavailable." << endl;
						
						rreq[0] = true;
						send_rreq_p(p_buffer[0].getTo());
					}

					shift_Buffer();
					adv = false;
				}
				else
				{
					char pass_ID = getNextInChain(p_buffer[0].getTo());
					if (verbose)
						cout << "Sending packet to Node " << (int)p_buffer[0].getTo() << " via node " << (int)pass_ID << "." << endl;
					ScThreadManager::getNode(pass_ID)->addPacketToBuffer(p_buffer[0], this);

				}
			}

			if(adv)
				advance_Buffer(adv_del);
		}
		std::this_thread::sleep_for(_SLEEP_TIME);
	} while (running);

	if (ScThreadManager::active)
		cout << "Node " << (int)ID << " has crashed!" << endl;
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
	char * tempTable = new char[size];
	char * tempHash = new char[size];
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

void Node::randomizeNeighbors(int max_node_id)
{
	random_device rd;
	mt19937 rng(rd());
	uniform_int_distribution<int> nodes(1, max_node_id);

	int attempts = 0;
	for (int i = 0; i < NEIGHBOR_COUNT; i++)
	{
		int p_node = nodes(rng);
		if (!addRandomNeighbor(p_node))
		{
			i--;
			attempts++;
		}

		if (attempts >= max_node_id * 3)
		{
			break;
		}
	}
}

//Adds the ID in the neighbor list if there is room. Returns true if the ID is already a neighbor
//or is set as a neighbor.  Returns false if no room for a new neighbor.
bool Node::addNeighbor(char ID)
{
	if (ID == this->ID || ID == 0)
		return false;

	int i = 0;
	for (i; i < NEIGHBOR_COUNT; i++)
	{
		if (Neighbors[i] == 0)
			break;
		if (Neighbors[i] == ID)
			return true;
	}

	if (i == 4)
		return false;

	Neighbors[i] = ID;
	if (!ScThreadManager::getNode(ID)->addNeighbor(this->ID))
	{
		Neighbors[i] = 0;
		return false;
	}
	return true;
}

//Same as add neighbor, only returns false on all failures.
bool Node::addRandomNeighbor(char ID)
{
	if (ID == this->ID || ID == 0)
		return false;

	int i = 0;
	for (i; i < 4; i++)
	{
		if (Neighbors[i] == 0)
			break;
		if (Neighbors[i] == ID)
			return false;
	}

	if (i == 4)
		return true;

	Neighbors[i] = ID;
	if (!ScThreadManager::getNode(ID)->addNeighbor(this->ID))
	{
		Neighbors[i] = 0;
		return false;
	}
	routeHash[ID - 1] = ID;
	return true;
}

//Adds a packet to the buffer if there is room and the node is currently active.
//If the buffer is full, it sends a failed response packet.
//If the node is inactive, a false response is sent. (no power)
bool Node::addPacketToBuffer(Packet p, Node* sender)
{
	if (!active) { return false; }
	if (packets_received >= p_buffer_size)
	{
		p.setFail();

		if (sender != nullptr && sender->canAcceptPackets())
			sender->addPacketToBuffer(p, this);

		return true;
	}

	p_buffer[packets_received] = p;
	packets_received++;
	return true;
}

void Node::addPacketToBuffer(Packet p)
{
	if (!active)
	{
		cout << "Node " << (int)ID << " is offline.  Cannot accept packets." << endl;
		return;
	}
	
	if (packets_received >= p_buffer_size)
	{
		cout << "Node " << (int)ID << " packet buffer full.  Cannot accept additional packets." << endl;
		return;
	}

	p_buffer[packets_received] = p;
	packets_received++;
}

bool Node::canAcceptPackets()
{
	return packets_received < p_buffer_size;
}

//Moves all packets in buffer forward by one index
//Also deletes the c string in the packet if this is its last stop.
void Node::advance_Buffer(bool deleting)
{
	if (deleting)
		p_buffer[0].delete_data();

	for (int i = 1; i < packets_received; i++ )
	{
		p_buffer[i - 1] = p_buffer[i];
		rreq[i - 1] = rreq[i];
		rreq[i] = false;

	}
	packets_received--;
}

void Node::shift_Buffer()
{
	if (packets_received > 1)
	{
		Packet p = p_buffer[0];
		bool r = rreq[0];

		advance_Buffer(true);

		p_buffer[packets_received] = p;
		rreq[packets_received] = r;
		packets_received++;
	}
}

bool Node::send_rreq_p(char target)
{
	bool short_line = false;
	for (int i = 0; i < NEIGHBOR_COUNT; i++)
	{
		routeHash[Neighbors[i] - 1] = Neighbors[i];
		if (Neighbors[i] == target)
			short_line = true;
	}

	if(short_line)
		return short_line;
	else
	{
		char * c = new char[2];
		c[0] = this->ID;
		c[1] = '\0';
		Packet p(target, ID, 1, CTRL_FLAG, c);
		for (int i = 0; i < NEIGHBOR_COUNT; i++)
		{
			ScThreadManager::getNode(Neighbors[i])->addPacketToBuffer(p, this);
		}

		delete c;
		c = nullptr;
	}

	return false;
}

void Node::forward_rreq_p()
{
	char* c = new char[p_buffer[0].getDataSize() + 2];
	int i = 0;
	for (i = 0; i < p_buffer[0].getDataSize(); i++)
	{
		c[i] = p_buffer[0].getData()[i];
	}
	c[i] = this->ID;
	c[i + 1] = '\0';

	Packet p(p_buffer[0].getTo(), this->ID, i, CTRL_FLAG, c);
	for (int j = 0; j < NEIGHBOR_COUNT; j++)
	{
		if (Neighbors[j] != 0 && Neighbors[j] != p_buffer[0].getFrom())
		{
			ScThreadManager::getNode(Neighbors[j])->addPacketToBuffer(p, this);
		}

	}
	delete c;
	c = nullptr;
}

//Disables this node
void Node::kill()
{
	active = false;
}

//Re-enables this node
void Node::revive()
{
	active = true;
}

void Node::toggleVerbose()
{
	verbose = !verbose;
	cout << "Node " << (int)ID << " verbose: " << (verbose ? "Enabled" : "Disabled") << endl;
}

bool Node::hasRoute(char ID)
{
	return (routeHash[ID - 1] != 0);
}

char Node::getNextInChain(char ID)
{
	return routeHash[ID-1];
}

void Node::send_fail_packets(char ID)
{
	Packet p(ID, this->ID, 0, CTRL_FLAG | FAIL_FLAG, nullptr);
	for (int i = 0; i < NEIGHBOR_COUNT; i++)
	{
		ScThreadManager::getNode(Neighbors[i])->addPacketToBuffer(p, this);
	}
}

bool Node::isNeighbor(char ID)
{
	for (int i = 0; i < NEIGHBOR_COUNT; i++)
	{
		if (ID == Neighbors[i])
			return true;
	}

	return false;
}