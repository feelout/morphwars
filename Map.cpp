#include <algorithm>
#include "Map.h"
#include "Logger.h"
#include "Surface.h"
#include "Drawer.h"

using namespace Core;
using namespace Gui;

//const int MAP_MINIMAL_FRAME_WIDTH = 400;
//const int MAP_MINIMAL_FRAME_HEIGHT = 300;

bool CompareTiles(Tile *t1, Tile *t2)
{
	if((!t1) || (!t2))
	{
		return false;
	}
	//printf("CompareTiles: (%i,%i) vs (%i,%i)\n", t1->getX(), t1->getY(), t2->getX(), t2->getY());
	return t1->getType()->getPriority() > t2->getType()->getPriority();
}

Map::Map(Rect frame, int width, int height, std::string tilesetname, Widget *parent)
	: Widget(frame, parent), width(width), height(height), cached(NULL), clip(0,0,0,0), maxTileHeight(1)
{
	tiles = new Tile*[width*height];
	tileset = new TileSet(tilesetname);

	for(int y=0; y < height; ++y)
	{
		for(int x=0; x < width; ++x)
		{
			tiles[x+y*width] = new Tile(x, y, 1, tileset->getType(0));
			tiles[x+y*width]->setImage(tiles[x+y*width]->getType()->getTileImage(CENTER));
		}
	}

	//width+1 because of tile shift
	cached = new Graphics::Surface((width+1)*TILE_WIDTH, height*TILE_HEIGHT+TILE_HEIGHT_OFFSET*maxTileHeight);
	//lastFov = new FieldOfView(width, height);
	currentFov = new FieldOfView(width, height);

	//requestedFrame = Rect(0, 0, MAP_MINIMAL_FRAME_WIDTH, MAP_MINIMAL_FRAME_HEIGHT); 
	recalculateClipping(); //TODO: Check if Widget constructor calls frameChanged(). If so, delete this
}

Map::Map(Rect frame, TiXmlElement *xmlmap, Widget *parent)
	: Widget(frame, parent), cached(NULL), clip(0,0,0,0), maxTileHeight(1)
{
	xmlmap->QueryIntAttribute("width", &width);
	xmlmap->QueryIntAttribute("height", &height);

	Utility::Logger::getInstance()->log("Map dimensions: %ix%i\n", width, height);

	tileset = new TileSet(xmlmap->Attribute("tileset"));
	//printf("tiles = new Tile*[%i]\n", width * height);
	tiles = new Tile*[width*height];

	TiXmlNode *row=NULL, *cell=NULL;
	int x=0, y=0;
	int height;
	std::string cliff;

	while(row = xmlmap->IterateChildren("row", row))
	{
		//FIXME: atoi(cell->Value()) is somehow always zero
		while(cell = row->IterateChildren("cell", cell))
		{
			cell->ToElement()->QueryIntAttribute("height", &height);
			tiles[x+y*width] = new Tile(x, y, height, tileset->getType(atoi(cell->ToElement()->GetText())));

			if(cell->ToElement()->QueryStringAttribute("cliff", &cliff) == TIXML_SUCCESS)
				tiles[x+y*width]->setCliffDirection(Tile::StringToDirection(cliff));

			maxTileHeight = std::max(tiles[x+y*width]->getHeight(), maxTileHeight);
			++x;
		}
		x=0;
		++y;
	}

	calculateSurfaces();
	cached = new Graphics::Surface((width+1)*TILE_WIDTH, (height+1)*TILE_HEIGHT+
			TILE_HEIGHT_LEVEL_OFFSET*maxTileHeight);
	//lastFov = new FieldOfView(width, height); 
	currentFov = new FieldOfView(width, height);

	//requestedFrame = Rect(0, 0, MAP_MINIMAL_FRAME_WIDTH, MAP_MINIMAL_FRAME_HEIGHT); 
	//setFrame(frame);
	//
	recalculateClipping(); // TODO: same as in previous constructor
}

void Map::recalculateClipping()
{
	clip.w = frame.w / TILE_WIDTH;
	clip.h = frame.h / TILE_HEIGHT;
}

