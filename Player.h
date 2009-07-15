#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include "assist.h"

namespace Core
{
	class PlayerController; //subclass for mouse+keyboard, ai, net
	class Unit;
	class Building;
	class Tile;

	enum Fraction
	{
		LEGACY=0,
		HAND,
		NEUTRAL,
	};

	struct FieldOfView
	{
		bool **visibility;

		FieldOfView(int w, int h);

		bool isTileVisible(Tile *tile);
	};

	class Player
	{
		private:
			std::string name;
			std::vector<Unit*> units;
			std::vector<Building*> buildings;

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
