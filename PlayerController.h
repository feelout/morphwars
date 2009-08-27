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
	};

	class LocalPlayerController : public IEventListener, public PlayerController
	{
		protected:
			Map *map;
		public:
			LocalPlayerController(Player *target, Map *map);

			virtual void mouseMoved(int x, int y);
			virtual void mouseLMBClicked(int x, int y);
			virtual void mouseRMBClicked(int x, int y);
			virtual void keyPressed(int key);
	};

	class AIPlayerController : public PlayerController
	{
		public:
			AIPlayerController(Player *target);
	};
}

#endif //PLAYER_CONTROLLER_H
