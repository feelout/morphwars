#include "Tile.h"
#include "MapObject.h"
#include "Logger.h"
#include "Drawer.h"
#include "assist.h"

using namespace Core;

const float SHADOW_FACTOR=0.3; //FIXME: DEBUG!
//const float SHADOW_FACTOR=0.5;


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

	delete mainSurf; 
}

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

Graphics::Surface* TileType::getLeftHeightFiller() const
{
	return surfaces[9];
}

Graphics::Surface* TileType::getRightHeightFiller() const
{
	return surfaces[10];
}

Graphics::Surface* TileType::getLeftHeightTransition() const
{
	return surfaces[11];
}

Graphics::Surface* TileType::getRightHeightTransition() const
{
	return surfaces[12];
}

Graphics::Surface* TileType::getCliffImage(Direction direction) const
{
	switch(direction)
	{
		case SOUTHWEST:
			return surfaces[13];
		case NORTHWEST:
			return surfaces[14];
		case NORTHEAST:
			return surfaces[15];
		case SOUTHEAST:
			return surfaces[16];
		default:
			Utility::Logger::getInstance()->log("Invalid cliff direction\n");
			return NULL;
	}
}

int TileType::getPriority() const
{
	return priority;
}

int TileType::getMovementCost(MovementType type) const
{
	return movementCosts.costs[type];
}

Tile::Tile(int x, int y, int height, TileType *type)
	: x(x), y(y), height(height), type(type), topobject(NULL), cliff(CENTER)
{
	//printf("Tile (%i,%i) created\n", x, y);
	image = type->getTileImage(CENTER);
	leftHeightLevelImages = new Graphics::Surface*[height];
	rightHeightLevelImages = new Graphics::Surface*[height];
	std::fill(leftHeightLevelImages, leftHeightLevelImages + height, type->getLeftHeightFiller());
	std::fill(rightHeightLevelImages, rightHeightLevelImages + height, type->getRightHeightFiller());
}

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

	/*Utility::Logger::getInstance()->log("Distance from (%i,%i) to (%i,%i) = %i\n", x, y,
			dst->x, dst->y, distance);*/
	return distance;
}

bool Tile::canBeAdded(const MapObject *object) const
{
	return ( (objects.empty()) || (!isEnemy(object)) );
}

