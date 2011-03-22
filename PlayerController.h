#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "Player.h"
#include "Map.h"
#include "EventDispatcher.h"
#include "Order.h"
#include "SidePanel.h"

namespace Core
{
	class PlayerController
	{
		protected:
			Player *target;
		public:
			PlayerController(Player *target);
			virtual ~PlayerController();

			virtual void newTurn();

			//FIXME: DEBUG!
			Player* getTarget();
			virtual Gui::Container *getGUI();
	};

	class LocalPlayerController : public IEventListener, public PlayerController
	{
		protected:
			Map *map;
			boost::shared_ptr<Order> pendingOrder;
			Gui::SidePanel *sidePanel;
			MapObject *currentObject;
			Gui::HBox *gui;

			bool objectTargeted(Tile *clickedTile);
		public:
			//LocalPlayerController(Player *target, Map *map, Gui::SidePanel *sidePanel);
			LocalPlayerController(Player *target, Map *map);
			virtual ~LocalPlayerController();

			void setOrder(boost::shared_ptr<Order> order);
			void setOrder(std::string orderName);

			virtual bool mouseMoved(int x, int y);
			virtual bool mouseLMBClicked(int x, int y);
			virtual bool mouseRMBClicked(int x, int y);
			virtual bool keyPressed(int key);

			virtual Gui::Container* getGUI();
			virtual void newTurn();
	};

	class AIPlayerController : public PlayerController
	{
		public:
			AIPlayerController(Player *target);
			virtual ~AIPlayerController();

			virtual void newTurn();
	};
}

#endif //PLAYER_CONTROLLER_H
