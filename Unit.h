#ifndef UNIT_H
#define UNIT_H

#include "MapObject.h"

namespace Core
{
	class UnitType: public MapObjectType
	{
		protected:
			std::string name;
			int maxhp; 	// Hit points
			int maxmp;	// Movement points
			int maxsp;	// Skill points
			int attack;	// Attack
			int defense;	// Defense
			int cost;	// Cost in energy
			MovementType movementType;
			
			UnitType();
		public:
			UnitType(std::string typeName);
			virtual ~UnitType();

			virtual UnitType* clone();

			std::string getName() const;
			int getMaxHP() const;
			int getMaxMP() const;
			int getMaxSP() const;
			int getAttack() const;
			int getDefense() const;
			int getCost() const;
			MovementType getMovementType() const;
	};

	class Unit : public MapObject
	{
		protected:
			UnitType *type;
			int hp;
			int mp;
			int sp;
			int attack;
			int defense;

			/** Stupid anti-OOP hack **/
			int dx, dy; //Drawing deltas
		public:
			// Add support for setting color of unit from player`s color.
			// Or just change color of selection frame
			Unit(UnitType *type, Tile *tile, Player *owner);

			//virtual void draw(Graphics::Drawer *target, int x, int y);

			void setDeltas(int dx, int dy);
			int getXDelta() const;
			int getYDelta() const;
			
			/**
			 * Changes unit position unconditionally (unless tile is occypied by enemy
			 * @param newPosition New position
			 * @return true on success
			 */
			bool changePosition(Tile *newPosition);
			
			//void issueOrder(Order *order)

			/**
			 * Moves to adjacent tile
			 * @param tile Tile to move to. It must me one of 8 neighbour tiles
			 * @return True if movement occured
			 */
			bool moveTo(Tile *tile);
	};
}

#endif //UNIT_H
