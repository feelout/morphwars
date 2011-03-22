#ifndef SIDEPANEL_H
#define SIDEPANEL_H

#include <string>
#include <map>
#include "Minimap.h"
#include "Widget.h"
#include "Player.h"
#include "Box.h"
#include "Placeholder.h"
#include "Button.h"

namespace Core
{
	class LocalPlayerController;
}

namespace Gui
{
	class OrderButton : public ImageButton
	{
		public:
			const static int ORDER_ICON_WIDTH, ORDER_ICON_HEIGHT;
		protected:
			Core::LocalPlayerController *receiver;
			std::string orderName;
		public:
			OrderButton(std::string orderName);

			void setReceiver(Core::LocalPlayerController *receiver);
			virtual bool mouseLMBClicked(int x, int y);
	};

	class OrderButtonCache
	{
		private:
			static OrderButtonCache *instance;
			
			std::map<std::string, OrderButton*> buttons;
		public:
			static OrderButtonCache* getInstance();

			OrderButton* getButton(std::string orderName);
	};

	/* FIXME: HBox here is temporary. When Table is ready, replace */
	class OrderPanel : public HBox
	{
		private:
			Core::MapObject *currentObject; //TODO : May be not needed
		public:
			OrderPanel(Rect frame, Widget *parent=NULL);

			void setCurrentObject(Core::MapObject *currentObject);
	};

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
			//Container *actions;
			OrderPanel *orderPanel;
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
