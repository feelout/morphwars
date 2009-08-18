#include <algorithm>
#include "Map.h"
#include "Logger.h"
#include "Surface.h"

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

			std::vector< std::pair<int, int> > neighbourPairs = currentTile->getDiagonalNeighbours();
			std::vector< std::pair<int, int> >::iterator nb_iter;

			for(nb_iter = neighbourPairs.begin(); nb_iter != neighbourPairs.end(); ++nb_iter)
			{
				if(getTile(nb_iter->first, nb_iter->second))
				{
					neighbours.push_back(getTile(nb_iter->first, nb_iter->second));
				}
			}

			//printf("Neighbours: %i\n", neighbours.size());

			//ENDDEBUG

			/** Sort neighbours by priority **/
			std::sort(neighbours.begin(), neighbours.end(), CompareTiles);

			//printf("Blitting neighbour surfaces for (%i,%i)\n", x, y);
			/** Blit neighbour surfaces **/
			for(std::vector<Tile*>::iterator i=neighbours.begin(); i != neighbours.end(); ++i)
			{
				//printf("Current tile priority: %i\tNeighbour priority: %i\n", currentTilePriority,
						//(*i)->getType()->getPriority());
				if((*i)->getType()->getPriority() > currentTilePriority)
				{
					//printf("Blitting neighbour surfaces for (%i,%i)\n", x, y);
					Direction facing = (*i)->getDirection(currentTile);
					//Direction facing = currentTile->getDirection((*i));

					//printf("\tTile: (%i,%i), Facing: %i\n", (*i)->getX(), (*i)->getY(), facing);

					(*i)->getType()->getTileImage(facing)->blit(tilesurf, 0, 0);
				} }

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

int Map::getWidth() const
{
	return width;
}

int Map::getHeight() const
{
	return height;
}

void Map::draw(Graphics::Drawer *target, int x, int y, FieldOfView *fov)
{
	int dx=0,dy=0;
	
	for(int tiley=0; tiley < height; ++tiley)
	{
		dx = (tiley % 2) * TILE_WIDTH/2;
		dy = TILE_HEIGHT - TILE_TERRAIN_HEIGHT;

		//printf("Row %i: shift: %i, height: %i\n", tiley, dy, tiley*(TILE_TERRAIN_HEIGHT)-dy);

		for(int tilex=0; tilex < width; ++tilex)
		{
			getTile(tilex, tiley)->draw(target, x+dx+(tilex*TILE_WIDTH),
					y+(tiley*(TILE_HEIGHT_OFFSET)-dy), fov->isTileVisible(tilex, tiley));
		}
	}
}

Tile* Map::getTileByMouseCoords(int mx, int my, int dx, int dy)
{
	Graphics::Surface coordFinder("Gfx/CoordFinder.png");

	int rectX = (mx-dx) / coordFinder.getWidth();
	int rectY = (my-dy) / coordFinder.getHeight() * 2;

	/*if(rectY % 2 == 1)
	{
		++rectX;
	}*/
	int cf_x = (mx-dx) % coordFinder.getWidth();
	int cf_y = (my-dy) % coordFinder.getHeight();

	//Utility::Logger::getInstance()->log("%i, %i, %i, %i\n", rectX, rectY, cf_x, cf_y);

	int tilex, tiley;

	Graphics::Drawer peeker(&coordFinder);
	RGBColor color = peeker.getPixel(cf_x, cf_y);

	if(color == RGBColor::WHITE)
	{
		tilex = rectX;
		tiley = rectY;
	}
	else if(color == RGBColor::RED)
	{
		tilex = rectX-1;
		tiley = rectY-1;
	}
	else if(color == RGBColor::YELLOW)
	{
		tilex = rectX;
		tiley = rectY-1;
	}
	else if(color == RGBColor::GREEN)
	{
		tilex = rectX-1;
		tiley = rectY+1;
	}
	else if(color == RGBColor::BLUE)
	{
		tilex = rectX;
		tiley = rectY+1;
	}

	if( (tilex > width) || (tilex < 0) || (tiley > height) || (tiley < 0) )
	{
		Utility::Logger::getInstance()->log("Map::getTileByMouseCoords : clicked out of bounds");
		return NULL;
	}

	return getTile(tilex, tiley);
}
