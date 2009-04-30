#include "Tile.h"

TileType::TileType(SDL_Surface *surfaces, int priority)
	: surfaces(surfaces), priority(priority)
{
}

SDL_Surface* getTileImage(TileImageType type)
{
	return surfaces[type];
}


Tile::Tile(int x, int y, SDL_Surface *image)
	: x(x), y(y), image(image)
{
}
