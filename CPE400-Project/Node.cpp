#include "Node.h"
#include <iostream>
#include <random>
#include "ScThreadManager.h"

#define _SLEEP_TIME 1000ms

using namespace std;

Node::Node()
{
	ID = 0;
	std::this_thread::sleep_for(250ms);
	for (int i = 0; i < p_buffer_size; i++)
	{
		rreq[i] = 0;
		delay[i] = 0;
	}

	for (int i = 0; i < RREQ_COUNT; i++)
	{
		rreq_invalid[i] = 0;
	}
}

Node::Node(char ID)
{
	this->ID = ID;
	std::this_thread::sleep_for(250ms);
	for (int i = 0; i < p_buffer_size; i++)
	{
		rreq[i] = 0;
		delay[i] = 0;
	}
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

		delete[] delay;
		delay = nullptr;
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
					cout << "Acknowledgement flag: " << (p_buffer[0].getFlag(ACK_FLAG) ? "sed" : "unset") << "." << endl;
					cout << "Fail flag: " << (p_buffer[0].getFlag(FAIL_FLAG) ? "set" : "unset") << "." << endl;
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
				adv_del = true;

				if (p_buffer[0].getFlag(FAIL_FLAG))
				{
					if (verbose)
						cout << "Node " << (int)p_buffer[0].getTo() << " failure packet detected." << endl;
					
					if (cleanRouteHash(p_buffer[0].getTo()))
					{
						send_fail_p(p_buffer[0].getTo());
						clearInvalidRREQs();
					}
					
					adv_del = false;
				}
				else if (p_buffer[0].getFlag(ACK_FLAG))
				{
					char r_ID = p_buffer[0].getData()[0];
					if (verbose)
						cout << "Packet from leaf Node " << (int)r_ID << " detected." << endl;

					if (routeHash[r_ID - 1] == 0)
					{
						routeHash[r_ID - 1] = p_buffer[0].getFrom();
					}

					if (p_buffer[0].getTo() != ID)
					{
						if (verbose)
							cout << "Forwarding CTRL/ACK packet from Node " << (int)r_ID << "." << endl;
						forward_rack_p();
						adv_del = false;
					}
					else
					{
						if(verbose)
							cout << "Acquired CTRL/ACK packet from Node " << (int)r_ID << " via Node " << (int)p_buffer[0].getFrom() << "." << endl;
					}
				}
				else
				{
					char r_ID = p_buffer[0].getData()[0];
					if (verbose)
						cout << "Packet from root Node " << (int)r_ID << " detected." << endl;

					if (isRREQvalid(p_buffer[0].getTo()))
					{
						if (routeHash[r_ID - 1] == 0)
						{
							routeHash[r_ID - 1] = p_buffer[0].getFrom();
						}
						
						
						if (p_buffer[0].getTo() == ID)
						{
							if (verbose)
								cout << "Packet arrived at node " << (int)ID << "." << endl;

							send_rack_p(r_ID);
						}
						else if (hasRoute(p_buffer[0].getTo()))
						{
							send_rack_p(r_ID, p_buffer[0].getTo());
						}
						else
						{
							forward_rreq_p();
						}

						addInvalidRREQ(p_buffer[0].getTo());
					}

					
				}
			}
			else
			{
				if (ID == p_buffer[0].getTo())
				{
					cout << "Packet received by Node " << (int)ID << ": " << endl;

					if (p_buffer[0].getFlag(FAIL_FLAG))
						cout << "Undeliverable to Node " << (int)p_buffer[0].getFrom() << "." << endl;

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
						delay[0] = (ScThreadManager::getThreadCount() * 3) + 1;
						send_rreq_p(p_buffer[0].getTo());
					}
					else if (delay[0])
					{
						if(verbose)
							cout << "Packet to Node " << (int)p_buffer[0].getTo() << " on delay hold: " << (int)delay[0] << " ticks remaining." << endl;

						delay[0]--;
					}

					if (delay[0])
					{
						shift_Buffer();
						adv = false;
					}
					else
					{
						cout << "Packet to Node " << (int)p_buffer[0].getTo() << " could not be delivered. TIMEOUT" << endl;
					}
				}
				else
				{
					char pass_ID = getNextInChain(p_buffer[0].getTo());
					if (verbose)
						cout << "Sending packet to Node " << (int)p_buffer[0].getTo() << " via node " << (int)pass_ID << "." << endl;
					if (!ScThreadManager::getNode(pass_ID)->addPacketToBuffer(p_buffer[0], this))
					{
						if (verbose)
							cout << "Sending failed.  Node " << (int)pass_ID << " is offline." << endl;

						send_fail_p(pass_ID);
						adv = false;

						if (pass_ID == p_buffer[0].getTo())
						{
							char temp = p_buffer[0].getTo();
							p_buffer->setFail();
							p_buffer[0].setTo(p_buffer[0].getFrom());
							p_buffer[0].setFrom(temp);
						}
						else
						{
							cleanRouteHash(pass_ID);
						}
					}

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

	rreq[0] = false;
	delay[0] = 0;

	for (int i = 1; i < packets_received; i++ )
	{
		p_buffer[i - 1] = p_buffer[i];
		rreq[i - 1] = rreq[i];
		rreq[i] = false;

		delay[i - 1] = delay[i];
		delay[i] = 0;
	}
	packets_received--;
}

