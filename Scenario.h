#ifndef SCENARIO_H
#define SCENARIO_H

#include <string>
#include <list>
#include "Map.h"
#include "Player.h"
#include "Drawer.h"
#include "EventDispatcher.h"

namespace Core
{
	class Scenario : public IEventListener
	{
		private:
			/** Pass <map> TiXMLNode to Map class **/
			bool loadFromFile(std::string path);

			std::list<Player*> players;
			Map *map;
			Player *currentPlayer;
		public:
			Scenario(std::string path);
			~Scenario();

			void switchTurn(Player *player);

			virtual void draw(Graphics::Drawer *target, int x, int y);

			virtual void mouseMoved(int x, int y);
			virtual void mouseLMBClicked(int x, int y);
			virtual void mouseRMBClicked(int x, int y);
			virtual void keyPressed(int key);
	};
}

#endif //SCENARIO_H
