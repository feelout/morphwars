#include "TileSet.h"
#include <cstdio>
#include <SDL/SDL.h>

TileSet::TileSet(std::string name)
{
	if(loadTileSet(name))
		Logger::getInstance()->log("Tileset %s loaded\n", name.c_str());
	else
		Logger::getInstance()->log("Couldn`t load Tileset %s\n", name.c_str());
}

bool TileSet::loadTileSet(std::string name)
{
	FILE *f_def = fopen( ("/Tilesets/" + name + ".def").c_str(), "r");

	if(!f_def)
	{
		Logger::getInstance()->log("Definition file for %s not found.\n", name.c_str());
		return false;
	}

	SDL_Surface *f_map = IMG_Load(("/Tilesets/" + name + ".png").c_str());

	if(!f_map)
	{
		Logger::getInstance()->log("Couldn`t load image for Tileset %s\n", name.c_str());
		return false;
	}

	int numOfTileTypes = f_map->h / TILE_Y;

	Logger::getInstance()->log("%i tile types detected\n", numOfTileTypes);

	TileType *currentType;

	for(int y=0; y < numOfTileTypes; ++y)
	{
		int *movementCosts = new int[MOVEMENT_TYPES_NUM];
		int priority;
		fscanf(f_def, "%i,%i,%i,%i,%i,%i\n", &priority, &movementCosts[FEET], *movementCosts[WHEEL],
				&movementCosts[TRACK], &movementCosts[LOWAIR], &movementCosts[HIGHAIR]);
		currentType = new TileType(f_map, y, priority, movementCosts);

		types.push_back(currentType);
	}

	fclose(f_def);
	SDL_FreeSurface(f_map);
}

TileSet::~TileSet()
{
	std::vector<TileType*>::iterator i;

	for(i = types.begin(); i != types.end(); i++)
	{
		delete *i;
	}
}
