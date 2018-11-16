#include "NodeManager.h"
#include <iostream>

using namespace std;

int main()
{
	NodeManager* nm = new NodeManager();

	nm->init(5, false);

	cin.ignore();

	delete nm;
	nm = nullptr;
}