#include "SidePanel.h"
#include "MapObject.h"
#include "Logger.h"
#include "OrderPack.h"
#include "Drawer.h"

using namespace Gui;

const int SidePanel::SIDE_PANEL_WIDTH;
const int ResourceBar::RESOURCE_BAR_HEIGHT;
const int UNIT_BAR_HEIGHT = 80;

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
	: VBox(frame, 5), minimap(NULL), currentPlayer(NULL), resources(NULL),
	actions(NULL), tileobjects(NULL), buttons(NULL)
{
	Utility::Logger::getInstance()->log("Creating SidePanel with dimensions %i,%i\n", frame.w, frame.h);
	Utility::Logger::getInstance()->log("Requested %i,%i\n", requestedFrame.w, requestedFrame.h);
	//Rect rb_frame(frame.x+5, frame.y+5, SIDE_PANEL_WIDTH-10, ResourceBar::RESOURCE_BAR_HEIGHT);
	resources = new ResourceBar(Rect(0,0, 100, ResourceBar::RESOURCE_BAR_HEIGHT));
	//rb_frame.y += rb_frame.h+10;
	// Keep the minimap square
	//rb_frame.w = SIDE_PANEL_WIDTH-10;
	//rb_frame.h = SIDE_PANEL_WIDTH-10;
	minimap = new Minimap(Rect(0, 0, SIDE_PANEL_WIDTH-10, SIDE_PANEL_WIDTH-10), map);
	
	//rb_frame.y += rb_frame.h + 10 + UNIT_BAR_HEIGHT;
	//rb_frame.h = OrderPack::ORDER_BUTTON_HEIGHT;
	actions = new OrderPack(Rect(0,0,100, OrderPack::ORDER_BUTTON_HEIGHT));

	addChild(resources, false, 0);
	addChild(minimap, false, 0);
	addChild(actions, true, 0);

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
	Utility::Logger::getInstance()->log("SidePanel::setCurrentPlayer\n");
	currentPlayer = player;
	resources->setCurrentPlayer(player);
}

void SidePanel::draw(Graphics::Surface *target)
{
	Container::draw(target);

	/*if(currentPlayer && currentPlayer->getSelectedObject())
	{
		currentPlayer->getSelectedObject()->drawInfoPanel(target, frame.x, 
			frame.y+ResourceBar::RESOURCE_BAR_HEIGHT+10 + minimap->getFrame().h);
	}*/
	Graphics::Drawer(target).drawRect(frame, RGBColor::WHITE);
}
