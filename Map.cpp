#include <algorithm>
#include "Map.h"
#include "Logger.h"
#include "Surface.h"
#include "Drawer.h"

using namespace Core;
using namespace Gui;
using Graphics::Surface;

bool CompareTiles(Tile *t1, Tile *t2)
{
	if((!t1) || (!t2))
	{
		return false;
	}
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
	cached = new Surface((width+1)*TILE_WIDTH, height*TILE_HEIGHT+TILE_HEIGHT_OFFSET*maxTileHeight);
	//lastFov = new FieldOfView(width, height);
	currentFov = new FieldOfView(width, height);

	recalculateClipping(); //TODO: Check if Widget constructor calls frameChanged(). If so, delete this
}

Map::Map(Rect frame, TiXmlElement *xmlmap, Widget *parent)
	: Widget(frame, parent), cached(NULL), clip(0,0,0,0), maxTileHeight(1)
{
	xmlmap->QueryIntAttribute("width", &width);
	xmlmap->QueryIntAttribute("height", &height);

	Utility::Logger::getInstance()->log("Map dimensions: %ix%i\n", width, height);

	tileset = new TileSet(xmlmap->Attribute("tileset"));

	tiles = new Tile*[width*height];

	TiXmlNode *row=NULL, *cell=NULL;
	int x=0, y=0;
	int height;
	std::string cliff;

	while(row = xmlmap->IterateChildren("row", row))
	{
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
	cached = new Surface((width+1)*TILE_WIDTH, (height+1)*TILE_HEIGHT+
			TILE_HEIGHT_LEVEL_OFFSET*maxTileHeight);
	currentFov = new FieldOfView(width, height);

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

			/*if(currentTile->isCliff())
			{
				// FIXME FIXME FIXME FOUND IT TODO TODO TODO
				currentTile->setImage(currentTile->getType()->
							getCliffImage(currentTile->getCliffDirection()));
				continue;
			}*/
			//TODO: Cache instead of producting unneeded copies
			/** Check neighbours **/
			//Surface *tilesurf = new Surface(TILE_WIDTH, TILE_HEIGHT);
			Surface *tilesurf = NULL;

			if(currentTile->isCliff())
				tilesurf = currentTile->getType()->getCliffImage(currentTile->getCliffDirection());
			else
				tilesurf = new Surface(TILE_WIDTH, TILE_HEIGHT);
			/*Surface **leftFillers = new Surface*[currentTile->getHeight()];
			Surface **rightFillers = new Surface*[currentTile->getHeight()];*/

			/* FIXME: Those two should probably use (height-1) as multiplier */
			Surface *leftHeightImage = new Surface(TILE_WIDTH,
					TILE_HEIGHT + currentTile->getHeight() * TILE_HEIGHT_LEVEL_OFFSET);
					//+ currentTile->isCliff() ? TILE_HEIGHT_LEVEL_OFFSET : 0);
			Surface *rightHeightImage = new Surface(TILE_WIDTH,
					TILE_HEIGHT + currentTile->getHeight() * TILE_HEIGHT_LEVEL_OFFSET);
					//+ currentTile->isCliff() ? TILE_HEIGHT_LEVEL_OFFSET : 0);

			// Create initial height fillers, not influenced by neighbours
			for(int i=0, y=leftHeightImage->getHeight(); i < currentTile->getHeight(); ++i, y -= TILE_HEIGHT_LEVEL_OFFSET)
			{
				currentTile->getType()->getLeftHeightFiller()->blit(leftHeightImage, 0, y);	
				currentTile->getType()->getRightHeightFiller()->blit(rightHeightImage, 0, y);	
			}

			if(!currentTile->isCliff())
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

			/** Sort neighbours by priority **/
			std::sort(neighbours.begin(), neighbours.end(), CompareTiles);

			/** Blit neighbour surfaces **/
			for(std::vector<Tile*>::iterator i=neighbours.begin(); i != neighbours.end(); ++i)
			{
				// XXX: Pull up common thing
				// Add terrain transitions
				if( !currentTile->isCliff() && (*i)->getType()->getPriority() > currentTilePriority &&
						( (*i)->getHeight() >= currentTile->getHeight() 
						  || (*i)->getHeight() == currentTile->getHeight()-1
						  && (*i)->getCliffDirection() == currentTile->getDirection((*i)))
				  )
				{
					Direction facing = (*i)->getDirection(currentTile);

					(*i)->getType()->getTileImage(facing)->blit(tilesurf, 0, 0);
				} 
				// Add height transitions
				if( (*i)->getType()->getPriority() > currentTilePriority )
				{
					Direction facing = (*i)->getDirection(currentTile);
					Surface *targetHeightFiller = NULL;
					
					switch(facing)
					{
						case NORTHWEST:
						case SOUTHEAST:
							targetHeightFiller = leftHeightImage;
							break;
						case NORTHEAST:
						case SOUTHWEST:
							targetHeightFiller = rightHeightImage;
							break;
						default:
							continue;
					}

					Surface *transition = (*i)->getType()->getHeightTransition(facing);

					for(int h=0, y=leftHeightImage->getHeight(); h < (*i)->getHeight(); ++h)
					{
						transition->blit(targetHeightFiller, 0, y);
						y -= TILE_HEIGHT_LEVEL_OFFSET;
					}
				}
			}

			currentTile->setImage(tilesurf);
			//XXX HEAVY DEBUG
			if(currentTile->getX() == 3 && currentTile->getY() == 2)
			{
				if(!leftHeightImage)
					WriteToLog("Left height image = NULL\n");
				if(!rightHeightImage)
					WriteToLog("Right heigh image = NULL\n");
			}
			currentTile->setLeftHeightLevelImage(leftHeightImage);
			currentTile->setRightHeightLevelImage(rightHeightImage);
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
	if(currentFov != fov || currentFov->isDirty())
	{
		currentFov = fov;
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
	int dx = 0, dy = 0;

	int max_tilex = (width < clip.w) ? width : (clip.x + clip.w);
	int max_tiley = (height < clip.h) ? height : (clip.y + clip.h);

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

void Map::draw(Surface *target)
{
	draw(target, currentFov, true);
}

void Map::draw(Surface *target, FieldOfView *fov, bool drawframe)
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
	Surface coordFinder("Gfx/CoordFinder.png");

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
