#include "PlayerController.h"
#include "Logger.h"
#include "MapObject.h"
#include "Unit.h"
#include "Order.h"
#include "Engine.h"

using namespace Core;

/* PlayerController */
PlayerController::PlayerController(Player *target)
	: target(target)
{
}

PlayerController::~PlayerController()
{
}

void PlayerController::newTurn()
{
}

/* LocalPlayerController */
LocalPlayerController::LocalPlayerController(Player *target, Map *map)
	: PlayerController(target), map(map)
{
	Utility::Logger::getInstance()->log("LocalPlayerController created for %s\n", target->getName().c_str());
	Engine::getInstance()->getEventDispatcher()->attachListener(this);
}

LocalPlayerController::~LocalPlayerController()
{
	Engine::getInstance()->getEventDispatcher()->detachListener(this);
}

bool LocalPlayerController::mouseMoved(int x, int y)
{
	if(!target->isCurrent())
		return false;

	return true;
}

bool LocalPlayerController::mouseLMBClicked(int x, int y)
{
	if(!target->isCurrent())
		return false;

	Tile *clickedTile = map->getTileByMouseCoords(x, y);

	if(!clickedTile)
		return false;
	
	MapObject *topobject = NULL;
	topobject = clickedTile->getTopObject();

	if(topobject)
	{
		if(topobject->getOwner() == target)
		{
			target->selectObject(topobject);
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool LocalPlayerController::mouseRMBClicked(int x, int y)
{
	if(!target->isCurrent())
		return false;

	Tile *clickedTile = map->getTileByMouseCoords(x, y);

	if(!clickedTile)
		return false;

	MapObject *selected = target->getSelectedObject();

	if(!selected)
	{
		Utility::Logger::getInstance()->log("Nothing clicked\n");
		return false;
	}

	Utility::Logger::getInstance()->log("Selected type: %s\n", selected->getType()->getType().c_str());

	//FIXME : Move to MapObject, something like "DefaultRMBOrder"
	if(selected->getType()->getType() == "Unit") //ugh, hate it
	{
		Utility::Logger::getInstance()->log("Moving unit\n");
		Unit *targetUnit = static_cast<Unit*>(selected);
		MovementOrder *order = new MovementOrder(targetUnit, clickedTile, map);
	}
	return true;
}

bool LocalPlayerController::keyPressed(int key)
{
	return false;
}

/* AIPlayerController */
AIPlayerController::AIPlayerController(Player *target)
	: PlayerController(target)
{
}

AIPlayerController::~AIPlayerController()
{
}

void AIPlayerController::newTurn()
{
	//STUB
}
