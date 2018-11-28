#include "ScThreadManager.h"

thread_data* ScThreadManager::tHandles = nullptr;
int ScThreadManager::number_of_threads = 0;
bool ScThreadManager::active = true;

//Terminates all the threads then deletes all avaialble pointers.
void ScThreadManager::terminate()
{
	active = false;

	for (int i = 0; i < number_of_threads; i++)
	{
		tHandles[i].m_node->terminate();

		delete tHandles[i].m_node;
		tHandles[i].m_node = nullptr;
	}

	delete[] tHandles;
	tHandles = nullptr;

	cout << "Nodes terminated.  Ready to exit." << endl;
}

//Builds the thread_data array and builds the nodes to populate it.
void ScThreadManager::init(int size)
{
	tHandles = new thread_data[size]();
	number_of_threads = size;

	for (int i = 0; i < size; i++)
	{
		buildNode(i + 1, size);
	}
}

//Builds the node for the system, also triggers the thread to be created and run by the node.
void ScThreadManager::buildNode(int id, int node_count)
{
	thread_data temp(id);
	Node * nTemp = new Node(id);
	nTemp->setRouteTableSize(node_count);
	temp.m_node = nTemp;
	
	tHandles[id - 1] = temp;
	tHandles[id - 1].m_node->run(tHandles[id - 1].m_node);

	nTemp = nullptr;
}

//Returns a pointer to the node with the requested ID.
Node* ScThreadManager::getNode(int id)
{
	return tHandles[id-1].m_node;
}