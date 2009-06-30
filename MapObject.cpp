#include "MapObject.h"
#include "AnimationManager.h"
#include "Logger.h"

using namespace Core;

bool MapObjectType::loadFromFile(std::string name)
{
	return false;
}

MapObjectType::MapObjectType(std::string name)
{
	if(!loadFromFile(name))
	{
		Utility::Logger::getInstance()->log("Object class %s loaded\n", name.c_str());
	}
	else
	{
		Utility::Logger::getInstance()->log("Failed to load object class %s\n", name.c_str());
	}
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
