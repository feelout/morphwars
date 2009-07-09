#ifndef TILE_H
#define TILE_H

#include <SDL/SDL.h>
#include "Surface.h"

namespace Core
{
    const int TILE_HEIGHT = 40;
    const int TILE_WIDTH = 40;
    const int TILE_TERRAIN_HEIGHT = 21;

    /**
     * Types of tile images
     */
    enum Direction
    {
        /* Whole tile is filled with surface */
        CENTER = 0,
        /* Used for blending when collided (if priority is higher) */
	NORTHWEST,
	NORTHEAST,
	SOUTHWEST,
	SOUTHEAST,
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
    };

    /**
     * Tile represens a map cell
     */
    class Tile
    {
        private:
            int x,y; //might be not needed
	    Graphics::Surface *image;
            TileType *type;
            //Units and building occupying
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
	    Direction getDirection(Tile *dst);

	    int getX() const;
	    int getY() const;
    };
}

#endif //TILE_H
