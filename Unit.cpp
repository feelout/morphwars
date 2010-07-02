#include <cmath>
#include "Unit.h"
#include "Logger.h"
#include "Player.h"
#include "Timer.h"
#include "Order.h"
#include "MouseSelector.h"
#include "FontSystem.h"
#include "Drawer.h"

using namespace Core;

const int MOVEMENT_DELAY = 50;
const float MOVEMENT_X_UNIT = (TILE_WIDTH / 2) / float(TILE_HEIGHT_OFFSET);
const float MOVEMENT_Y_UNIT = 2 / MOVEMENT_X_UNIT; //TODO: Find out, why 2, not 1

UnitType::UnitType(std::string typeName)
	: MapObjectType(typeName, "Unit")
{
	//Utility::Logger::getInstance()->log("UnitType::UnitType(%s)\n", typeName.c_str());

	FILE *f_def = fopen(("Objects/Units/"+typeName+"/definition").c_str(),"r");

	if(!f_def)
	{
		Utility::Logger::getInstance()->log("Failed to load definition file for unit %s\n", typeName.c_str());
		return;
	}

	char name_buf[255];
	int mt;
	int retaliation;
	fscanf(f_def, "%s\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i", name_buf, &maxhp, &maxmp, &maxsp, &attack,
		       &hits, &distance, &retaliation, &defense, &cost, &mt);

	movementType = (MovementType)mt;
	Utility::Logger::getInstance()->log("MT: %i\n", mt);
	enemyRetaliates = (retaliation == 1);

	Graphics::Surface *strip = new Graphics::Surface("Objects/Units/"+typeName+"/Gfx.png");
	std::string xmlpath = "Opjects/Units/"+typeName+"/Animations.xml";

	graphics = Graphics::AnimationPack(strip, "Objects/Units/"+typeName+"/Animations.xml");	

	name = name_buf;

	char orderNameBuffer[255];
	while(!feof(f_def))
	{
		fscanf(f_def, "%s\n", &orderNameBuffer);
		Utility::Logger::getInstance()->log("Order %s added\n", orderNameBuffer);
		orders.push_back(orderNameBuffer);
	}

	defaultOrder = "move";

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
	//Utility::Logger::getInstance()->log("UnitType::clone()\n");
	UnitType *result = new UnitType();

	result->name = name;
	//Utility::Logger::getInstance()->log("Result->name == %s\n", result->name.c_str());
	result->type = type;
	result->maxhp = maxhp;
	result->maxsp = maxsp;
	result->maxmp = maxmp;
	result->attack = attack;
	result->hits = hits;
	result->distance = distance;
	result->enemyRetaliates = enemyRetaliates;
	result->defense = defense;
	result->cost = cost;
	result->movementType = movementType;
	result->enabled = enabled;
	result->orders = orders;
	result->defaultOrder = defaultOrder;

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

int UnitType::getHitCount() const
{
	return hits;
}

int UnitType::getAttackDistance() const
{
	return distance;
}

bool UnitType::doesEnemyRetaliate() const
{
	return enemyRetaliates;
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
	: MapObject(type, tile, owner), moving(false), canRetaliate(true), attackingState(false), dxmodifier(0),
		dymodifier(0), dstdx(0), dstdy(0), dst(NULL)
{
	//Utility::Logger::getInstance()->log("Unit::Unit: %s\n", this->type->getName().c_str());
	hp = type->getMaxHP();
	mp = type->getMaxMP();
	sp = type->getMaxSP();
	attack = type->getAttack();
	defense = type->getDefense();
	hitsLeft = type->getHitCount();

	//this->type = type->clone();
	infoScreen = new Gui::UnitInfoScreen(this);
}

UnitType* Unit::getType() const
{
	return static_cast<UnitType*>(type);
}

bool Unit::changePosition(Tile *newPosition)
{
	/*Utility::Logger::getInstance()->log("Changing unit position from (%i,%i) to (%i,%i)\n",
		tile->getX(), tile->getY(), newPosition->getX(), newPosition->getY());*/
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

bool Unit::moveTo(Tile *dst)
{
	Utility::Logger::getInstance()->log("Unit::moveTo (%i,%i)\n", dst->getX(), dst->getY());
	//Utiltiy::Logger::getInstance()->log("Direction : %s\n", Tile::DirectionToString(tile->getDirection
	if(tile == dst)
		return false;

	if(!dst->canBeAdded(this))
		return false;

	if(canMoveFromTo(tile, dst) && mp >= dst->getType()->getMovementCost(getType()->getMovementType()))	
	{
		switch(tile->getDirection(dst))
		{
			case NORTH:
				dxmodifier = 0;
				dymodifier = -2;
				break;
			case EAST:
				dxmodifier = 2;
				dymodifier = 0;
				break;
			case SOUTH:
				dxmodifier = 0;
				dymodifier = 2;
				break;
			case WEST:
				dxmodifier = -2;
				dymodifier = 0;
				break;
			case NORTHWEST:
				dxmodifier = -1;
				dymodifier = -1;
				break;
			case NORTHEAST:
				dxmodifier = 1;
				dymodifier = -1;
				break;
			case SOUTHEAST:
				dxmodifier = 1;
				dymodifier = 1;
				break;
			case SOUTHWEST:
				dxmodifier = -1;
				dymodifier = 1;
				break;
		}

		dstdx = (TILE_WIDTH / 2) * dxmodifier;
		dstdy = TILE_HEIGHT_OFFSET * dymodifier;

		//Utility::Logger::getInstance()->log("Params: %i,%i,%i,%i\n", dxmodifier, dymodifier, dstdx, dstdy);

		moving = true;
		prevTime = Utility::Timer::currentTicks();
		this->dst = dst;

		type->getGraphics()->changeToAnimation(type->getName()+"-"+Tile::DirectionToString(tile->getDirection(dst)));
		type->getGraphics()->getCurrent()->start();

		owner->setDone(false);

		mp -= dst->getType()->getMovementCost(getType()->getMovementType());

		return true;
	}
	else
	{
		owner->setDone(true);
		return false;
	}
}

/*
 * XXX : Cliff checking does not work!!! 
 */
bool Unit::canMoveFromTo(Tile *src, Tile *dst) const
{
	/*Utility::Logger::getInstance()->log("Checking movement from (%i,%i) to (%i,%i)\n",
			src->getX(), src->getY(), dst->getX(), dst->getY());*/
	/* Occupied by enemy */
	if(!dst->canBeAdded(this))
		return false;
	/* Not enough MP */ // Disable, so canMoveTo could be used in A*
	/*if(mp < dst->getType()->getMovementCost(getType()->getMovementType()))
		return false;*/
	int srch = src->getHeight();
	int dsth = dst->getHeight();

	/*Utility::Logger::getInstance()->log("Src height = %i\nDst height = %i\n", srch, dsth);
	Utility::Logger::getInstance()->log("Direction from source to dest : %s\n",
			Tile::DirectionToString(src->getDirection(dst)).c_str());
	Utility::Logger::getInstance()->log("Src cliff : %s\nDst cliff : %s\n",
			Tile::DirectionToString(src->getCliffDirection()).c_str(),
			Tile::DirectionToString(dst->getCliffDirection()).c_str());*/

	if(srch != dsth)
	{
		if(abs(srch-dsth) != 1)
			return false;
		/*if(dst->isCliff() && srch > dsth && dst->getCliffDirection() != src->getDirection(dst))
			return false;
		if(src->isCliff() && srch < dsth && src->getCliffDirection() != dst->getDirection(src))
			return false;*/
		if(dst->isCliff() && srch > dsth && dst->getCliffDirection() == src->getDirection(dst))
			return true;
		if(src->isCliff() && srch < dsth && src->getCliffDirection() == dst->getDirection(src))
			return true;
		/*if(!dst->isCliff() && !src->isCliff())
			return false;*/
		return false;
	}

	return true;
}

bool Unit::performAttack(Tile *tile)
{
	Utility::Logger::getInstance()->log("%s : performing attack\n", getType()->getName().c_str());
	if(this->tile == tile)
		return false;

	if(!isTileInAtackDistance(tile))
	{
		Utility::Logger::getInstance()->log("Target not in attack reach : %i/%i\n",
				tile->getDistance(this->tile),
				getType()->getAttackDistance());
		return false;
	}

	if(tile->isEnemy(this))
	{
		Direction dir = this->tile->getDirection(tile);

		type->getGraphics()->playTimedAnimation(type->getName()+"Attack-"+
				Tile::DirectionToString(this->tile->getDirection(tile)), 2000);
		type->getGraphics()->getCurrent()->start();

		attackingState = true;
		bool fightResult = false;

		for(int i=0; i < static_cast<UnitType*>(type)->getHitCount(); ++i)
		{
			fightResult = tile->getTopObject()->damage(attack, this);
		}

		attackingState = false;

		// FIXME: Add timed animation to animation pack
		/*type->getGraphics()->changeToAnimation(type->getName()+"-"+
				Tile::DirectionToString(this->tile->getDirection(tile)));*/
		return fightResult;
	}
	/*else
	{
		return moveTo(tile);
	}*/
}

bool Unit::damage(int damage, MapObject *source)
{
	Unit *unit_source = static_cast<Unit*>(source);

	int resulting_damage = damage - static_cast<UnitType*>(type)->getDefense();
	if(resulting_damage < 1)
		resulting_damage = 1;
	hp -= resulting_damage;

	if(hp <= 0)
	{
		kill();
		return true;
	}
	else
	{
		// Retaliation
		if(canRetaliate && !attackingState && unit_source->getType()->doesEnemyRetaliate())
		{
			//FIXME: Test
			//unit_source->damage(static_cast<UnitType*>(type)->getAttack(), this);
			performAttack(unit_source->getTile());
			canRetaliate = false;
		}
		return false;
	}
}

void Unit::kill()
{
	dead = true;
	tile->removeObject(this);
}

bool Unit::isTileInAtackDistance(Tile *tile) const
{
	return tile->getDistance(this->tile) <= getType()->getAttackDistance();
}

bool Unit::updateMovement()
{
	//Utility::Logger::getInstance()->log("Unit::updateMovement()\n");

	if(!moving)
		return true;

	//Utility::Logger::getInstance()->log("Prev: %i\nCurrent: %i\n", prevTime, Utility::Timer::currentTicks());
	if( (prevTime+MOVEMENT_DELAY) > Utility::Timer::currentTicks() )
		return false;

	prevTime = Utility::Timer::currentTicks();

	//Utility::Logger::getInstance()->log("dxm = %i, dym = %i, xu = %f, yu = %f\n", dxmodifier, dymodifier,
			//MOVEMENT_X_UNIT, MOVEMENT_Y_UNIT);

	dx += (float(dxmodifier) * MOVEMENT_X_UNIT);
	dy += (float(dymodifier) * MOVEMENT_Y_UNIT);

	//Utility::Logger::getInstance()->log("Dx = %f, Dy = %f\n", dx, dy);

	if( (abs(int(dx)) >= abs(dstdx)) && (abs(int(dy)) >= abs(dstdy)) )
	{
		Utility::Logger::getInstance()->log("Movement complete\n");
		if(changePosition(dst))
		{
			Utility::Logger::getInstance()->log("Changed position\n");
			dx = 0;
			dy = 0;
			dxmodifier = 0;
			dymodifier = 0;
			dstdx = 0;
			dstdy = 0;
			dst = NULL;
			moving = false;
			type->getGraphics()->getCurrent()->stop();
			return true;
		}
		else //Stop sprite moving, wait for another chance MAYBE NOT NEEDED
		{
			Utility::Logger::getInstance()->log("Cannot change position\n");
			return false;
		}
	}
	return false;
}

int Unit::getHP() const
{
	return hp;
}

int Unit::getSP() const
{
	return sp;
}

int Unit::getMP() const
{
	return mp;
}

int Unit::getHitsLeft() const
{
	return hitsLeft;
}

void Unit::setHP(int hp)
{
	this->hp = hp;
}

void Unit::setSP(int sp)
{
	this->sp = sp;
}

void Unit::setMP(int mp)
{
	this->mp = mp;
}

void Unit::onTurnBegin()
{
	//TODO: Introduce SP and HP regen
	setMP(getType()->getMaxMP());
	hitsLeft = getType()->getHitCount();
	canRetaliate = true;
}
