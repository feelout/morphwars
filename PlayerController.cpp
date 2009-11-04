#include "PlayerController.h"
#include "Logger.h"
#include "MapObject.h"
#include "Unit.h"
#include "Order.h"
#include "Engine.h"
#include "MouseSelector.h"

using namespace Core;

/* PlayerController */
PlayerController::PlayerController(Player *target)
	: target(target)
{
	target->setController(this);
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

bool LocalPlayerController::objectTargeted(Tile *clickedTile)
{
	if(!clickedTile)
		return false;

	MapObject *selected = target->getSelectedObject();

	if(!selected)
	{
		return false;
	}

	selected->defaultTargetOrder(clickedTile, map);

	return true;
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

	if(MouseState::getInstance()->getActionType() == MouseState::SELECT)
	{
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
	else if(MouseState::getInstance()->getActionType() == MouseState::MOVE)
	{
		return objectTargeted(map->getTileByMouseCoords(x,y));
	}

	return false;
}

bool LocalPlayerController::mouseRMBClicked(int x, int y)
{
	if(!target->isCurrent())
		return false;

	return objectTargeted(map->getTileByMouseCoords(x,y));
}

bool LocalPlayerController::keyPressed(int key)
{
	switch(key)
	{
		case SDLK_m:
			//MOVE;
			if(target->getSelectedObject())
				MouseState::getInstance()->setActionType(MouseState::MOVE);
			return true;
		case SDLK_a:
			//ATTACK
			if(target->getSelectedObject())
				MouseState::getInstance()->setActionType(MouseState::ATTACK);
			return true;
		case SDLK_s:
			//STOP
			if(target->getSelectedObject())
				MouseState::getInstance()->setActionType(MouseState::SELECT);
			return true;
		case SDLK_k:
			// SKILL
			if(target->getSelectedObject())
				MouseState::getInstance()->setActionType(MouseState::SKILL);
			return true;
	}
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
