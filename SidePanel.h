#ifndef SIDEPANEL_H
#define SIDEPANEL_H

#include "Minimap.h"
#include "Widget.h"
#include "Player.h"
#include "Box.h"
#include "Placeholder.h"

namespace Gui
{
	class ResourceBar : public Widget
	{
		public:
			static const int RESOURCE_BAR_HEIGHT = 25;
		private:
			Core::Player *currentPlayer;
		public:
			ResourceBar(Rect frame);

			void setCurrentPlayer(Core::Player *player);

			virtual void draw(Graphics::Surface *target);
	};

	class SidePanel : public VBox
	{
		public:
			static const int SIDE_PANEL_WIDTH = 200;
		private:
			Minimap *minimap;
			Core::Player *currentPlayer;
			Core::MapObject *currentObject;

			// Line with energy count (and mb some other info too)
			ResourceBar *resources;
			// Info on current object
			//Container *objectinfo;
			Placeholder *objectInfo;
			// Table of object actions
			Container *actions;
			// Widget for selecting from objects in tile
			Container *tileobjects;
			// System buttons (menu and end turn)
			Container *buttons;

			virtual void frameUpdated();
		public:
			SidePanel(Rect frame, Core::Map *map);
			~SidePanel();

			void setCurrentPlayer(Core::Player *player);
			void setCurrentObject(Core::MapObject *object);
			void draw(Graphics::Surface *target);
	};
}

#endif //SIDEPANEL_H
