#include <algorithm>
#include "Order.h"
#include "AStar.h"
#include "Logger.h"
#include "Unit.h"

using namespace Core;

Order::Order(Unit *unit)
	: unit(unit), done(false)
{
}

void Order::stop()
{
	done = true;
}

bool Order::isDone() const
{
	return done;
}


MovementOrder::MovementOrder(Unit *unit, Tile *target, Map *map)
	: Order(unit), target(target), map(map)
{
	if(target->isEnemy(unit))
	{
		// Can`t move to tile occupied by enemy
		done = true;
		return;
	}
	
	if(!makePath())
	{
		Utility::Logger::getInstance()->log("Failed to build path from (%i,%i) to (%i,%i)",
				unit->getTile()->getX(), unit->getTile()->getY(),
				target->getX(), target->getY());
		done = true;
		return;
	}
}

bool MovementOrder::makePath()
{
	//std::vector<AStar::Node*> openlist;
	std::vector<AStar::Node*> closedlist;

	AStar::Node* srcNode = new AStar::Node(NULL, unit->getTile(),
			((UnitType*)(unit->getType()))->getMovementType(), target);
	AStar::Node* currentNode = srcNode;

	closedlist.push_back(srcNode);
	
	/* FIXME: If anything isn`t working, maybe should check if node already in lists */

	while(currentNode->getSource() != target)
	{
		/* Adding neighbours to open list */
		std::vector< std::pair<int,int> > neighbourCoords = currentNode->getSource()->getNeighbours();
		std::vector< std::pair<int,int> >::iterator nb_iter;

		AStar::Node *minimumNode=NULL;

		for(nb_iter = neighbourCoords.begin(); nb_iter != neighbourCoords.end(); ++nb_iter)
		{
			AStar::Node *node = new AStar::Node(currentNode, map->getTile(nb_iter->first, nb_iter->second),
					((UnitType*)(unit->getType()))->getMovementType(), target);
			/* Finding minimum-cost node */
			if((!minimumNode) || (minimumNode->getCost() >  node->getCost()))
			{
				// Free previous minimum
				if(minimumNode)
				{
					delete minimumNode;
				}

				minimumNode = node;
			}
			else
			{
				// Free unneeded node
				delete node;
			}

			//openlist.push_back(node); //just to destroy nodes after pathfinding
		}

		closedlist.push_back(minimumNode);
		currentNode = minimumNode;
	}
	//TODO :CONTINUE HERE
	/* Constructing waypoint list from closed list */
	std::vector<AStar::Node*>::iterator cl_iter;

	for(cl_iter = ++(closedlist.begin()); cl_iter != closedlist.end(); ++cl_iter)
	{
		waypoints.push_back((*cl_iter)->getSource());
	}

	currentWaypoint = waypoints.begin();

	/*std::vector<AStar::Node*> nodes;
	nodes.resize(closedlist.size() + openlist.size());*/

	/* Free list members*/
	// Shit, closed nodes are also in openlist
	for(cl_iter = closedlist.begin(); cl_iter != closedlist.end(); ++cl_iter)
	{
		delete (*cl_iter);
	}

	/*for(cl_iter = openlist.begin(); cl_iter != openlist.begin(); ++cl_iter)
	{
		delete (*cl_iter);
	}*/
}

void MovementOrder::process()
{
	//STUB
}
