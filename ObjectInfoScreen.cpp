#include "ObjectInfoScreen.h"
#include "FontSystem.h"
#include "Tile.h"
#include "assist.h"
#include "Unit.h"
#include "Drawer.h"

using namespace Gui;

const int MIN_WIDTH = 100;
const int MIN_HEIGHT = 80;
const int TEXT_PLACE_HEIGHT = 20;

ObjectInfoScreen::ObjectInfoScreen(Core::MapObject *object, Widget *parent)
	: Widget(Rect(0,0,MIN_WIDTH,MIN_HEIGHT), parent), object(object)
{
}

void ObjectInfoScreen::draw(Graphics::Surface *target)
{
	Graphics::Drawer drawer(target);

	object->getType()->getGraphics()->getCurrent()->draw(target, frame.x, frame.y);

	Graphics::FontSystem *fs = Graphics::FontSystem::getInstance();

	fs->print(target, object->getType()->getName(), frame.x + Core::TILE_WIDTH, frame.y, RGBColor::WHITE);

	drawer.drawRect(frame, RGBColor::WHITE);
}

/*
 * UnitInfoScreen members
 */
UnitInfoScreen::UnitInfoScreen(Core::Unit *object, Widget *parent)
	: ObjectInfoScreen(object, parent)
{
}

void UnitInfoScreen::draw(Graphics::Surface *target)
{
	ObjectInfoScreen::draw(target);

	Core::Unit *unit = static_cast<Core::Unit*>(object);

	Graphics::FontSystem *fs = Graphics::FontSystem::getInstance();

	fs->print(target, "HP : " + br_itoa(unit->getHP()) + "/" + br_itoa(unit->getType()->getMaxHP()),
			frame.x + Core::TILE_WIDTH, frame.y + TEXT_PLACE_HEIGHT, RGBColor::RED);
	fs->print(target, "SP : " + br_itoa(unit->getSP()) + "/" + br_itoa(unit->getType()->getMaxSP()),
			frame.x + Core::TILE_WIDTH, frame.y + TEXT_PLACE_HEIGHT*2, RGBColor::BLUE);
	fs->print(target, "MP : " + br_itoa(unit->getMP()) + "/" + br_itoa(unit->getType()->getMaxMP()),
			frame.x + Core::TILE_WIDTH, frame.y + TEXT_PLACE_HEIGHT*3, RGBColor::GREEN);
}
