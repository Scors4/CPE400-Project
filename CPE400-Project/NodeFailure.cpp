#include "NodeManager.h"
#include <iostream>

using namespace std;

int main()
{
	NodeManager* nm = new NodeManager();

	nm->init(5, false);

	cin.ignore();
	nm->printNodeData(3);
	cin.ignore();
	nm->printNodeData(5);
	cin.ignore();
	nm->terminate();
	cin.ignore();

	delete nm;
	nm = nullptr;
}