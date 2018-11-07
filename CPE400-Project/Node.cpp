#include "Node.h"

Node::Node()
{
	ID = 0;
}

Node::Node(char ID)
{
	this->ID = ID;
}

Node::~Node()
{

}

char Node::getID()
{
	return ID;
}

bool Node::setID(char ID)
{
	if (this->ID != 0)
		return false;

	this->ID = ID;
	return true;
}