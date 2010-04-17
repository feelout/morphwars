#include "PlayerController.h"
#include "Logger.h"
#include "MapObject.h"
#include "Unit.h"
#include "Order.h"
#include "Engine.h"
#include "MouseSelector.h"
#include "OrderFactory.h"

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
LocalPlayerController::LocalPlayerController(Player *target, Map *map, Gui::SidePanel *sidePanel)
	: PlayerController(target), map(map), pendingOrder(NULL), sidePanel(sidePanel), currentObject(NULL)
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

	//MapObject *selected = target->getSelectedObject();
	MapObject *selected = currentObject;

	if(!selected)
	{
		return false;
	}

	Order *order = OrderFactory::getInstance()->createOrder(selected->getType()->getDefaultOrder(), selected, map);
	if(order)
	{
		order->execute(clickedTile);
		return true;
	}

	return false;
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

	//if(MouseState::getInstance()->getActionType() == MouseState::SELECT)
	if(!pendingOrder)
	{
		Tile *clickedTile = map->getTileByMouseCoords(x, y);

		if(!clickedTile)
			return false;

		MapObject *topobject = NULL;
		topobject = clickedTile->getTopObject();

		if(topobject)
		{
			if(currentObject)
				currentObject->setSelected(false);
			if(topobject->getOwner() == target)
			{
				// TODO: Make it possible to select other players` units
				currentObject = topobject;
				currentObject->setSelected(true); 
				sidePanel->setCurrentObject(currentObject);
			}
			return true;
		}
		else
		{
			return false;
		}
	}
	//else if(MouseState::getInstance()->getActionType() == MouseState::MOVE)
	else
	{
		//return objectTargeted(map->getTileByMouseCoords(x,y));

		// Apply current order
		Tile *tile = map->getTileByMouseCoords(x,y);
		if(tile)
		{
			pendingOrder->execute(tile);
		}
		return tile != NULL;
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
	if(!target->isCurrent())
		return false;
	switch(key)
	{
		/* This is just temporary
		 * TODO: Think about providing hotkeys
		 */
		case SDLK_m:
			//MOVE;
			//if(target->getSelectedObject())
			if(currentObject)
				MouseState::getInstance()->setActionType(MouseState::MOVE);
			return false;
		case SDLK_a:
			//ATTACK
			//if(target->getSelectedObject())
			if(currentObject)
				MouseState::getInstance()->setActionType(MouseState::ATTACK);
			return false;
		case SDLK_s:
			//STOP
			//if(target->getSelectedObject())
			if(currentObject)
				MouseState::getInstance()->setActionType(MouseState::SELECT);
			return false;
		case SDLK_k:
			// SKILL
			//if(target->getSelectedObject())
			if(currentObject)
				MouseState::getInstance()->setActionType(MouseState::SKILL);
			return false;
		case SDLK_e:
			Utility::Logger::getInstance()->log("E key pressed\n");
			// TODO: Remove handler from Scenario
			target->endTurn();
			return true;
	}
	return false;
}

void LocalPlayerController::setOrder(Order *order)
{
	if(pendingOrder)
		delete pendingOrder;
	pendingOrder = order;
}

void LocalPlayerController::setOrder(std::string orderName)
{
	setOrder(OrderFactory::getInstance()->createOrder(orderName, currentObject, map));
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
