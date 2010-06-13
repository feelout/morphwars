#ifndef MAP_H
#define MAP_H

#include <string>
#include "Tile.h"
#include "TileSet.h"
#include "Surface.h"
#include "Player.h"
#include "Widget.h"
#include "Minimap.h"
#include "tinyxml.h"

namespace Core
{
	class Map : public Gui::Widget
	{
		private:
			Tile **tiles;
			int maxTileHeight;
			TileSet *tileset;

			int width,height;
			//FIXME: work on this one
			// Map clipping rect
			Rect clip;
			// As in widget (cannot subclass Widget, because of non-standart draw() method)
			//Rect frame;

			std::string author;
			std::string name;
			std::string version;

			void calculateSurfaces();

			//Cache
			Graphics::Surface *cached;
			//Minimap
			//Minimap *minimap; //will be in panel in Scenario
			//
			
			FieldOfView *currentFov;

			void recalculateClipping();
			virtual void frameUpdated();

			void updateCache();
		public:
			Map(Rect frame, int width, int height, std::string tilesetName, Widget *parent=NULL);
			Map(Rect frame, TiXmlElement *xmlmap, Widget *parent=NULL);
			~Map();

			Tile* getTile(int x, int y);
			Tile* getTileByMouseCoords(int mx, int my);

			int getWidth() const;
			int getHeight() const;

			void setPointOfView(int x, int y);
			void setClipping(Rect clip);
			Rect getClipping() const;

			void setFieldOfView(FieldOfView *fov);

			// How to make it consistent with definition of Widget::draw?? TODO: STOPPED HERE
			void draw(Graphics::Surface *target, FieldOfView *fov, bool drawframe=true);
			void draw(Graphics::Surface *target);
	};
}

#endif //MAP_H
