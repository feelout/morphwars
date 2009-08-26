#ifndef MAP_H
#define MAP_H

#include <string>
#include "Tile.h"
#include "TileSet.h"
#include "Drawer.h"
#include "Player.h"
#include "Widget.h"
#include "Minimap.h"
#include "tinyxml.h"

namespace Core
{
	class Map
	{
		private:
			Tile **tiles;
			TileSet *tileset;

			int width,height;
			//FIXME: work on this one
			// Map clipping rect
			Rect clip;
			// As in widget (cannot subclass Widget, because of non-standart draw() method)
			Rect frame;

			std::string author;
			std::string name;
			std::string version;

			void calculateSurfaces();

			//Cache
			FieldOfView *lastFov;
			Graphics::Surface *cached;
			//Minimap
			//Minimap *minimap; //will be in panel in Scenario

			void recalculateClipping();
		public:
			Map(Rect frame, int width, int height, std::string tilesetName);
			Map(Rect frame, TiXmlElement *xmlmap);
			~Map();

			Tile* getTile(int x, int y);
			Tile* getTileByMouseCoords(int mx, int my);

			int getWidth() const;
			int getHeight() const;

			void setFrame(Rect frame);
			Rect getFrame() const;

			void setPointOfView(int x, int y);
			void setClipping(Rect clip);
			Rect getClipping() const;

			void draw(Graphics::Drawer *target, FieldOfView *fov, bool drawframe=true);
	};
}

#endif //MAP_H
