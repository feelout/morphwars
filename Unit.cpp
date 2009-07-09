#include <stdio.h>
#include "Unit.h"
#include "Logger.h"

using namespace Core;

UnitType::UnitType(std::string typeName)
	: MapObjectType(typeName)
{
	printf("UnitType::UnitType(%s)\n", typeName.c_str());

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

	name = name_buf;
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
	result->maxhp = maxhp;
	result->maxsp = maxsp;
	result->attack = attack;
	result->defense = defense;
	result->cost = cost;
	result->movementType = movementType;
	result->enabled = enabled;
	//TODO: Copy graphics
	
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
	: MapObject(type, tile, owner)
{
	hp = type->getMaxHP();
	mp = type->getMaxMP();
	sp = type->getMaxSP();
	attack = type->getAttack();
	defense = type->getDefense();
}
