#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "Player.h"
#include "Map.h"
#include "EventDispatcher.h"

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
	};

	class LocalPlayerController : public IEventListener, public PlayerController
	{
		protected:
			Map *map;
		public:
			LocalPlayerController(Player *target, Map *map);
			virtual ~LocalPlayerController();

			virtual bool mouseMoved(int x, int y);
			virtual bool mouseLMBClicked(int x, int y);
			virtual bool mouseRMBClicked(int x, int y);
			virtual bool keyPressed(int key);
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
