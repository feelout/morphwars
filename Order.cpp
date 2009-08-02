#include <algorithm>
#include "Order.h"
#include "OrderManager.h"
#include "AStar.h"
#include "Logger.h"
#include "Unit.h"

using namespace Core;

Order::Order(Unit *unit)
	: unit(unit), done(false)
{
	//Utility::Logger::getInstance()->log("Order::Order\n");
	OrderManager::getInstance()->addOrder(this);
}

//DEBUG
Order::~Order()
{
	//Utility::Logger::getInstance()->log("Order destructor called\n");
}
//ENDDEBUG

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
	//Utility::Logger::getInstance()->log("MovementOrder::MovementOrder\n");

	if(unit->getOwner()->getFieldOfView()->isTileVisible(target->getX(), target->getY()))
	{
		if(target->isEnemy(unit))
		{
			Utility::Logger::getInstance()->log("Occupied by enemy\n");
			// Can`t move to tile occupied by enemy
			done = true;
			return;
		}
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
	Utility::Logger::getInstance()->log("Calculating path from (%i,%i) to (%i,%i)\n", unit->getTile()->getX(),
		unit->getTile()->getY(), target->getX(), target->getY());

	int distance = unit->getTile()->getDistance(target);

	if(distance == 0)
	{
		return false;
	}
	// TODO: if dist == 1, move without A*

	//std::vector<AStar::Node*> openlist;
	std::vector<AStar::Node*> closedlist;

	AStar::Node* srcNode = new AStar::Node(NULL, unit->getTile(),
			((UnitType*)(unit->getType()))->getMovementType(), target);
	AStar::Node* currentNode = srcNode;

	closedlist.push_back(srcNode);
	
	/* FIXME: If anything isn`t working, maybe should check if node already in lists */

	while(currentNode->getSource() != target)
	{
		Utility::Logger::getInstance()->log("Iteration: currentNode = (%i,%i)\n",
			currentNode->getSource()->getX(), currentNode->getSource()->getY());
		/* Adding neighbours to open list */
		std::vector< std::pair<int,int> > neighbourCoords = currentNode->getSource()->getNeighbours();
		std::vector< std::pair<int,int> >::iterator nb_iter;

		AStar::Node *minimumNode=NULL;

		for(nb_iter = neighbourCoords.begin(); nb_iter != neighbourCoords.end(); ++nb_iter)
		{
			// Checking for validity
			int nbx = nb_iter->first;
			int nby = nb_iter->second;

			if( (nbx < 0) || (nbx >= map->getWidth()) || (nby < 0) || (nby >= map->getHeight()) )
			{
				continue;
			}

			/* Checking if node is already in closed list */
			std::vector<AStar::Node*>::iterator check;
			bool alreadyInCList = false;

			for(check = closedlist.begin(); check != closedlist.end(); ++check)
			{
				if( ((*check)->getSource()->getX() == nb_iter->first)
						&& ((*check)->getSource()->getY() == nb_iter->second) )
				{
					alreadyInCList = true;
					break;
				}
			}

			if(alreadyInCList)
			{
				continue;
			}

			/* Everything is OK */

			AStar::Node *node = new AStar::Node(currentNode, map->getTile(nbx, nby),
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
		}

		closedlist.push_back(minimumNode);
		currentNode = minimumNode;
	}
	/* Constructing waypoint list from closed list */
	std::vector<AStar::Node*>::iterator cl_iter;

	for(cl_iter = ++(closedlist.begin()); cl_iter != closedlist.end(); ++cl_iter)
	//for(cl_iter = closedlist.begin(); cl_iter != closedlist.end(); ++cl_iter)
	{
		waypoints.push_back((*cl_iter)->getSource());
		Utility::Logger::getInstance()->log("Waypoint: (%i,%i)\n", (*cl_iter)->getSource()->getX(),
			(*cl_iter)->getSource()->getY());
	}

	currentWaypoint = waypoints.begin();

	/* Free list members*/
	// Shit, closed nodes are also in openlist
	for(cl_iter = closedlist.begin(); cl_iter != closedlist.end(); ++cl_iter)
	{
		delete (*cl_iter);
	}

	return true;
}

void MovementOrder::process()
{
	/*Utility::Logger::getInstance()->log("MovementOrder::process().Unit tile: %i,%i\n",
		unit->getTile()->getX(), unit->getTile()->getY());*/
	if(done)
	{
		return;
	}


	if(unit->updateMovement())
	{
		if(unit->getTile() == target)
		{
			Utility::Logger::getInstance()->log("MovementOrder done\n");
			done = true;
			unit->getOwner()->setDone(true);
			return;
		}
		else
		{
			if(!(*currentWaypoint)->isEnemy(unit))
			{
				unit->moveTo(*currentWaypoint++);
			}
			else
			{
				unit->getOwner()->setDone(true);
				done = true;
			}
		}
	}
}
