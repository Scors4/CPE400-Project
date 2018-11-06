#include "NodeManager.h"
#include <iostream>

int main()
{
	NodeManager* nm = new NodeManager();

	nm->init();

	while (true)
	{
		nm->run();
	}

	delete nm;
	nm = nullptr;
}