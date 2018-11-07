#include "ScThreadManager.h"

#ifdef _WIN64

HANDLE ScThreadManager::BuildThread(int id)
{

	HANDLE tHandle = CreateThread(NULL, 0, NULL, NULL, 0,  NULL);

	return tHandle;
}

#elif _linux_

#endif