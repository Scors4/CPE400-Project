#include "NodeManager.h"
#include "Node.h"
#include "ScThreadManager.h"
#include <iostream>
#include <thread>

using namespace std;


NodeManager::NodeManager() 
{
	nodes = nullptr;
	number_of_nodes = 0;
}

NodeManager::~NodeManager()
{
	ScThreadManager::terminate();
	if (number_of_nodes)
	{
		delete[] nodes;
		nodes = nullptr;
	}
}

//Initiates the nodes and holds the number of nodes available.  Will be used
//as a catch for user input that may be greater than numNodes.
void NodeManager::init(int numNodes, bool random)
{
	ScThreadManager::init(numNodes);
	number_of_nodes = numNodes;
}

//Used in bebugging, but not dealing with pre-compiler stuff just yet.
void NodeManager::terminate()
{
	ScThreadManager::terminate();
}

//Get the node to print the data.  As this manager no longer handles the nodes directly,
//it is now a call to ScThreadManager.
void NodeManager::printNodeData(int id)
{
	ScThreadManager::getNode(id)->printData();
}

//Returns the node that is requested by ID.
Node* NodeManager::getNode(int id)
{
	return ScThreadManager::getNode(id);
}