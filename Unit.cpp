#include <stdio.h>
#include "Unit.h"
#include "Logger.h"
#include "Player.h"

using namespace Core;

UnitType::UnitType(std::string typeName)
	: MapObjectType(typeName, "Unit")
{
	Utility::Logger::getInstance()->log("UnitType::UnitType(%s)\n", typeName.c_str());

	FILE *f_def = fopen(("Objects/Units/"+typeName+"/definition").c_str(),"r");

	if(!f_def)
	{
		Utility::Logger::getInstance()->log("Failed to load definition file for unit %s\n", typeName.c_str());
		return;
	}

	char name_buf[255];
	int mt;
	fscanf(f_def, "%s\n%i\n%i\n%i\n%i\n%i\n%i\n%i", name_buf, &maxhp, &maxmp, &maxsp, &attack, &defense,
			&cost, &mt);

	movementType = (MovementType)mt;

	Graphics::Surface *strip = new Graphics::Surface("Objects/Units/"+typeName+"/Gfx.png");
	std::string xmlpath = "Opjects/Units/"+typeName+"/Animations.xml";

	graphics = Graphics::AnimationPack(strip, "Objects/Units/"+typeName+"/Animations.xml");	

	name = name_buf;

	delete strip;
}

//FIXME
UnitType::~UnitType()
{
	//MapObject::~MapObjectType();
}

UnitType::UnitType()
{
}

UnitType* UnitType::clone()
{
	UnitType *result = new UnitType();

	result->name = name;
	result->type = type;
	result->maxhp = maxhp;
	result->maxsp = maxsp;
	result->attack = attack;
	result->defense = defense;
	result->cost = cost;
	result->movementType = movementType;
	result->enabled = enabled;

	result->graphics = graphics;
	
	return result;
}

std::string UnitType::getName() const
{
	return name;
}

int UnitType::getMaxHP() const
{
	return maxhp;
}

int UnitType::getMaxMP() const
{
	return maxmp;
}

int UnitType::getMaxSP() const
{
	return maxsp;
}

int UnitType::getAttack() const
{
	return attack;
}

int UnitType::getDefense() const
{
	return defense;
}

int UnitType::getCost() const
{
	return cost;
}

MovementType UnitType::getMovementType() const
{
	return movementType;
}

Unit::Unit(UnitType *type, Tile *tile, Player *owner)
	: MapObject(type, tile, owner), dx(0), dy(0)
{
	hp = type->getMaxHP();
	mp = type->getMaxMP();
	sp = type->getMaxSP();
	attack = type->getAttack();
	defense = type->getDefense();

	//this->type = type->clone();
}

bool Unit::changePosition(Tile *newPosition)
{
	Utility::Logger::getInstance()->log("Changing unit position from (%i,%i) to (%i,%i)\n",
		tile->getX(), tile->getY(), newPosition->getX(), newPosition->getY());
	if(newPosition == tile)
		return false;
	
	if(newPosition->isEnemy(this))
		return false;

	if(newPosition->addObject(this))
	{
		tile->removeObject(this);
		tile = newPosition;
		owner->updateFOV();

		return true;
	}

	return false;
}
