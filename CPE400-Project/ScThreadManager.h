#pragma once
#ifdef _WIN64 
#include <Windows.h>
#elif _linux_
#include <pthread.h>
#endif

struct thread_data {
	int m_id;
	thread_data(int id) : m_id(id) {}
};


#ifdef _WIN64
class ScThreadManager
{
public:
	static HANDLE BuildThread(int id);

private:
	HANDLE * tHandles;

};

#elif _linux_

#endif