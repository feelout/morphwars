#include "Tile.h"
#include "MapObject.h"
#include "Logger.h"
#include "assist.h"

using namespace Core;

const float SHADOW_FACTOR=0.5;

TileType::TileType(Graphics::Surface *src, int y, int priority, MovementCosts movementCosts)
	: priority(priority), movementCosts(movementCosts)
{
	// Rect of tiletype line on tileset
	Rect cuttingRect;
	cuttingRect.x = 0;
	cuttingRect.y = y*TILE_HEIGHT;
	cuttingRect.w = TILE_WIDTH*TILE_IMAGES_NUM;
	cuttingRect.h = TILE_HEIGHT;

	// Getting tiletype line
	Graphics::Surface *mainSurf = new Graphics::Surface(TILE_WIDTH*TILE_IMAGES_NUM, TILE_HEIGHT);
	src->blit(mainSurf, &cuttingRect, 0, 0);
	
	// Splitting line on tile surfaces
	std::vector<Graphics::Surface*> v_surfaces = Graphics::Surface::splitSpriteStrip(mainSurf, TILE_WIDTH, TILE_HEIGHT);

	for(int i=0; i < TILE_IMAGES_NUM; ++i)
	{
		surfaces[i] = v_surfaces[i];
	}

	delete mainSurf; }

TileType::~TileType()
{
	for(int x=0; x < TILE_IMAGES_NUM; ++x)
	{
		delete surfaces[x];
	}
}

Graphics::Surface* TileType::getTileImage(Direction type) const
{
	return surfaces[type];
}

int TileType::getPriority() const
{
	return priority;
}

int TileType::getMovementCost(MovementType type) const
{
	return movementCosts.costs[type];
}

Tile::Tile(int x, int y, TileType *type)
	: x(x), y(y), type(type), topobject(NULL)
{
	//printf("Tile (%i,%i) created\n", x, y);
	image = type->getTileImage(CENTER);
}

/*void Tile::setImageType(TileImageType imgtype)
{
	image = type->getTileImage(imgtype);
}*/

void Tile::setImage(Graphics::Surface *image)
{
	this->image = image;
	this->shadowedImage = Graphics::Surface::createShadowedSurface(image, SHADOW_FACTOR);
}

TileType* Tile::getType() const
{
	return type;
}

Direction Tile::getDirection(Tile *dst) const
{
	return Tile::getDirection(std::make_pair(x,y), std::make_pair(dst->x, dst->y));
}

Direction Tile::getDirection(std::pair<int, int> src, std::pair<int, int> dst)
{
	int dx = dst.first - src.first;
	int dy = dst.second - src.second;

	bool src_shifted = (src.second % 2) == 1;
	bool dst_shifted = (dst.second % 2) == 1;

	if(dy == 0)
	{
		if(dx > 0) return EAST;
		else if(dx < 0) return WEST;
		else return CENTER;
	}

	if(src_shifted == dst_shifted)
	{
		if(dy > 0)
		{
			if(dx > 0) return SOUTHEAST;
			else if(dx < 0) return SOUTHWEST;
			else return SOUTH;
		}
		else
		{
			if(dx > 0) return NORTHEAST;
			else if(dx < 0) return NORTHWEST;
			else return NORTH;
		}
	}
	else
	{
		if(dy > 0)
		{
			if(!src_shifted)
			{
				if(dx < 0) return SOUTHWEST;
				else return SOUTHEAST;
			}
			else
			{
				if(dx > 0) return SOUTHEAST;
				else return SOUTHWEST;
			}
		}
		else
		{
			if(!src_shifted)
			{
				if(dx < 0) return NORTHWEST;
				else return NORTHEAST;
			}
			else
			{
				if(dx > 0) return NORTHEAST;
				else return NORTHWEST;
			}
		}
	}
}

int Tile::getDistance(Tile *dst) const //FIXME, check from (1,1) to (2,3)
{
	if(this == dst)
		return 0;

	int distance = 0;
	std::pair<int, int> dest = std::make_pair(dst->getX(), dst->getY());
	std::pair<int, int> current = std::make_pair(x, y);
	Direction facing;

	while(current != dest)
	{
		facing = Tile::getDirection(current, dest);
		current = Tile::translateCoordinates(current.first, current.second, facing);
		++distance;
	}

	return distance;
}

bool Tile::addObject(MapObject *object)
{
	if( (objects.empty()) || (!isEnemy(object)) )
	{
		objects.push_back(object);
		//TODO: Sort objects, buildings first
		topobject = object;

		Utility::Logger::getInstance()->log("Total objects on tile (%i,%i): %i\n", x, y, objects.size());

		return true;
	}
	else
	{
		return false;
	}
}

