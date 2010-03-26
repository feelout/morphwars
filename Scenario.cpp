#include <algorithm>
#include "Scenario.h"
#include "Logger.h"
#include "UnitTypeManager.h"
#include "Order.h"
#include "OrderManager.h"
#include "Engine.h"
#include "MouseSelector.h"

using namespace Core;

const int MAP_MINIMAL_FRAME_WIDTH = 400;
const int MAP_MINIMAL_FRAME_HEIGHT = 300;

Scenario::Scenario(std::string path)
	: EngineState("Scenario"), currentPlayer(NULL), map(NULL), sidepanel(NULL), hbox(NULL)
{
	if(!loadFromFile(path))
	{
		Utility::Logger::getInstance()->log("Failed to load scenario %s\n", path.c_str());
		return;
	}

	OrderFactory::getInstance()->registerCreator("move", new MovementOrderCreator());

	int screen_height = Engine::getInstance()->getRenderer()->getHeight();
	int screen_width = Engine::getInstance()->getRenderer()->getWidth();

	Utility::Logger::getInstance()->log("Screen: %i,%i\n", screen_width, screen_height);

	hbox = new Gui::HBox(Rect(0,0,screen_width, screen_height), 5);

	sidepanel = new Gui::SidePanel(Rect(0,0, Gui::SidePanel::SIDE_PANEL_WIDTH, screen_height-50), map);

	Utility::Logger::getInstance()->log("BEGUN SCENARIO HBOX POPULATION\n");
	Rect rf = sidepanel->getRequestedFrame();
	Utility::Logger::getInstance()->log("SidePanel requested %i,%i\n", rf.w, rf.h);
	hbox->addChild(map, true, 0);
	hbox->addChild(sidepanel, true, 0);
	Utility::Logger::getInstance()->log("ENDED SCENARIO HBOX POPULATION\n");
}

Scenario::~Scenario()
{
	players.clear(); //check, whether clear() calls destructors
	delete map;
}

bool Scenario::loadFromFile(std::string path)
{
	Utility::Logger::getInstance()->log("Loading scenario %s\n", path.c_str());

	TiXmlDocument xmldoc(path.c_str());

	if(!xmldoc.LoadFile()) {
		Utility::Logger::getInstance()->log("Failed to load scenario XML file.\n");
		return false;
	}

	TiXmlElement *root = xmldoc.FirstChildElement("scenario");

	/** Map and tileset **/
	Utility::Logger::getInstance()->log("\nLoading map\n");

	Graphics::Renderer *renderer = Engine::getInstance()->getRenderer();
	//map = new Map(Rect(5,5, renderer->getWidth()-Gui::SidePanel::SIDE_PANEL_WIDTH-15, renderer->getHeight()-10),
		       	//root->FirstChildElement("map"));
	map = new Map(Rect(0,0, MAP_MINIMAL_FRAME_WIDTH, MAP_MINIMAL_FRAME_HEIGHT), root->FirstChildElement("map"));

	Utility::Logger::getInstance()->log("\nLoading object types\n");
	/** Unit and Building types **/
	TiXmlNode *parent, *child=NULL;
	
	parent = root->FirstChildElement("types");

	while(child = parent->IterateChildren("unit", child))
	{
		UnitTypeManager::getInstance()->getType(child->ToElement()->GetText());
	}

	child = NULL;
	/*while(child = parent->IterateChildren("building", child)->ToElement())
	{
		BuildingTypeManager::getInstance()->getType(child->ValueStr());
	}*/

	/** Players **/
	Utility::Logger::getInstance()->log("\nLoading players\n");

	TiXmlNode *section=NULL, *current=NULL;
	Player *currentPlayer;
	Unit *currentUnit;
	Building *currentBuilding;

	parent = root->FirstChildElement("players");
	child = NULL;

	std::vector<Force*> forces;

	while(child = parent->IterateChildren("player", child))
	{
		Utility::Logger::getInstance()->log("\nNew player\n");

		std::string s_fraction = child->ToElement()->Attribute("fraction");
		Fraction fraction;

		if(s_fraction == "Legacy")
		{
			fraction = LEGACY;
		}
		else if(s_fraction == "Hand")
		{
			fraction = HAND;
		}
		else
		{
			fraction = NEUTRAL;
		}

		/** TODO: Add controller **/
		int r,g,b; //Player`s color
		child->ToElement()->QueryIntAttribute("r", &r);
		child->ToElement()->QueryIntAttribute("g", &g);
		child->ToElement()->QueryIntAttribute("b", &b);



		int forcenum = 0;
		child->ToElement()->QueryIntAttribute("force", &forcenum);

		Utility::Logger::getInstance()->log("Force number = %i\n", forcenum);

		Force *force = NULL;

		for(std::vector<Force*>::iterator i = forces.begin(); i != forces.end(); ++i)
		{
			if((*i)->getNumber() == forcenum)
			{
				force = *i;
				break;
			}
		}

		if(!force)
		{
			Utility::Logger::getInstance()->log("Creating force %i\n", forcenum);
			force = new Force(forcenum);
			forces.push_back(force);
		}

		currentPlayer = new Player(child->ToElement()->Attribute("name"),  fraction, force, RGBColor(r,g,b), 
				map->getWidth(), map->getHeight());

		/** Adding controller **/
		PlayerController *currentController = NULL;
		std::string s_controller = child->ToElement()->Attribute("controller");

		// TODO: Make using factory
		if(s_controller == "local")
		{
			currentController = new LocalPlayerController(currentPlayer, map, sidepanel);
		}
		else if(s_controller == "ai")
		{
			currentController = new AIPlayerController(currentPlayer);
		}	
		else
		{
			Utility::Logger::getInstance()->log("Unknown controller type: %s\n", s_controller.c_str());
		}

		section = child->FirstChildElement("units");

		Utility::Logger::getInstance()->log("Adding units to player\n");
		while(current = section->IterateChildren("unit", current))
		{
			int x,y;
			current->ToElement()->QueryIntAttribute("x", &x);
			current->ToElement()->QueryIntAttribute("y", &y);

			std::string type = current->ToElement()->Attribute("type");

			Utility::Logger::getInstance()->log("Type : %s\n", type.c_str());

			currentUnit = new Unit(UnitTypeManager::getInstance()->getType(type),
						map->getTile(x,y), currentPlayer);
			currentPlayer->addUnit(currentUnit);
		}

		Utility::Logger::getInstance()->log("Loading buildings\n");

		section = child->FirstChildElement("buildings");

		current = NULL;

		while(current = section->IterateChildren("building", current))
		{
			//TODO: Create building
		}

		currentPlayer->setDone(true);
		players.push_back(currentPlayer);
	}


	//sidepanel = new Gui::SidePanel(Rect(screen_width-Gui::SidePanel::SIDE_PANEL_WIDTH-5, 5,
	//			Gui::SidePanel::SIDE_PANEL_WIDTH, screen_height-10), map);

	switchTurn(*(players.begin()));
	//this->currentPlayer = *(players.begin());
	//this->currentPlayer->setDone(true);

	//TODO: setup scripts
	
	Utility::Logger::getInstance()->log("Scenario loaded\n\n");


	return true;
}

