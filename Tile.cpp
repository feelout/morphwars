#include "Tile.h"
#include "MapObject.h"
#include "assist.h"

using namespace Core;

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

int TileType::getPriority() const
{
	return priority;
}

Tile::Tile(int x, int y, TileType *type)
	: x(x), y(y), type(type)
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
}

TileType* Tile::getType() const
{
	return type;
}

Direction Tile::getDirection(Tile *dst)
{
	//printf("Calculating direction from (%i,%i) to (%i,%i)\n", x, y, dst->x, dst->y);
	//int dx = this->x - dst->x;
	//int dy = this->y - dst->y;
	int dx = dst->x - this->x;
	int dy = dst->y - this->y;

	//printf("dx = %i, dy = %i\n", dx, dy);

	if(dy == 0)
	{
		if(dx > 0) return EAST;
		else if(dx < 0) return WEST;
		else return CENTER;
		//return CENTER;
	}

	if((y % 2) == (dst->getY() % 2))
	{
		//printf("Tiles are not shifted\n");
		if(dy > 0)
		{
			if(dx > 0) return SOUTHEAST;
			else if(dx < 0) return SOUTHWEST;
			else return SOUTH;
			//else return SOUTHWEST;
		}
		else
		{
			if(dx > 0) return NORTHEAST;
			else if(dx < 0) return NORTHWEST;
			else return NORTH;
			//else return NORTHWEST;
		}
	}
	else
	{
		//printf("Tiles are shifted\n");
		if(dy > 0)
		{
			//if(dx > 0) return SOUTHEAST;
			//else return SOUTHWEST;
			if(y % 2 == 0)
			{
				if(dx < 0) return SOUTHWEST;
				else return SOUTHWEST;
			}
			else
			{
				if(dx > 0) return SOUTHEAST;
				else return SOUTHWEST;
			}
		}
		else //THE MAIN BUG
		{
			//if(dx > 0) return NORTHEAST;
			//else return NORTHWEST;
			if(y % 2 == 0)
			{
				if(dx < 0) return NORTHWEST;
				else return NORTHEAST;
			}
			else
			{
				if(dx > 0) return NORTHEAST;
				else return NORTHWEST;
			}
			//if(dx < 0) return NORTHWEST;
			//else return NORTHEAST;
		}
	}
}

void Tile::addObject(MapObject *object)
{
	objects.push_back(object);
	//TODO: Sort objects, buildings first
}

int Tile::getX() const
{
	return x;
}

int Tile::getY() const
{
	return y;
}

void Tile::draw(Graphics::Drawer *target, int x, int y)
{
	//printf("Drawing Tile (%i,%i) on %i,%i\n", this->x, this->y, x, y);
	image->blit(target->getTarget(), x, y);
	for(std::vector<MapObject*>::iterator i = objects.begin(); i != objects.end(); ++i)
	{
		(*i)->draw(target, x, y);
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
	//target->drawRect(Rect(x+1, y+1, TILE_WIDTH-1, TILE_HEIGHT-1), RGBColor(255, 0, 0));
}
