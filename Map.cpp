#include <Map.h>

Map::Map(int width, int height, std::strign tilesetname)
{
	tiles = new Tile[width*height];
	tileset = new TileSet(tilesetname);

	for(int y=0; y < height; ++y)
	{
		for(int x=0; x < width; ++x)
		{
			tiles[x+y*width] = new Tile(x, y, tileset->getType(0));
			tiles[x+y*width]->setImageType(CENTER);
		}
	}
}

Map::Map(std::string filename)
{
	loadMap(filename);
}

void Map::loadMap(std::string filename)
{
}
