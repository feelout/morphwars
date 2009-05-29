#ifndef TILE_H
#define TILE_H

#include <SDL/SDL.h>

const int TILE_HEIGHT = 40;
const int TILE_WIDTH = 40;
const int TILE_TERRAIN_HEIGHT = 21;

/**
 * Types of tile images
 */
enum TileImageType
{
	/* Whole tile is filled with surface */
	CENTER = 0,
	/* Used for blending when collided (if priority is higher) */
	LEFT,
	RIGHT,
	UP,
	DOWN,
};

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

const int TILE_IMAGES_NUM = 5;
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
		 * Tile type images
		 */
		SDL_Surface* surfaces[TILE_IMAGES_NUM]; //one for each tile side + center
		/**
		 * Priority is used to show which tiles should blend onto others.
		 */
		int priority;
		/**
		 * Movement costs for moving through tile type;
		 */
		MovementCosts movementCosts;
	public:
		/**
		  * Creates tile type
		  * @param surface Surface, from which tile images are extracted
		  * @param y Line occupied by tile on surface
		  * @param movementCosts Array with movement costs
		  */
		TileType(SDL_Surface *surface, int y, int priority, MovementCosts movementCosts);
		/**
		 * Destroys images
		 */
		~TileType();

		/**
		 * Returt tile image for given image type
		 */
		SDL_Surface *getTileImage(TileImageType type);
};

/**
 * Tile represens a map cell
 */
class Tile
{
	private:
		int x,y; //might be not needed
		SDL_Surface *image;
		TileType *type;
		//Units and building occupying
	public:
		Tile(int x, int y, TileType *type);

		void setImageType(TileImageType imgtype);
		void setImage(SDL_Surface *image);
		
		TileType* getType();
};

#endif //TILE_H
