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
	//ScThreadManager::terminate();
	if (number_of_nodes)
	{
		delete[] nodes;
		nodes = nullptr;
	}
}

void NodeManager::init(int numNodes, bool random)
{
	ScThreadManager::init(numNodes);
	number_of_nodes = numNodes;
}

void NodeManager::run()
{
	
}

void NodeManager::terminate()
{
	ScThreadManager::terminate();
}

void NodeManager::printNodeData(int id)
{
	ScThreadManager::getNode(id)->printData();
}