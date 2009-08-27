#include "PlayerController.h"
#include "Logger.h"
#include "MapObject.h"
#include "Unit.h"
#include "Order.h"

using namespace Core;

/* PlayerController */
PlayerController::PlayerController(Player *target)
	: target(target)
{
}

/* LocalPlayerController */
LocalPlayerController::LocalPlayerController(Player *target, Map *map)
	: PlayerController(target), map(map)
{
}

void LocalPlayerController::mouseMoved(int x, int y)
{
	if(!target->isCurrent())
		return;
}

void LocalPlayerController::mouseLMBClicked(int x, int y)
{
	if(!target->isCurrent())
		return;

	Tile *clickedTile = map->getTileByMouseCoords(x, y);

	if(!clickedTile)
		return;
	
	MapObject *topobject = NULL;
	topobject = clickedTile->getTopObject();

	if(topobject)
	{
		if(topobject->getOwner() == target)
		{
			target->selectObject(topobject);
		}
	}
}

void LocalPlayerController::mouseRMBClicked(int x, int y)
{
	if(!target->isCurrent())
		return;

	Tile *clickedTile = map->getTileByMouseCoords(x, y);

	if(!clickedTile)
		return;

	MapObject *selected = target->getSelectedObject();

	if(!selected)
	{
		Utility::Logger::getInstance()->log("Nothing clicked\n");
		return;
	}

	Utility::Logger::getInstance()->log("Selected type: %s\n", selected->getType()->getType().c_str());

	//FIXME : Move to MapObject, something like "DefaultRMBOrder"
	if(selected->getType()->getType() == "Unit") //ugh, hate it
	{
		Utility::Logger::getInstance()->log("Moving unit\n");
		Unit *targetUnit = static_cast<Unit*>(selected);
		MovementOrder *order = new MovementOrder(targetUnit, clickedTile, map);
	}
}

void LocalPlayerController::keyPressed(int key)
{
}

/* AIPlayerController */
AIPlayerController::AIPlayerController(Player *target)
	: PlayerController(target)
{
}
