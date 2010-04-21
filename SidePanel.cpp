#include "SidePanel.h"
#include "MapObject.h"
#include "Logger.h"
#include "OrderPack.h"
#include "Drawer.h"
#include "PlayerController.h"

using namespace Gui;

const int SidePanel::SIDE_PANEL_WIDTH;
const int ResourceBar::RESOURCE_BAR_HEIGHT;
const int UNIT_BAR_HEIGHT = 80;
const int OrderButton::ORDER_ICON_WIDTH = 60;
const int OrderButton::ORDER_ICON_HEIGHT = 60;

OrderButtonCache* OrderButtonCache::instance = NULL;

OrderButton::OrderButton(std::string orderName)
	: ImageButton(Rect(0,0,ORDER_ICON_WIDTH,ORDER_ICON_HEIGHT),Graphics::Surface("Gfx/Actions/" + orderName + ".png")),
		       	orderName(orderName), receiver(NULL)
{
}

void OrderButton::setReceiver(Core::LocalPlayerController *receiver)
{
	Utility::Logger::getInstance()->log("OrderButton::setReceiver(%s)\n", receiver->getTarget()->getName().c_str());
	this->receiver = receiver;
}

bool OrderButton::mouseLMBClicked(int x, int y)
{
	if(frame.isPointInRect(x, y))
	{
		receiver->setOrder(orderName);
	}
}

/* OrderButtonCache class */
OrderButtonCache* OrderButtonCache::getInstance()
{
	if(!instance)
		instance = new OrderButtonCache();
	return instance;
}

OrderButton* OrderButtonCache::getButton(std::string orderName)
{
	if(buttons.count(orderName) == 0)
		buttons[orderName] = new OrderButton(orderName);
	return buttons[orderName];
}

/* OrderPanel class */
OrderPanel::OrderPanel(Rect frame, Widget *parent)
	: HBox(frame, 5, parent), currentObject(NULL)
{
}

void OrderPanel::setCurrentObject(Core::MapObject *currentObject)
{
	Utility::Logger::getInstance()->log("setCurrentObject(%s)\n", currentObject->getType()->getName().c_str());
	if(currentObject == this->currentObject)
		return;
	this->currentObject = currentObject;

	removeAll();

	const std::list<std::string>& orders = currentObject->getType()->getOrders();

	std::list<std::string>::const_iterator i;

	Core::LocalPlayerController *ctrl = static_cast<Core::LocalPlayerController*>(currentObject->getOwner()->
			getController());
	for(i = orders.begin(); i != orders.end(); ++i)
	{
		//OrderButton *button = new OrderButton(*i);
		OrderButton *button = OrderButtonCache::getInstance()->getButton(*i);
		button->setReceiver(ctrl);
		addChild(button, false, 0);
	}
}

/* ResourceBar class */
ResourceBar::ResourceBar(Rect frame)
	: Widget(frame), currentPlayer(NULL)
{
}

void ResourceBar::setCurrentPlayer(Core::Player *player)
{
	currentPlayer = player;
}

void ResourceBar::draw(Graphics::Surface *target)
{
	Graphics::Drawer(target).drawRect(frame, RGBColor::WHITE);
	if(!currentPlayer)
		return;
	//FIXME:STUB, fix when resources are done
}

/* SidePanel class */
SidePanel::SidePanel(Rect frame, Core::Map *map)
	: VBox(frame, 5), minimap(NULL), currentPlayer(NULL), currentObject(NULL), resources(NULL),
	/*actions(NULL),*/ tileobjects(NULL), buttons(NULL), objectInfo(NULL), orderPanel(NULL)
{
	Utility::Logger::getInstance()->log("Creating SidePanel with dimensions %i,%i\n", frame.w, frame.h);
	Utility::Logger::getInstance()->log("Requested %i,%i\n", requestedFrame.w, requestedFrame.h);

	resources = new ResourceBar(Rect(0,0, 100, ResourceBar::RESOURCE_BAR_HEIGHT));
	minimap = new Minimap(Rect(0, 0, SIDE_PANEL_WIDTH-10, SIDE_PANEL_WIDTH-10), map);
	//actions = new OrderPack(Rect(0,0,100, OrderPack::ORDER_BUTTON_HEIGHT));
	orderPanel = new OrderPanel(Rect(0,0,200,OrderButton::ORDER_ICON_HEIGHT+10), this);
	objectInfo = new Placeholder(Rect(0,0,100, 100));

	addChild(resources, false, 0);
	addChild(minimap, false, 0);
	addChild(objectInfo, true, 0);
	//addChild(actions, true, 0);
	addChild(orderPanel, false, 0);

	Utility::Logger::getInstance()->log("At the end requested %i,%i\n", requestedFrame.w, requestedFrame.h);
}

SidePanel::~SidePanel()
{
	delete minimap;
	delete resources;
}

void SidePanel::frameUpdated()
{
	//TODO: FIXME
	VBox::frameUpdated();
}

void SidePanel::setCurrentPlayer(Core::Player *player)
{
	currentPlayer = player;
	resources->setCurrentPlayer(player);
}

void SidePanel::setCurrentObject(Core::MapObject *object)
{
	currentObject = object;
	objectInfo->setChild(currentObject->getInfoScreen());
	orderPanel->setCurrentObject(object);
}

void SidePanel::draw(Graphics::Surface *target)
{
	//Container::draw(target);
	VBox::draw(target);

	/*if(currentPlayer && currentPlayer->getSelectedObject())
	{
		currentPlayer->getSelectedObject()->drawInfoPanel(target, frame.x, 
			frame.y+ResourceBar::RESOURCE_BAR_HEIGHT+10 + minimap->getFrame().h);
	}*/
	Graphics::Drawer(target).drawRect(frame, RGBColor::WHITE);
}
