#include "PlayerController.h"
#include "Logger.h"
#include "MapObject.h"
#include "Unit.h"
#include "Order.h"
#include "Engine.h"
#include "MouseSelector.h"
#include "OrderFactory.h"
#include "OrderManager.h"

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

Player* PlayerController::getTarget()
{
	return target;
}

Gui::Container* PlayerController::getGUI()
{
	return NULL;
}

/* LocalPlayerController */
LocalPlayerController::LocalPlayerController(Player *target, Map *map)
	: PlayerController(target), map(map), pendingOrder(), sidePanel(NULL), currentObject(NULL)
{
	Utility::Logger::getInstance()->log("LocalPlayerController created for %s\n", target->getName().c_str());

	Graphics::Renderer *rend = Engine::getInstance()->getRenderer();
	gui = new Gui::HBox(Rect(0,0,rend->getWidth(), rend->getHeight()), 5);

	sidePanel = new Gui::SidePanel(Rect(0,0, Gui::SidePanel::SIDE_PANEL_WIDTH, rend->getHeight() - 100), map);
	sidePanel->setCurrentPlayer(target);
	//DEBUG!!
	gui->addChild(map, true, 0);
	gui->addChild(sidePanel, true, 0);

	Engine::getInstance()->getEventDispatcher()->attachListener(this);
}

LocalPlayerController::~LocalPlayerController()
{
	Engine::getInstance()->getEventDispatcher()->detachListener(this);
}

void LocalPlayerController::newTurn()
{
	if(currentObject)
	{
		if(currentObject->isDead())
		{
			WriteToLog("CurrentObject is dead - deleting\n");
			delete currentObject;
		}
		else
		{
			WriteToLog("CurrentObject is not dead\n");
			sidePanel->setCurrentObject(currentObject);
		}
	}
	sidePanel->setCurrentPlayer(target);
	map->setFieldOfView(target->getFieldOfView()); //FIXME: DEBUG
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

	//boost::shared_ptr<Order> order = OrderFactory::getInstance()->createOrder(selected->getType()->getDefaultOrder(), selected, map);
	setOrder(selected->getType()->getDefaultOrder());
	/*if(order.get())
	{
		// XXX DEBUG
		order->getText();
		order->execute(clickedTile);
		return true;
	}*/

	//return false;
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
		Utility::Logger::getInstance()->log("Executing pending order\n");
		//return objectTargeted(map->getTileByMouseCoords(x,y));

		// Apply current order
		Tile *tile = map->getTileByMouseCoords(x,y);
		if(tile)
		{
			Utility::Logger::getInstance()->log("Applying order to (%i,%i)\n", tile->getX(), tile->getY());
			pendingOrder->execute(tile);
			/* XXX : Found the error: before reset(), use_count == 1,
			 * despite adding another shared_ptr to the same Order
			 * to the OrderManager::orders vector. So, when we do reset,
			 * order pointed is deleted */
			/* XXX: As a workaround, I move addition from Order::execute to this point.
			 * So, must not forget, when other Controllers are done, put something similar
			 * to them */
			OrderManager::getInstance()->addOrder(pendingOrder);
			pendingOrder.reset();
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
			// TODO: Remove handler from Scenario
			target->endTurn();
			return true;
	}
	return false;
}

void LocalPlayerController::setOrder(boost::shared_ptr<Order> order)
{
	Utility::Logger::getInstance()->log("Controller::setOrder(%s)\n", order->getText().c_str());

	if(pendingOrder)
	{
		Utility::Logger::getInstance()->log("Controller : stopping pending order\n");
		pendingOrder->stop();
		pendingOrder.reset();
	}
	Utility::Logger::getInstance()->log("Controller : order.use_count = %d\n", order.use_count());
	/// XXX Is this the real bug?
	pendingOrder = order;
	if(pendingOrder) {
		Utility::Logger::getInstance()->log("PO is OK\n");
		pendingOrder->getText().c_str();
	}
	if(order) {
		Utility::Logger::getInstance()->log("Order is OK\n");
	}
	Utility::Logger::getInstance()->log("Controller : order.use_count = %d\n", order.use_count());
	Utility::Logger::getInstance()->log("New pendingOrder for %s : %s\n", target->getName().c_str(),
			pendingOrder->getText().c_str());
}

void LocalPlayerController::setOrder(std::string orderName)
{
	WriteToLog("LocalPlayerController::setOrder(orderName)\n");
	boost::shared_ptr<Order> order = OrderFactory::getInstance()->createOrder(orderName, currentObject, map);
	WriteToLog(order->getText().c_str()); // XXX DEBUG
	setOrder(order);
}

Gui::Container* LocalPlayerController::getGUI()
{
	return gui;
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
