#include "MapObject.h"
#include "AnimationManager.h"
#include "Logger.h"
#include "Player.h"

using namespace Core;

MapObjectType::MapObjectType()
{
}

MapObjectType::MapObjectType(std::string name, std::string type)
	: type(type)
{
	Utility::Logger::getInstance()->log("MapObjectType::MapObjectType(%s, %s)", name.c_str(), type.c_str());
}

MapObjectType::~MapObjectType()
{
}

Graphics::AnimationPack *MapObjectType::getGraphics()
{
	return &graphics;
}

bool MapObjectType::isEnabled() const
{
	return enabled;
}

void MapObjectType::setEnabled(bool enabled)
{
	this->enabled = enabled;
}

std::string MapObjectType::getType() const
{
	return type;
}

MapObject::MapObject(MapObjectType *type, Tile *tile, Player *owner)
	: tile(tile), owner(owner)
{
	//Utility::Logger::getInstance()->log("MapObjectType::MapObjectType()\n");
	this->type = type->clone();
	tile->addObject(this);
}

MapObject::~MapObject()
{
}

void MapObject::draw(Graphics::Drawer *target, int x, int y)
{
	if( (!owner->isDone()) && (owner->getSelectedObject() == this) )
	{
		// FIXME: Cache map image!!!
		Graphics::Surface selection = Graphics::Surface("Gfx/Selection.png");
		selection.blit(target->getTarget(), x, y);
	}
	type->getGraphics()->getCurrent()->draw(target, x, y);
}

Tile* MapObject::getTile() const
{
	return tile;
}

bool MapObject::isEnemy(MapObject *other) const
{
	if(other == this)
		return false;

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
