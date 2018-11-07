#include "NodeManager.h"
#include <iostream>

using namespace std;

int main()
{
	NodeManager* nm = new NodeManager();

	nm->init(15, false);

	while (true)
	{
		nm->run();
		cin.ignore();
	}

	delete nm;
	nm = nullptr;
}