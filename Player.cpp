#include <algorithm>
#include "Player.h"
#include "Logger.h"
#include "Unit.h"

using namespace Core;

bool BackwardCompareUnits(Unit *u1, Unit *u2)
{
	Tile *t1 = u1->getTile();
	Tile *t2 = u2->getTile();

	/*if(t1->getY() != t2->getY())
	{
		return t1->getY() > t2->getY();
	}
	else
	{
		return t1->getX() > t2->getX();
	}*/
	return true;
}

Player::Player(std::string name, Fraction fraction, RGBColor color)
	: name(name), fraction(fraction), color(color), energy(0)
{
	Utility::Logger::getInstance()->log("Player %s created.\n", name.c_str());
}

void Player::addUnit(Unit *unit)
{
	units.push_back(unit);

	std::sort(units.begin(), units.end(), BackwardCompareUnits); 
}

void Player::addBuilding(Building *building)
{
	buildings.push_back(building);
}
