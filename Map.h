#ifndef MAP_H
#define MAP_H

#include <string>
#include "Tile.h"
#include "TileSet.h"

class Map
{
	private:
		Tile **tiles;
		TileSet *tileset;

		int width,height;
		
		std::string author;
		std::string name;
		std::string version;

		bool loadMap(std::string name);
		void calculateSurfaces();
	public:
		Map(int width, int height, std::string tilesetName);
		Map(std::string name);
		~Map();
};

#endif //MAP_H
