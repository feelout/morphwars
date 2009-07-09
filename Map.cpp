#include <algorithm>
#include "Map.h"
#include "Logger.h"

using namespace Core;

bool CompareTiles(Tile *t1, Tile *t2)
{
	if((!t1) || (!t2))
	{
		return false;
	}
	//printf("CompareTiles: (%i,%i) vs (%i,%i)\n", t1->getX(), t1->getY(), t2->getX(), t2->getY());
	return t1->getType()->getPriority() > t2->getType()->getPriority();
}

Map::Map(int width, int height, std::string tilesetname)
	: width(width), height(height)
{
	tiles = new Tile*[width*height];
	tileset = new TileSet(tilesetname);

	for(int y=0; y < height; ++y)
	{
		for(int x=0; x < width; ++x)
		{
			tiles[x+y*width] = new Tile(x, y, tileset->getType(0));
			tiles[x+y*width]->setImage(tiles[x+y*width]->getType()->getTileImage(CENTER));
		}
	}
}

Map::Map(TiXmlElement *xmlmap)
{
	xmlmap->QueryIntAttribute("width", &width);
	xmlmap->QueryIntAttribute("height", &height);

	Utility::Logger::getInstance()->log("Map dimensions: %ix%i\n", width, height);

	tileset = new TileSet(xmlmap->Attribute("tileset"));
	//printf("tiles = new Tile*[%i]\n", width * height);
	tiles = new Tile*[width*height];

	TiXmlNode *row=NULL, *cell=NULL;
	int x=0, y=0;

	while(row = xmlmap->IterateChildren("row", row))
	{
		//FIXME: atoi(cell->Value()) is somehow always zero
		while(cell = row->IterateChildren("cell", cell))
		{
			tiles[x+y*width] = new Tile(x, y, tileset->getType(atoi(cell->ToElement()->GetText())));
			++x;
		}
		x=0;
		++y;
	}

	calculateSurfaces();
}

void Map::calculateSurfaces()
{
	Utility::Logger::getInstance()->log("Calculating map surfaces.\n");

	for(int y=0; y < height; ++y)
	{
		for(int x=0; x < width; ++x)
		{
			//TODO: Cache instead of producting unneeded copies
			/** Check neighbours **/
			Graphics::Surface *tilesurf = new Graphics::Surface(TILE_WIDTH, TILE_HEIGHT);
			Tile *currentTile = getTile(x,y);
			currentTile->getType()->getTileImage(CENTER)->blit(tilesurf, 0, 0);

			int currentTilePriority = currentTile->getType()->getPriority();

			/** Check neighbours **/
			std::vector<Tile*> neighbours;

			//printf("Checking neighbours of (%i,%i)\n", x, y);

			for(int dx=-1; dx < 2; ++dx)
			{
				for(int dy=-1; dy < 2; ++dy)
				{
					//printf("Checking (%i,%i)\n", x+dx, y+dy);
					if(getTile(x+dx,y+dy))
					{
						//TODO: getTile should return NULL if coordinates are bad
						//printf("Adding (%i,%i) to neighbour list\n", x+dx, y+dy);
						neighbours.push_back(getTile(x+dx, y+dy));
					}
				}
			}

			/** Sort neighbours by priority **/
			std::sort(neighbours.begin(), neighbours.end(), CompareTiles);

			/** Blit neighbour surfaces **/
			for(std::vector<Tile*>::iterator i=neighbours.begin(); i != neighbours.end(); ++i)
			{
				if((*i)->getType()->getPriority() > currentTilePriority)
				{
					Direction facing = (*i)->getDirection(currentTile);

					(*i)->getType()->getTileImage(facing)->blit(tilesurf, 0, 0);
				}
			}

			currentTile->setImage(tilesurf);
		}
	}
}

Map::~Map()
{
	for(int i=0; i < width*height; ++i)
	{
		delete tiles[i];
	}
	delete[] tiles;
	delete tileset;
}

Tile* Map::getTile(int x, int y)
{
	//TODO: Maybe move this to smthing like IsTileValid - if overhead is too high
	if((x < 0) || (y < 0) || (x >= width) || (y >= height))
	{
		//printf("getTile: invalid tile (%i,%i)\n", x, y);
		return NULL;
	}

	return tiles[x+y*width];
}

void Map::draw(Graphics::Drawer *target, int x, int y)
{
	//STUB
		


}
