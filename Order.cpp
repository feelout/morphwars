#include <algorithm>
#include "Order.h"
#include "OrderManager.h"
#include "AStar.h"
#include "Logger.h"
#include "Unit.h"

using namespace Core;

Order::Order(MapObject *object, Map *map)
	: object(object), map(map), done(false), target(NULL)
{
}

Order::~Order()
{
}

void Order::execute(Tile *target)
{
	this->target = target;
	OrderManager::getInstance()->addOrder(this);
}

void Order::process()
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

std::string Order::getText() const
{
	std::string msg = "Order for unit at " + br_itoa(object->getTile()->getX()) + "," 
		+ br_itoa(object->getTile()->getY());
	if(target)
	{
		msg = msg + "; Target is " + br_itoa(target->getX()) + "," +
			br_itoa(target->getY());
	}

	return msg;
}

MovementOrder::MovementOrder(MapObject *object, Map *map)
	: Order(object, map)
{
}

void MovementOrder::execute(Tile *target)
{
	Order::execute(target);

	if(object->getOwner()->getFieldOfView()->isTileVisible(target->getX(), target->getY()))
	{
		if(target->isEnemy(object))
		{
			//Utility::Logger::getInstance()->log("Occupied by enemy\n");
			// Can`t move to tile occupied by enemy
			done = true;
			return;
		}
	}

	if(!makePath())
	{
		Utility::Logger::getInstance()->log("Failed to build path from (%i,%i) to (%i,%i)",
				object->getTile()->getX(), object->getTile()->getY(),
				target->getX(), target->getY());
		done = true;
		return;
	}
}

bool MovementOrder::makePath()
{
	Unit *unit = static_cast<Unit*>(object);

	Utility::Logger::getInstance()->log("A* : Calculating path from (%i,%i) to (%i,%i)\n", unit->getTile()->getX(),
		unit->getTile()->getY(), target->getX(), target->getY());

	int distance = unit->getTile()->getDistance(target);

	if(distance == 0)
	{
		Utility::Logger::getInstance()->log("A* : Zero distance, aborting\n");
		return false;
	}
	// TODO: if dist == 1, move without A*

	std::vector<AStar::Node*> closedlist;

	AStar::Node* srcNode = new AStar::Node(NULL, unit->getTile(),
			unit->getType()->getMovementType(), target);
	AStar::Node* currentNode = srcNode;

	closedlist.push_back(srcNode);
	
	// FIXME: If anything isn`t working, maybe should check if node already in lists 

	while(currentNode->getSource() != target)
	{
		Utility::Logger::getInstance()->log("A* : Iteration: currentNode = (%i,%i)\n",
			currentNode->getSource()->getX(), currentNode->getSource()->getY());
		// Adding neighbours to open list 
		std::vector< std::pair<int,int> > neighbourCoords = currentNode->getSource()->getNeighbours();
		std::vector< std::pair<int,int> >::iterator nb_iter;

		AStar::Node *minimumNode=NULL;

		for(nb_iter = neighbourCoords.begin(); nb_iter != neighbourCoords.end(); ++nb_iter)
		{
			// Checking for validity
			int nbx = nb_iter->first;
			int nby = nb_iter->second;
			Utility::Logger::getInstance()->log("A* : Checking neighbour (%i,%i)\n", nbx, nby);

			if( (nbx < 0) || (nbx >= map->getWidth()) || (nby < 0) || (nby >= map->getHeight()) )
			{
				Utility::Logger::getInstance()->log("A* : Neighbour outside of map bounds.\n");
				continue;
			}

			// Checking if node is already in closed list 
			std::vector<AStar::Node*>::iterator check;
			bool alreadyInCList = false;

			for(check = closedlist.begin(); check != closedlist.end(); ++check)
			{
				if( ((*check)->getSource()->getX() == nb_iter->first)
						&& ((*check)->getSource()->getY() == nb_iter->second) )
				{
					WriteToLog("A* : Neighbour already in closed list\n");
					alreadyInCList = true;
					break;
				}
			}

			if(alreadyInCList)
			{
				//Utility::Logger::getInstance()->log("Node (%i,%i) already in closed list\n",
						//nb_iter->first, nb_iter->second);
				continue;
			}

			if((unit->getOwner()->getFieldOfView()->isTileVisible(nbx, nby))
						&& (map->getTile(nbx, nby)->isEnemy(unit)))
			{
				Utility::Logger::getInstance()->log("A* : Enemy on tile\n");
				continue;
			}

			if( !unit->canMoveFromTo(closedlist.back()->getSource(),
					map->getTile(nbx, nby)))
			{
				Utility::Logger::getInstance()->log("A* : Tile inaccessible\n");
				continue;
			}
			// Everything is OK 

			AStar::Node *node = new AStar::Node(currentNode, map->getTile(nbx, nby),
					unit->getType()->getMovementType(), target);


			// Finding minimum-cost node 
			if((!minimumNode) || (minimumNode->getCost() >  node->getCost()))
			{
				// Free previous minimum
				if(minimumNode)
				{
					delete minimumNode;
				}

				//Utility::Logger::getInstance()->log("New minimum node: %i,%i with cost %i\n",
					//node->getSource()->getX(), node->getSource()->getY(), node->getCost());
				minimumNode = node;
			}
			else
			{
				// Free unneeded node
				delete node;
			}
		}

		if(!minimumNode)
		{
			Utility::Logger::getInstance()->log("A* : Unable to find path.\n");
			return false;
		}

		closedlist.push_back(minimumNode);
		currentNode = minimumNode;
	}
	// Constructing waypoint list from closed list */
	std::vector<AStar::Node*>::iterator cl_iter;

	for(cl_iter = ++(closedlist.begin()); cl_iter != closedlist.end(); ++cl_iter)
	//for(cl_iter = closedlist.begin(); cl_iter != closedlist.end(); ++cl_iter)
	{
		waypoints.push_back((*cl_iter)->getSource());
		/*Utility::Logger::getInstance()->log("Waypoint: (%i,%i)\n", (*cl_iter)->getSource()->getX(),
			(*cl_iter)->getSource()->getY());*/
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
	/*Utility::Logger::getInstance()->log("MovementOrder::process().Unit tile: %i,%i. Unit MP: %i",
		unit->getTile()->getX(), unit->getTile()->getY(), unit->getMP());*/
	Unit *unit = static_cast<Unit*>(object);
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
				//Utility::Logger::getInstance()->log("Moving unit to next waypoint\n");
				if(unit->moveTo(*currentWaypoint))
				{
					currentWaypoint++;
				}
			}
			else
			{
				//Utility::Logger::getInstance()->log("Next waypoint is hostile\n");
				unit->getOwner()->setDone(true);
				done = true;
			}
		}
	}
}

Order* MovementOrderCreator::createOrder(MapObject *object, Map *map)
{
	return new MovementOrder(object, map);
}

/* AttackOrder class members */

AttackOrder::AttackOrder(MapObject *object, Map *map)
	: Order(object, map)
{
	Unit* unit = static_cast<Unit*>(object);

	if(unit->getHitsLeft() == 0)
		stop(); //TODO: How will it work with LPC::pendingOrder??
}

void AttackOrder::execute(Tile *target)
{
	Unit* unit = static_cast<Unit*>(object);

	unit->performAttack(target);
}

void AttackOrder::process()
{
}

Order* AttackOrderCreator::createOrder(MapObject *object, Map *map)
{
	return new AttackOrder(object, map);
}