void Tile::removeObject(MapObject *object)
{
	std::vector<MapObject*>::iterator i;

	for(i = objects.begin(); i != objects.end(); ++i)
	{
		if((*i) == object)
		{
			objects.erase(i);
			return;
		}
	}
}

MapObject* Tile::getTopObject() const
{
	return topobject;
}

bool Tile::isEnemy(MapObject *object) const
{
	if(objects.size() != 0)
	{
		return (object->isEnemy(objects[0]));
	}

	return false;
}

int Tile::getX() const
{
	return x;
}

int Tile::getY() const
{
	return y;
}

//Returns 4 neighbours
std::vector< std::pair<int, int> > Tile::getDiagonalNeighbours() const
{
	std::vector< std::pair<int, int> > result;

	int additionalDx = (y % 2 == 0) ? -1 : 0;

	for(int dx=additionalDx; dx < 2+additionalDx; ++dx)
	{
		for(int dy=-1; dy < 2; dy+=2)
		{
			result.push_back(std::make_pair(x+dx, y+dy));
		}
	}

	return result;
}

std::vector< std::pair<int, int> > Tile::getNeighbours() const
{
	using std::make_pair;

	std::vector< std::pair<int, int> > results;

	results.push_back(make_pair(x, y-2));
	results.push_back(make_pair(x, y-1));
	results.push_back(make_pair(x, y+1));
	results.push_back(make_pair(x, y+2));

	if(y % 2 == 0)
	{
		results.push_back(make_pair(x-1, y-1));
		results.push_back(make_pair(x-1, y));
		results.push_back(make_pair(x-1, y+1));
		results.push_back(make_pair(x+1, y));
	}
	else
	{
		results.push_back(make_pair(x-1, y));
		results.push_back(make_pair(x+1, y-1));
		results.push_back(make_pair(x+1, y));
		results.push_back(make_pair(x+1, y+1));
	}

	return results;
}

void Tile::draw(Graphics::Drawer *target, int x, int y, bool visible)
{
	if(visible)
	{
		image->blit(target->getTarget(), x, y);
		for(std::vector<MapObject*>::iterator i = objects.begin(); i != objects.end(); ++i)
		{
			(*i)->draw(target, x, y);
		}
	}
	else
	{
		shadowedImage->blit(target->getTarget(), x, y);
	}
	// Drawing grid
	RGBColor gridcolor(255, 255, 255);
	target->drawLine(x, y + TILE_HEIGHT - TILE_HEIGHT_OFFSET, x + TILE_WIDTH / 2, y + TILE_HEIGHT - TILE_TERRAIN_HEIGHT,
			gridcolor);
	target->drawLine(x + TILE_WIDTH / 2, y + TILE_HEIGHT - TILE_TERRAIN_HEIGHT,
			x + TILE_WIDTH, y + TILE_HEIGHT - TILE_HEIGHT_OFFSET, gridcolor);
	target->drawLine(x + TILE_WIDTH, y + TILE_HEIGHT - TILE_HEIGHT_OFFSET,
			x + TILE_WIDTH / 2, y + TILE_HEIGHT, gridcolor);
	target->drawLine(x + TILE_WIDTH / 2, y +TILE_HEIGHT, x, y +TILE_HEIGHT - TILE_HEIGHT_OFFSET, gridcolor);
}

std::pair<int, int> Tile::translateCoordinates(int x, int y, Direction direction)
{
	bool shifted = (y % 2) == 1;

	switch(direction)
	{
		case CENTER:
			return std::make_pair(x, y); 
		case NORTH:
			return std::make_pair(x, y-2);
		case EAST:
			return std::make_pair(x+1, y);
		case SOUTH:
			return std::make_pair(x, y+2);
		case WEST:
			return std::make_pair(x-1, y);
		case NORTHWEST:
			if(shifted)
				return std::make_pair(x, y-1);
			else
				return std::make_pair(x-1, y-1);
		case NORTHEAST:
			if(shifted)
				return std::make_pair(x+1, y-1);
			else
				return std::make_pair(x, y-1);
		case SOUTHEAST:
			if(shifted)
				return std::make_pair(x+1, y+1);
			else
				return std::make_pair(x, y+1);
		case SOUTHWEST:
			if(shifted)
				return std::make_pair(x, y+1);
			else
				return std::make_pair(x-1, y+1);
	}
}