void Map::calculateSurfaces()
{
	Utility::Logger::getInstance()->log("Calculating map surfaces.\n");

	for(int y=0; y < height; ++y)
	{
		for(int x=0; x < width; ++x)
		{
			Tile *currentTile = getTile(x,y);

			if(currentTile->isCliff())
			{
				currentTile->setImage(currentTile->getType()->
							getCliffImage(currentTile->getCliffDirection()));
				continue;
			}
			//TODO: Cache instead of producting unneeded copies
			/** Check neighbours **/
			Graphics::Surface *tilesurf = new Graphics::Surface(TILE_WIDTH, TILE_HEIGHT);

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
				if( (*i)->getType()->getPriority() > currentTilePriority &&
						(*i)->getHeight() >= currentTile->getHeight() )
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
	delete cached;
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

void Map::frameUpdated()
{
	recalculateClipping();

	Graphics::Drawer drawer(cached);
	drawer.fillRect(Rect(0, 0, cached->getWidth(), cached->getHeight()), RGBColor::BLACK);
}

/*void Map::setFrame(Rect frame)
{
	this->frame = frame;
	recalculateClipping();
}*/

/*Rect Map::getFrame() const
{
	return frame;
}*/

void Map::setPointOfView(int x, int y)
{
	clip.x = x;
	clip.y = y;
}

void Map::setClipping(Rect clip)
{
	this->clip = clip;
}

Rect Map::getClipping() const
{
	return clip;
}

void Map::setFieldOfView(FieldOfView *fov)
{
	//if(!(*currentFov == *fov))
	if(currentFov != fov || currentFov->isDirty())
	{
		//WriteToLog("Map : got new FOV.\n");
		//*currentFov = *fov;
		currentFov = fov;
		/*currentFov->copyFromOther(fov);
		for(int x=0; x < width; ++x)
		{
			for(int y=0; y < height; ++y)
			{
				if(currentFov->isTileVisible(x,y) != fov->isTileVisible(x,y))
					WriteToLog("ACTUNG!\n");
			}
		}*/
		updateCache();
		currentFov->setDirty(false);
	}
}

int Map::getMaxTileHeight() const
{
	return maxTileHeight;
}

void Map::updateCache()
{
	//Utility::Logger::getInstance()->log("Rebuilding map cache\n");

	int dx = 0, dy = 0;

	int max_tilex = (width < clip.w) ? width : (clip.x + clip.w);
	int max_tiley = (height < clip.h) ? height : (clip.y + clip.h);
	//Utility::Logger::getInstance()->log("max_tilex = %i, max_tiley = %i\n", max_tilex, max_tiley);
	//Utility::Logger::getInstance()->log("Clipping rect: %i, %i, %i, %i\n", clip.x, clip.y, clip.w, clip.h);
	//Utility::Logger::getInstance()->log("Max tile height : %i\n", maxTileHeight);

	for(int tiley=clip.y; tiley < max_tiley; ++tiley)
	{
		dx = (tiley % 2) * TILE_WIDTH/2;
		dy = TILE_HEIGHT - TILE_TERRAIN_HEIGHT - TILE_HEIGHT_LEVEL_OFFSET * maxTileHeight;

		for(int tilex=clip.x; tilex < max_tilex; ++tilex)
		{
			getTile(tilex, tiley)->draw(cached, frame.x + dx+(tilex*TILE_WIDTH),
					frame.y + (tiley*(TILE_HEIGHT_OFFSET)-dy), currentFov->isTileVisible(tilex, tiley));
		}
	}
}

void Map::draw(Graphics::Surface *target)
{
	//WriteToLog("Map::draw\n");
	//Utility::Logger::getInstance()->log("Frame : (%i,%i)\n", frame.w, frame.h);
	//FIXME: Maybe create cache to (0,0) and blit to real coordinates
	/*cached->blit(target, 0, 0);
	Graphics::Drawer(target).drawRect(frame, RGBColor::WHITE);*/
	draw(target, currentFov, true);
}

void Map::draw(Graphics::Surface *target, FieldOfView *fov, bool drawframe)
{
	int dx=0,dy=0;
	
	cached->blit(target, 0, TILE_HEIGHT_LEVEL_OFFSET*maxTileHeight);

	if(drawframe)
	{
		Graphics::Drawer(target).drawRect(frame, RGBColor::WHITE);
	}
}

Tile* Map::getTileByMouseCoords(int mx, int my)
{
	Graphics::Surface coordFinder("Gfx/CoordFinder.png");

	int rectX = (mx-frame.x) / coordFinder.getWidth();
	int rectY = (my-frame.y-(maxTileHeight+1)*TILE_HEIGHT_LEVEL_OFFSET) / coordFinder.getHeight() * 2;

	int cf_x = (mx-frame.x) % coordFinder.getWidth();
	int cf_y = (my-frame.y-(maxTileHeight+1)*TILE_HEIGHT_LEVEL_OFFSET) % coordFinder.getHeight();

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

	/* Handle shifting */
	tilex += clip.x;
	tiley += clip.y;

	if( (tilex > width) || (tilex < 0) || (tiley > height) || (tiley < 0) )
	{
		//Utility::Logger::getInstance()->log("Map::getTileByMouseCoords : clicked out of bounds");
		return NULL;
	}

	return getTile(tilex, tiley);
}
