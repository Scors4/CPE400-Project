#pragma once
#ifndef _NODEMANAGER_H
#define _NODEMANAGER_H
#include "Node.h"


class NodeManager
{
public:

	NodeManager();
	~NodeManager();

	void init(int num_nodes, bool random);
	void terminate();
	void printNodeData(int id);

	Node* getNode(int id);

	int number_of_nodes;

private:

};

#endif