void Node::shift_Buffer()
{
	if (packets_received > 1)
	{
		Packet p = p_buffer[0];
		bool r = rreq[0];
		char c = delay[0];

		advance_Buffer(true);

		p_buffer[packets_received] = p;
		rreq[packets_received] = r;
		delay[packets_received] = c;
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
		if (verbose)
			cout << "Sending route request packet for Node " << (int)target << "." << endl;

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

	Packet p(p_buffer[0].getTo(), this->ID, i + 1, CTRL_FLAG, c);
	for (int j = 0; j < NEIGHBOR_COUNT; j++)
	{
		if (Neighbors[j] != 0 && Neighbors[j] != p_buffer[0].getFrom())
		{
			if (!ScThreadManager::getNode(Neighbors[j])->addPacketToBuffer(p, this) && p.getTo() == Neighbors[j])
			{
				send_fail_p(p.getTo());
			}
		}
	}
	delete c;
	c = nullptr;
}

bool Node::send_rack_p(char target)
{
	char * c = new char[2];
	c[0] = ID;
	c[1] = '\0';

	char pass_ID = getNextInChain(target);

	Packet p(target, ID, 1, CTRL_FLAG | ACK_FLAG, c);

	ScThreadManager::getNode(pass_ID)->addPacketToBuffer(p, this);


	return true;
}

void Node::send_rack_p(char target, char from)
{
	char * c = new char[2];
	c[0] = from;
	c[1] = '\0';

	char pass_ID = getNextInChain(target);
	
	Packet p(target, ID, 1, CTRL_FLAG | ACK_FLAG, c);

	ScThreadManager::getNode(pass_ID)->addPacketToBuffer(p, this);
}

void Node::forward_rack_p()
{
	char pass_ID = getNextInChain(p_buffer[0].getTo());
	if(verbose)
		cout << "Forward to node " << (int)pass_ID << " ready." << endl;

	p_buffer[0].setFrom(ID);

	ScThreadManager::getNode(pass_ID)->addPacketToBuffer(p_buffer[0], this);
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

void Node::send_fail_p(char ID)
{
	Packet p(ID, this->ID, 0, CTRL_FLAG | FAIL_FLAG, nullptr);
	for (int i = 0; i < NEIGHBOR_COUNT; i++)
	{
		if(Neighbors[i] != ID)
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

bool Node::cleanRouteHash(char ID)
{
	if (verbose)
		cout << "Removing Node " << (int)ID << " from Node " << (int)this->ID << " route table." << endl;

	bool found = false;
	for (int i = 0; i < routeTableSize; i++)
	{
		if (routeHash[i] == ID)
		{
			routeHash[i] = 0;
			found = true;
		}
	}

	return found;
}

bool Node::isRREQvalid(char ID)
{
	for (int i = 0; i < RREQ_COUNT; i++)
	{
		if (rreq_invalid[i] == ID)
		{
			return false;
		}
	}

	return true;
}

void Node::addInvalidRREQ(char ID)
{
	for (int i = 0; i < RREQ_COUNT; i++)
	{
		if (rreq_invalid[i] == ID)
			return;

		if (rreq_invalid[i] == 0)
		{
			rreq_invalid[i] = ID;
			return;
		}
	}

	for (int i = 1; i < RREQ_COUNT; i++)
	{
		rreq_invalid[i - 1] = rreq_invalid[i];
	}

	rreq_invalid[RREQ_COUNT - 1] = ID;
}

void Node::clearInvalidRREQs()
{
	for (int i = 0; i < RREQ_COUNT; i++)
	{
		rreq_invalid[i] = 0;
	}
}