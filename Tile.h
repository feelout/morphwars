#ifndef TILE_H
#define TILE_H

#include <SDL/SDL.h>
#include <utility>
#include "Surface.h"

namespace Core
{
	class MapObject;

	const int TILE_HEIGHT = 60;//40;
	const int TILE_WIDTH = 60;//40;
	const int TILE_TERRAIN_HEIGHT = 30;//21;
	const int TILE_HEIGHT_OFFSET = 15;

	/**
	 * Types of tile images
	 */
	enum Direction
	{
		// Whole tile is filled with surface 
		CENTER = 0,
		//Used for blending when collided (if priority is higher) 
		NORTHWEST,
		NORTH,
		NORTHEAST,
		EAST,
		SOUTHEAST,
		SOUTH,
		SOUTHWEST,
		WEST,
	};

	std::string DirectionToString(Direction direction);

	/*enum Direction
	  {
	  CENTER = 0,
	  SOUTHWEST,
	  SOUTHEAST,
	  NORTHWEST,
	  NORTHEAST,
	  };*/

	// Move to unit?
	/**
	 * Unit movement types
	 */
	enum MovementType
	{
		FEET = 0,
		WHEEL,
		TRACK,
		LOWAIR,
		HIGHAIR,
	};

	const int TILE_IMAGES_NUM = 9;
	const int MOVEMENT_TYPES_NUM = 5;

	struct MovementCosts
	{
		int costs[MOVEMENT_TYPES_NUM];
	};

	/**
	 * Tile type - type of map cell
	 */
	class TileType
	{
		private:
			/**
			 * Priority is used to show which tiles should blend onto others.
			 */
			int priority;
			/**
			 * Movement costs for moving through tile type;
			 */
			MovementCosts movementCosts;

			/**
			 * NEW TILE CLASS
			 */
			Graphics::Surface* surfaces[TILE_IMAGES_NUM]; //one for each tile side + center
		public:
			/**
			 * Creates tile type
			 * @param surface Surface, from which tile images are extracted
			 * @param y Line occupied by tile on surface
			 * @param movementCosts Array with movement costs
			 */
			//TileType(SDL_Surface *surface, int y, int priority, MovementCosts movementCosts);
			TileType(Graphics::Surface *surface, int y, int priority, MovementCosts movementCosts);
			/**
			 * Destroys images
			 */
			~TileType();

			/**
			 * Returt tile image for given image type
			 */
			Graphics::Surface *getTileImage(Direction type) const;

			int getPriority() const;
			int getMovementCost(MovementType type) const;
	};

	/**
	 * Tile represens a map cell
	 */
	class Tile
	{
		private:
			int x,y; //might be not needed
			Graphics::Surface *image;
			Graphics::Surface *shadowedImage;
			TileType *type;
			//Units and building occupying
			std::vector<MapObject*> objects;
			MapObject* topobject;
		public:
			Tile(int x, int y, TileType *type);

			//void setImageType(Direction imgtype);
			void setImage(Graphics::Surface *image);

			TileType* getType() const;

			/**
			 * Calculates direction from this tile to another
			 * @param dst Tile to face
			 * @return Direction
			 */
			Direction getDirection(Tile *dst) const;
			static Direction getDirection(std::pair<int, int> src, std::pair<int, int> dst);
			
			/**
			 * Calculates distance from this tile to another
			 * @param dst Other tile
			 * @return distance
			 */
			int getDistance(Tile *dst) const;

			bool canBeAdded(MapObject *object);
			bool addObject(MapObject *object);
			void removeObject(MapObject *object);
			MapObject* getTopObject() const;

			/**
			 * Check object allegion
			 * @param object Object
			 * @return true if tile objects are on the same force
			 */
			bool isEnemy(MapObject *object) const;

			int getX() const;
			int getY() const;

			/**
			 * Calculates tile neighbours
			 * @return Coordinates of tile neighbours
			 */
			std::vector< std::pair<int,int> > getNeighbours() const;

			std::vector< std::pair<int,int> > getDiagonalNeighbours() const;

			void draw(Graphics::Surface *target, int x, int y, bool visible);

			static std::pair<int, int> translateCoordinates(int x, int y, Direction direction);
			static std::string DirectionToString(Direction direction);
	};
}

#endif //TILE_H
