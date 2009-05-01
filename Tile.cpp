#include "Tile.h"

TileType::TileType(SDL_Surface *surface, int y, int priority, int *movementCosts)
	: priority(priority), movementCosts(movementCosts);
{
	int rmask = surface->format->Rmask;
	int gmask = surface->format->Gmask;
	int bmask = surface->format->Bmask;
	int amask = surface->format->Amask;

	SDL_Rect targetRect, cuttingRect;
	targetRect.x = 0;
	targetRect.y = 0;
	targetRect.w = TILE_WIDTH;
	targetRect.h = TILE_HEIGHT;

	for(int x=0; x < TILE_IMAGES_NUM; ++x)
	{
		surfaces[i] = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCCOLORKEY, TILE_WIDTH, TILE_HEIGHT, 32,
			rmask, gmask, bmask, amask);

		cuttingRect.x = x*TILE_WIDTH;
		cuttingRect.y = y*TILE_HEIGHT;
		cuttingRect.w = TILE_WIDTH;
		cuttingRect.h = TILE_HEIGHT;

		SDL_BlitSurface(surface, cuttingRect, surfaces[i], targetRect);
	}
}

TileType::~TileType()
{
	for(int x=0; x < TILE_IMAGES_NUM; ++x)
	{
		SDL_FreeSurface(surfaces[i]);
	}

	delete[] surfaces;
	delete[] movementCosts;
}

SDL_Surface* getTileImage(TileImageType type)
{
	return surfaces[type];
}


Tile::Tile(int x, int y, SDL_Surface *image, int **mpcosts)
	: x(x), y(y), image(image), mpcosts(mpcosts);
{
}
