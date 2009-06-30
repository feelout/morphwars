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

Graphics::Surface* TileType::getTileImage(TileImageType type) const
{
	return surfaces[type];
}

Tile::Tile(int x, int y, TileType *type)
	: x(x), y(y), type(type)
{
	image = type->getTileImage(CENTER);
}

void Tile::setImageType(TileImageType imgtype)
{
	image = type->getTileImage(imgtype);
}

void Tile::setImage(Graphics::Surface *image)
{
	this->image = image;
}

TileType* Tile::getType() const
{
	return type;
}
