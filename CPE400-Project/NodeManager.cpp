#include "NodeManager.h"
#include "Node.h"
#include <iostream>
#include <thread>

using namespace std;


NodeManager::NodeManager()
{
	
}

NodeManager::~NodeManager()
{
	delete nodes;
	nodes = nullptr;
}

void NodeManager::init(int numNodes, bool random)
{
	this->numNodes = numNodes;
	Node* tempn = new Node[numNodes];
	for (int i = 0; i < numNodes; i++)
	{
		tempn[i].setID(NEXT_ID++);
		tempn[i].setRouteTableSize(numNodes);
	}
	nodes = tempn;
	tempn = nullptr;
}

void NodeManager::run()
{
	for (int i = 0; i < numNodes; i++)
	{
		nodes[i].printData();
	}
}