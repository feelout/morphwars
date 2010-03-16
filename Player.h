#ifndef PLAYER_H 
#define PLAYER_H

#include <string>
#include <vector>
#include "assist.h"
#include "Force.h"
#include "Surface.h"

namespace Core
{
	class PlayerController; //subclass for mouse+keyboard, ai, net
	class Unit;
	class Building;
	class MapObject;
	class Map;

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

			bool isTileVisible(int x, int y) const;
			void setTileVisible(int x, int y, bool visible);

			void clear();

			FieldOfView& operator = (const FieldOfView& other);
			bool operator == (const FieldOfView& other);
	};

	class Player
	{
		private:
			std::string name;
			std::vector<Unit*> units;
			std::vector<Building*> buildings;

			MapObject *selected;

			int energy;

			RGBColor color;
			Fraction fraction;
			Force *force;

			bool done; //can end turn(no units are moving)
			bool current;
			bool endedTurn;

			FieldOfView *fov;

			PlayerController *controller;
		public:
			Player(std::string name, Fraction fraction, Force *force, RGBColor color, 
					int mapWidth, int mapHeight);

			void addUnit(Unit *unit);
			void addBuilding(Building *building);

			FieldOfView* getFieldOfView() const;
			Force* getForce() const;
			std::string getName() const;

			void selectObject(MapObject *object);
			MapObject *getSelectedObject() const;

			/* Has player done his turn. Only one player can have is set to false at a time*/
			bool isDone() const;
			void setDone(bool done);
			
			bool isCurrent() const;
			void setCurrent(bool current);

			bool hasEndedTurn() const;
			void endTurn();

			void updateFOV();

			void onTurnBegin();

			void setController(PlayerController *controller);
			PlayerController *getController() const;

			void renderObjects(Graphics::Surface *target, const FieldOfView *fov, const Map *map);
	};
}

#endif //PLAYER_H
