#include "Player.h"
#include "Logger.h"

using namespace Core;

Player::Player(std::string name, Fraction fraction, RGBColor color)
	: name(name), fraction(fraction), color(color), energy(0)
{
	Utility::Logger::getInstance()->log("Player %s created.\n", name.c_str());
}

void Player::addUnit(Unit *unit)
{
	units.push_back(unit);
}

void Player::addBuilding(Building *building)
{
	buildings.push_back(building);
}
