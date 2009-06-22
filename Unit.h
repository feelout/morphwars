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
			virtual ~MapObjectType();

			virtual UnitType* clone();

			int getMaxHP() const;
			int getMaxMP() const;
			int getMaxSP() const;
			int getAttack() const;
			int getDefense() const;
			int getCose() const;
			MovementType movementType() const;
	};

	class Unit : public MapObject
	{
		protected:
			/* Hit points */
			int maxhp;
			int hp;
			/* Movement points */
			int maxmp;
			int mp;
			/* Skill points */
			int maxsp;
			int sp;
			/* Attack */
			int attack;
			/* Defense */
			int defense;
		public:
			Unit(UnitType *type, Tile *tile, Player *owner);
	};
}

#endif //UNIT_H
