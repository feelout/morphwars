#ifndef MAP_H
#define MAP_H

#include <string>
#include "Tile.h"
#include "TileSet.h"

class Map
{
	private:
		Tile *tiles;
		TileSet *tileset;

		int width,height;

		void loadMap(std::string filename);
	public:
		Map(int width, int height, std::string tilesetName);
		Map(std::string filename);
};

#endif //MAP_H
