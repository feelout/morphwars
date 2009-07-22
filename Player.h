#ifndef PLAYER_H 
#define PLAYER_H

#include <string>
#include <vector>
#include "assist.h"
#include "Force.h"

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

	class FieldOfView
	{
		private:
			bool *tiles;
			int w,h;
		public:
			FieldOfView(int w, int h);

			bool isTileVisible(int x, int y);
			void setTileVisible(int x, int y, bool visible);
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
			Force *force;

			FieldOfView *fov;

			void updateFOV();
		public:
			Player(std::string name, Fraction fraction, Force *force, RGBColor color, 
					int mapWidth, int mapHeight);

			void addUnit(Unit *unit);
			void addBuilding(Building *building);

			FieldOfView* getFieldOfView() const;
			Force* getForce() const;
	};
}

#endif //PLAYER_H
