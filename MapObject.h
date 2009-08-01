#ifndef MAPOBJECT_H
#define MAPOBJECT_H

#include <map>
#include <string>
#include "Tile.h"
#include "Drawer.h"
#include "AnimationPack.h"

namespace Core
{
	class Player; //FIXME: Write player
	/**
	 * Prototype for map objects. To be subclassed.
	 */
	class MapObjectType
	{
		protected:
			Graphics::AnimationPack graphics;
			bool enabled;
			std::string type;

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
			void draw(Graphics::Drawer *target, int x, int y);

			Tile *getTile() const;
			Player *getOwner() const;
			MapObjectType *getType() const;

			bool isEnemy(MapObject *other) const;

	};
}

#endif // MAPOBJECT_H
