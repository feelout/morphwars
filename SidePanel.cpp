#include "SidePanel.h"

using namespace Gui;

const int SidePanel::SIDE_PANEL_WIDTH;
const int ResourceBar::RESOURCE_BAR_HEIGHT;

/* ResourceBar class */
ResourceBar::ResourceBar(Rect frame)
	: Widget(frame), currentPlayer(NULL)
{
}

void ResourceBar::setCurrentPlayer(Core::Player *player)
{
	currentPlayer = player;
}

void ResourceBar::draw(Graphics::Drawer *target)
{
	target->drawRect(frame, RGBColor::WHITE);
	if(!currentPlayer)
		return;
	//FIXME:STUB, fix when resources are done
}

/* SidePanel class */
SidePanel::SidePanel(Rect frame, Core::Map *map)
	: Container(frame), minimap(NULL), currentPlayer(NULL), resources(NULL),
	actions(NULL), tileobjects(NULL), buttons(NULL)
{
	resources = new ResourceBar(Rect(frame.x+5, frame.y+5, SIDE_PANEL_WIDTH-10, ResourceBar::RESOURCE_BAR_HEIGHT));
	minimap = new Minimap(Rect(frame.x+5, frame.y+ResourceBar::RESOURCE_BAR_HEIGHT+10, SIDE_PANEL_WIDTH-10, 
				SIDE_PANEL_WIDTH-10), map);
	addChild(resources);
	addChild(minimap);
}

SidePanel::~SidePanel()
{
	delete minimap;
	delete resources;
}

void SidePanel::setCurrentPlayer(Core::Player *player)
{
	currentPlayer = player;
	resources->setCurrentPlayer(player);
}

void SidePanel::draw(Graphics::Drawer *target)
{
	Container::draw(target);

	target->drawRect(frame, RGBColor::WHITE);
}
