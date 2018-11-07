#pragma once
#include "Node.h"

class NodeManager
{
public:

	NodeManager();
	~NodeManager();

	void init(int num_nodes,bool random);
	void run();

private:

	char NEXT_ID = 1;
	int numNodes;
	Node* nodes;

};