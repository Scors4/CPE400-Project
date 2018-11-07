#include "NodeManager.h"
#include "Node.h"
#include <iostream>

using namespace std;


NodeManager::NodeManager()
{
	
}

NodeManager::~NodeManager()
{

}

void NodeManager::init(int numNodes, bool random)
{
	this->numNodes = numNodes;
	Node* tempn = new Node[numNodes];
	for (int i = 0; i < numNodes; i++)
	{
		tempn[i].setID(NEXT_ID++);
	}
	nodes = tempn;
	tempn = nullptr;
}

void NodeManager::run()
{
	for (int i = 0; i < numNodes; i++)
	{
		cout << "Node Number " << (int)nodes[i].getID() << endl;
	}
}