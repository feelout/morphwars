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

			std::list< std::pair<Player*, PlayerController*> > players;
			std::list<PlayerController*> controllers;
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

			virtual void mouseMoved(int x, int y);
			virtual void mouseLMBClicked(int x, int y);
			virtual void mouseRMBClicked(int x, int y);
			virtual void keyPressed(int key);
	};
}

#endif //SCENARIO_H
