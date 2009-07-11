#include "Tile.h"
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
	int xdelta = this->x - dst->x;
	int ydelta = this->y - dst->y;

	if(xdelta > 0)
	{
		if(ydelta > 0)
		{
			return NORTHEAST;
		}
		else
		{
			return SOUTHEAST;
		}
	}
	else
	{
		if(ydelta > 0)
		{
			return NORTHWEST;
		}
		else
		{
			return SOUTHWEST;
		}
	}
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
	//target->drawRect(Rect(x+1, y+1, TILE_WIDTH-1, TILE_HEIGHT-1), RGBColor(255, 0, 0));
}
