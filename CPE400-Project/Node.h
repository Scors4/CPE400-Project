#pragma once
class Node
{
public:

	Node();
	Node(char ID);
	~Node();

	char getID();
	bool setID(char ID);

private:

	char ID;
};