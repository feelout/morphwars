#ifndef MAP_H
#define MAP_H

#include <string>
#include "Tile.h"
#include "TileSet.h"
#include "Drawer.h"
#include "tinyxml.h"

namespace Core
{
    class Map
    {
        private:
            Tile **tiles;
            TileSet *tileset;

            int width,height;

            std::string author;
            std::string name;
            std::string version;

            void calculateSurfaces();
        public:
            Map(int width, int height, std::string tilesetName);
	    Map(TiXmlElement *xmlmap);
            ~Map();

            Tile* getTile(int x, int y);

	    void draw(Graphics::Drawer *target, int x, int y);
    };
}

#endif //MAP_H
