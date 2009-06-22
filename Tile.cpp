#include "Tile.h"
#include "assist.h"

using namespace Core;

TileType::TileType(SDL_Surface *surface, int y, int priority, MovementCosts movementCosts)
	: priority(priority), movementCosts(movementCosts)
{
	SDL_Rect targetRect, cuttingRect;
	targetRect.x = 0;
	targetRect.y = 0;
	targetRect.w = TILE_WIDTH;
	targetRect.h = TILE_HEIGHT;

	for(int x=0; x < TILE_IMAGES_NUM; ++x)
	{
		surfaces[x] = createSurface(TILE_WIDTH, TILE_HEIGHT);

		cuttingRect.x = x*TILE_WIDTH;
		cuttingRect.y = y*TILE_HEIGHT;
		cuttingRect.w = TILE_WIDTH;
		cuttingRect.h = TILE_HEIGHT;

		SDL_BlitSurface(surface, &cuttingRect, surfaces[x], &targetRect);
	}
}

TileType::~TileType()
{
	for(int x=0; x < TILE_IMAGES_NUM; ++x)
	{
		SDL_FreeSurface(surfaces[x]);
	}

	delete[] surfaces;
}

SDL_Surface* TileType::getTileImage(TileImageType type)
{
	return surfaces[type];
}

Tile::Tile(int x, int y, TileType *type)
	: x(x), y(y), type(type)
{
	image = type->getTileImage(CENTER);
}

void Tile::setImageType(TileImageType imgtype)
{
	image = type->getTileImage(imgtype);
}

void Tile::setImage(SDL_Surface *image)
{
	this->image = image;
}

TileType* Tile::getType()
{
	return type;
}
