#include "TileSet.h"
#include "Logger.h"
#include <cstdio>
#include "Surface.h"

using namespace Core;

TileSet::TileSet(std::string name)
{
	if(loadTileSet(name))
		Utility::Logger::getInstance()->log("Tileset %s loaded\n", name.c_str());
	else
		Utility::Logger::getInstance()->log("Couldn`t load Tileset %s\n", name.c_str());
}

bool TileSet::loadTileSet(std::string name)
{
	FILE *f_def = fopen( ("Tilesets/" + name + "/" + name + ".def").c_str(), "r");

	if(!f_def)
	{
		Utility::Logger::getInstance()->log("Definition file for %s not found.\n", name.c_str());
		Utility::Logger::getInstance()->log("Was searching for %s\n", ("Tilesets/" + name + "/" + name + ".def").c_str());
		return false;
	}

	//SDL_Surface *f_map = IMG_Load(("Tilesets/" + name + "/" + name + ".png").c_str());
	Graphics::Surface *f_map = new Graphics::Surface(("Tilesets/" + name + "/" + name + ".png").c_str());

	if(!f_map)
	{
		Utility::Logger::getInstance()->log("Couldn`t load image for Tileset %s\n", name.c_str());
		Utility::Logger::getInstance()->log("Was searching for %s\n", ("Tilesets/" + name + "/" + name + ".png").c_str());
		return false;
	}

	int numOfTileTypes = f_map->getHeight() / TILE_HEIGHT;

	Utility::Logger::getInstance()->log("%i tile types detected\n", numOfTileTypes);

	TileType *currentType;

	for(int y=0; y < numOfTileTypes+1; ++y)
	{
		int *movementCosts = new int[MOVEMENT_TYPES_NUM];
		MovementCosts costs;
		int priority;
		fscanf(f_def, "%i,%i,%i,%i,%i,%i\n", &priority, &costs.costs[FEET], &costs.costs[WHEEL],
				&costs.costs[TRACK], &costs.costs[LOWAIR], &costs.costs[HIGHAIR]);
		currentType = new TileType(f_map, y, priority, costs);

		types.push_back(currentType);
	}

	fclose(f_def);
	delete f_map;

	return true;
}

TileType* TileSet::getType(int n) const
{
	return types[n];
}

TileSet::~TileSet()
{
	/*std::vector<TileType*>::iterator i;

	for(i = types.begin(); i != types.end(); i++)
	{
		delete (*i);
	}
	*/
	types.erase(types.begin(), types.end());
}
