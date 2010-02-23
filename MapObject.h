#ifndef MAPOBJECT_H
#define MAPOBJECT_H

#include <map>
#include <string>
#include "Tile.h"
#include "Surface.h"
#include "AnimationPack.h"

namespace Core
{
	class Player;
	class Map;
	/**
	 * Prototype for map objects. To be subclassed.
	 */
	class MapObjectType
	{
		protected:
			Graphics::AnimationPack graphics;
			bool enabled;
			std::string type;
			std::string name;

			MapObjectType();
		public:
			/**
			 * Creates an object type from file
			 * @param name Object type name
			 */
			MapObjectType(std::string name, std::string type);
			virtual ~MapObjectType()=0;

			/**
			 * Returns graphics of type
			 * @return Type graphics
			 */
			Graphics::AnimationPack* getGraphics();

			/**
			 * Copies itself
			 * @return Clone of instance
			 */
			virtual MapObjectType* clone()=0;

			bool isEnabled() const;
			void setEnabled(bool enabled);

			// Big hole in the architecture
			std::string getType() const;

			std::string getName() const;
	};
	/**
	 * MapObject is drawable object on the map
	 */
	class MapObject
	{
		protected:
			/**
			 * Object type
			 */
			MapObjectType *type;
			/**
			 * Tile occypied by object
			 */
			Tile *tile;
			/**
			 * Owner of an object
			 */
			Player *owner;

			float dx,dy;

			bool dead;
		public:
			/**
			 * Main public constructor
			 * @param type Object type
			 * @param tile Tile, on which object will reside
			 * @param owner Owner of an object
			 */
			MapObject(MapObjectType *type, Tile *tile, Player *owner);
			virtual ~MapObject()=0;

			/**
			 * Draws object
			 * @param target
			 */
			void draw(Graphics::Surface *target, int x, int y);

			virtual void drawInfoPanel(Graphics::Surface *target, int x, int y)=0;

			Tile *getTile() const;
			Player *getOwner() const;
			MapObjectType *getType() const;

			bool isEnemy(MapObject *other) const;
			virtual bool damage(int damage, MapObject *source)=0;

			virtual void defaultTargetOrder(Tile *target, Map *map)=0;
	};
}

#endif // MAPOBJECT_H
