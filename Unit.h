#ifndef UNIT_H
#define UNIT_H

#include "MapObject.h"

namespace Core
{
	class UnitType: public MapObjectType
	{
		protected:
			int maxhp; 	// Hit points
			int maxmp;	// Movement points
			int maxsp;	// Skill points
			int attack;	// Attack
			int defense;	// Defense
			int cost;	// Cost in energy
			MovementType movementType;
			
			virtual bool loadFromFile(std::string name);
		public:
			UnitType(std::string name);
			virtual ~UnitType();

			virtual UnitType* clone();

			int getMaxHP() const;
			int getMaxMP() const;
			int getMaxSP() const;
			int getAttack() const;
			int getDefense() const;
			int getCose() const;
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
		public:
			// Add support for setting color of unit from player`s color.
			// Or just change color of selection frame
			Unit(UnitType *type, Tile *tile, Player *owner);
	};
}

#endif //UNIT_H
