#pragma once
#ifndef _SCTHREADMANAGER_H
#define _SCTHREADMANAGER_H
#include "Node.h"
#include <thread>
#include <iostream>

using namespace std;

struct thread_data {
	int m_id = 0;
	Node* m_node = nullptr;

	thread_data(int id) : m_id(id) {}
	thread_data() : m_id(0) {};
};

class ScThreadManager
{
public:

	static void init(int size);
	static void terminate();
	static void buildNode(int id, int node_count);
	static Node* getNode(int id);

private:
	static int number_of_threads;
	static thread_data* tHandles;

};

#endif