#ifndef SCENARIO_H
#define SCENARIO_H

#include <string>
#include <list>
#include <utility>
#include "Map.h"
#include "Player.h"
#include "Drawer.h"
#include "EngineState.h"
#include "SidePanel.h"
#include "PlayerController.h"

namespace Core
{
	class Scenario : public EngineState
	{
		private:
			/** Pass <map> TiXMLNode to Map class **/
			bool loadFromFile(std::string path);

			//std::list< std::pair<Player*, PlayerController*> > players;
			std::list<Player*> players;
			Map *map;
			Player *currentPlayer;

			Gui::SidePanel *sidepanel;
		public:
			Scenario(std::string path);
			~Scenario();

			void switchTurn(Player *player);
			void nextTurn();

			virtual void draw(Graphics::Drawer *target);
			virtual void process();

			virtual bool mouseMoved(int x, int y);
			virtual bool mouseLMBClicked(int x, int y);
			virtual bool mouseRMBClicked(int x, int y);
			virtual bool keyPressed(int key);
	};
}

#endif //SCENARIO_H
