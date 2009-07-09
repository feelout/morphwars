#include "MapObject.h"
#include "AnimationManager.h"
#include "Logger.h"

using namespace Core;

MapObjectType::MapObjectType()
{
}

MapObjectType::MapObjectType(std::string name)
{
}

MapObjectType::~MapObjectType()
{
}

Graphics::AnimationPack *MapObjectType::getGraphics()
{
	return &graphics;
}

MapObject::MapObject(MapObjectType *type, Tile *tile, Player *owner)
	: tile(tile), owner(owner)
{
	this->type = type->clone();
}

MapObject::~MapObject()
{
}

void MapObject::draw(Graphics::Drawer *target, int x, int y)
{
	type->getGraphics()->getCurrent()->draw(target, x, y);
}