void Scenario::switchTurn(Player *player)
{
	Utility::Logger::getInstance()->log("Passing turn to player %s\n", player->getName().c_str());
	if(currentPlayer)
	{
		currentPlayer->setDone(true);
		currentPlayer->setCurrent(false);
	}

	currentPlayer = player;
	currentPlayer->setDone(true); //not moving anything yet
	currentPlayer->setCurrent(true);
	currentPlayer->onTurnBegin();
	if(sidepanel)
	{
		sidepanel->setCurrentPlayer(currentPlayer);
	}

	//FIXME: Does not work, because fov can change by moving units (move to order??)
	//map->setFieldOfView(currentPlayer->getFieldOfView());
}

void Scenario::nextTurn()
{
	if(!currentPlayer->isDone())
		return;

	std::list<Player*>::iterator i = std::find(players.begin(), players.end(), currentPlayer);
	
	if(++i == players.end())
		i = players.begin();

	//currentPlayer = *i;
	switchTurn(*i);
}

void Scenario::draw(Graphics::Surface *target)
{
	//FIXME!!! - is it slow checking for fov on each frame?
	map->setFieldOfView(currentPlayer->getFieldOfView());
	//map->draw(target);

	//sidepanel->draw(target);
	//
	
	hbox->draw(target);

	std::list<Player*>::const_iterator i;

	for(i = players.begin(); i != players.end(); ++i)
	{
		(*i)->renderObjects(target, currentPlayer->getFieldOfView(), map);
	}
}

void Scenario::process()
{
	OrderManager::getInstance()->processOrders();

	if(currentPlayer->hasEndedTurn())
	{
		nextTurn();
	}
}

bool Scenario::mouseMoved(int x, int y)
{
	return false;
}

bool Scenario::mouseLMBClicked(int x, int y)
{
	sidepanel->mouseLMBClicked(x,y);
	return false;
}

bool Scenario::mouseRMBClicked(int x, int y)
{
	return false;
}

bool Scenario::keyPressed(int key)
{
	switch(key)
	{
		case SDLK_e:
			nextTurn();
			return true;
	};

	return false;
}
