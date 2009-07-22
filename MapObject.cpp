#include "MapObject.h"
#include "AnimationManager.h"
#include "Logger.h"
#include "Player.h"

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
	//Utility::Logger::getInstance()->log("MapObjectType::MapObjectType()\n");
	tile->addObject(this);
}

MapObject::~MapObject()
{
}

void MapObject::draw(Graphics::Drawer *target, int x, int y)
{
	type->getGraphics()->getCurrent()->draw(target, x, y);
}

Tile* MapObject::getTile() const
{
	return tile;
}

bool MapObject::isEnemy(MapObject *other) const
{
	return (owner->getForce() != other->getOwner()->getForce());
}

Player* MapObject::getOwner() const
{
	return owner;
}

MapObjectType* MapObject::getType() const
{
	return type;
}
