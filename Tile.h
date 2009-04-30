#ifndef TILE_H
#define TILE_H

#include <SDL/SDL.h>

const int TILE_X = 40;
const int TILE_Y = 40;
const int TILE_TERRAIN_HEIGHT = 21;

enum TileImagesType
{
	CENTER = 0,
	LEFT,
	RIGHT,
	UP,
	DOWN,
};

class TileType
{
	private:
		SDL_Surface[5] surfaces; //one for each tile side + center
		int priority;
	public:
		TileType(SDL_Surface *surfaces, int priority);
		SDL_Surface *getTileImage(TileImageType type);
};

class Tile
{
	private:
		int x,y; //might be not needed
		SDL_Surface *image;
		//Units and building occupying
	public:
		Tile(int x, int y, SDL_Surface *image);
};

#endif //TILE_H
