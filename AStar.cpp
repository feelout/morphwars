#include "AStar.h"
#include "Logger.h"

using namespace Core::AStar;

const int DISTANCE_WEIGHT = 10;

int abs(int n)
{
	if(n < 0)
	{
		return -n;
	}
	else
	{
		return n;
	}
}

Node::Node(Node *parent, Tile *src, MovementType movementtype, Tile *target) 
	: src(src), parent(parent)
{
	int g = src->getType()->getMovementCost(movementtype);
	if(parent)
	{
		g += parent->getCost();
	}

	/* TODO: Check whether distinguishing between shifted rows is needed */
	//int h = abs(target->getX()-src->getX()) + abs(target->getY()-src->getY());
	int h = src->getDistance(target)*DISTANCE_WEIGHT;
	cost = g + h;
	//Utility::Logger::getInstance()->log("Cost = %i\n", cost);
}

int Node::getCost() const
{
	return cost;
}

Core::Tile* Node::getSource() const
{
	return src;
}

bool Node::operator==(const Node &other) const
{
	return (this->src == other.src);
}