bool Tile::addObject(MapObject *object)
{
	if(canBeAdded(object))
	{
		objects.push_back(object);
		//TODO: Sort objects, buildings first
		topobject = object;

		//Utility::Logger::getInstance()->log("Total objects on tile (%i,%i): %i\n", x, y, objects.size());

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

bool Tile::isEnemy(const MapObject *object) const
{
	//Utility::Logger::getInstance()->log("(%i,%i)->isEnemy()\n", x, y);
	if(objects.size() != 0)
	{
		if(object->isEnemy(objects[0]))
				Utility::Logger::getInstance()->log("Enemy!!!\n");
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

int Tile::getHeight() const
{
	return height;
}

/*int Tile::getTopY() const
{
	return topY;
}*/

std::pair<int, int> Tile::getTopCoords() const
{
	return topCoords;
}

bool Tile::isCliff() const
{
	return cliff != CENTER;
}

Direction Tile::getCliffDirection() const
{
	return cliff;
}

void Tile::setCliffDirection(Direction direction)
{
	cliff = direction;
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

void Tile::draw(Graphics::Surface *target, int x, int y, bool visible)
{
	//Utility::Logger::getInstance()->log("Tile(%i,%i)::draw(%i,%i)\n", this->x , this->y, x, y);
	for(int h=0; h < height; ++h)
	{
		//getType()->getLeftHeightFiller()->blit(target, x, y);
		//getType()->getRightHeightFiller()->blit(target, x, y);
		// FIXME : Change 0 to h
		leftHeightLevelImages[0]->blit(target, x, y);
		rightHeightLevelImages[0]->blit(target, x, y);
		y -= TILE_HEIGHT_LEVEL_OFFSET;
	}
	//Utility::Logger::getInstance()->log("Tile(%i,%i) : final top y = %i\n", this->x, this->y, y);
	//topY = y;
	topCoords = std::make_pair(x, y);
	if(visible)
	{
		image->blit(target, x, y);
		/*for(std::vector<MapObject*>::iterator i = objects.begin(); i != objects.end(); ++i)
		{
			(*i)->draw(target, x, y);
		}*/ //Can`t use because of map caching
		//Utility::Logger::getInstance()->log("Drawing visible tile (%i,%i)\n", this->x, this->y);
	}
	else
	{
		shadowedImage->blit(target, x, y);
	}
	// Drawing grid
	Graphics::Drawer drawer(target);
	RGBColor gridcolor(255, 255, 255);
	if(!isCliff()) 
	{
		drawer.drawLine(x, y + TILE_HEIGHT - TILE_HEIGHT_OFFSET, x + TILE_WIDTH / 2, y + TILE_HEIGHT - TILE_TERRAIN_HEIGHT,
				gridcolor);
		drawer.drawLine(x + TILE_WIDTH / 2, y + TILE_HEIGHT - TILE_TERRAIN_HEIGHT,
				x + TILE_WIDTH, y + TILE_HEIGHT - TILE_HEIGHT_OFFSET, gridcolor);
		drawer.drawLine(x + TILE_WIDTH, y + TILE_HEIGHT - TILE_HEIGHT_OFFSET,
				x + TILE_WIDTH / 2, y + TILE_HEIGHT, gridcolor);
		drawer.drawLine(x + TILE_WIDTH / 2, y +TILE_HEIGHT, x, y +TILE_HEIGHT - TILE_HEIGHT_OFFSET, gridcolor);
	}
	// Drawing height borders
	drawer.drawLine(x, y + TILE_HEIGHT - TILE_HEIGHT_OFFSET, x,
				y + TILE_HEIGHT - TILE_HEIGHT_OFFSET + height * TILE_HEIGHT_LEVEL_OFFSET,
				gridcolor);
	drawer.drawLine(x + TILE_WIDTH, y + TILE_HEIGHT - TILE_HEIGHT_OFFSET, x + TILE_WIDTH,
				y + TILE_HEIGHT - TILE_HEIGHT_OFFSET + height * TILE_HEIGHT_LEVEL_OFFSET,
				gridcolor);
	drawer.drawLine(x + TILE_WIDTH / 2, y + TILE_HEIGHT, x + TILE_WIDTH / 2,
			y + TILE_HEIGHT + height * TILE_HEIGHT_LEVEL_OFFSET, gridcolor);
	// Drawing 
	std::string coords = "(" + br_itoa(this->x) + "," + br_itoa(this->y) + ")";
	/*Graphics::FontSystem::getInstance()->print(target, coords, x,
			y + TILE_HEIGHT - TILE_HEIGHT_OFFSET, RGBColor::RED);*/
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

std::string Tile::DirectionToString(Direction direction)
{
	switch(direction)
	{
		case NORTHWEST:
			return "NW";
		case NORTH:
			return "N";
		case NORTHEAST:
			return "NE";
		case EAST:
			return "E";
		case SOUTHEAST:
			return "SE";
		case SOUTH:
			return "S";
		case SOUTHWEST:
			return "SW";
		case WEST:
			return "W";
		case CENTER:
			return "C";
	}
}

Direction Tile::StringToDirection(std::string direction)
{
	if(direction == "N")
		return NORTH;
	else if(direction == "NE")
		return NORTHEAST;
	else if(direction == "E")
		return EAST;
	else if(direction == "SE")
		return SOUTHEAST;
	else if(direction == "S")
		return SOUTH;
	else if(direction == "SW")
		return SOUTHWEST;
	else if(direction == "W")
		return WEST;
	else if(direction == "NW")
		return NORTHWEST;
	else
		return CENTER;
}
