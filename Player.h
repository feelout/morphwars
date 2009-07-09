#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <list>
#include "assist.h"

namespace Core
{
	class PlayerController; //subclass for mouse+keyboard, ai, net
	class Unit;
	class Building;

	enum Fraction
	{
		LEGACY=0,
		HAND,
		NEUTRAL,
	};

	class Player
	{
		private:
			std::string name;
			std::list<Unit*> units;
			std::list<Building*> buildings;

			int energy;

			RGBColor color;
			Fraction fraction;
		public:
			Player(std::string name, Fraction fraction, RGBColor color);

			void addUnit(Unit *unit);
			void addBuilding(Building *building);
	};
}

#endif //PLAYER_H
