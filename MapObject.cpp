#include "MapObject.h"
#include "AnimationManager.h"
#include "Logger.h"
#include "Player.h"

using namespace Core;

MapObjectType::MapObjectType()
{
}

MapObjectType::MapObjectType(std::string name, std::string type)
	: name(name), type(type)
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

std::string MapObjectType::getName() const
{
	return name;
}

const std::list<std::string>& MapObjectType::getOrders() const
{
	return orders;
}

std::string MapObjectType::getDefaultOrder() const
{
	Utility::Logger::getInstance()->log("Object : default order %s requested\n", defaultOrder.c_str());
	return defaultOrder;
}

MapObject::MapObject(MapObjectType *type, Tile *tile, Player *owner)
	: tile(tile), owner(owner), dx(0), dy(0), type(type->clone()), dead(false), infoScreen(NULL), selected(false)
{
	//Utility::Logger::getInstance()->log("MapObject::MapObject: %s\n", this->type->getName().c_str());
	tile->addObject(this);
}

MapObject::~MapObject()
{
	tile->removeObject(this);
}

void MapObject::draw(Graphics::Surface *target, int x, int y)
{
	// TODO : Draw only if is top object on tile
	if(dead)
		return;

	//y -= (TILE_HEIGHT_LEVEL_OFFSET * (getTile()->getHeight()));
	//y -= TILE_HEIGHT_LEVEL_OFFSET;

	//if( (owner->isCurrent()) && (owner->getSelectedObject() == this) )
	if( (owner->isCurrent()) && selected )
	{
		// FIXME: Cache map image!!!
		Graphics::Surface selection = Graphics::Surface("Gfx/Selection.png");
		selection.blit(target, x+int(dx), y+int(dy));
	}
	Utility::Logger::getInstance()->log("MapObject(%i,%i)::draw(%i,%i)\n",
		getTile()->getX(), getTile()->getY(), x+int(dx), y+int(dy));
	type->getGraphics()->getCurrent()->draw(target, x+int(dx), y+int(dy));
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

void MapObject::setSelected(bool selected)
{
	this->selected = selected;
}

bool MapObject::isSelected() const
{
	return selected;
}

Gui::ObjectInfoScreen* MapObject::getInfoScreen() const
{
	return infoScreen;
}

bool MapObject::isDead() const
{
	return dead;
}
