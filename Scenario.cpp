#include <algorithm>
#include "Scenario.h"
#include "Logger.h"
#include "UnitTypeManager.h"
#include "Order.h"

using namespace Core;

Scenario::Scenario(std::string path)
	: currentPlayer(NULL), map(NULL)
{
	if(!loadFromFile(path))
	{
		Utility::Logger::getInstance()->log("Failed to load scenario %s\n", path.c_str());
	}
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

	if(!xmldoc.LoadFile())
	{
		Utility::Logger::getInstance()->log("Failed to load scenario XML file.\n");
		return false;
	}

	TiXmlElement *root = xmldoc.FirstChildElement("scenario");

	/** Map and tileset **/
	Utility::Logger::getInstance()->log("\nLoading map\n");
	map = new Map(root->FirstChildElement("map"));

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

void Scenario::draw(Graphics::Drawer *target, int x, int y)
{
	//FIXME: Add units nad other stuff..
	map->draw(target, x, y, currentPlayer->getFieldOfView());

	std::list<Player*>::const_iterator i;

	for(i = players.begin(); i != players.end(); ++i)
	{
		(*i)->renderObjects(target, x, y, currentPlayer->getFieldOfView());
	}
}

void Scenario::mouseMoved(int x, int y)
{
}

void Scenario::mouseLMBClicked(int x, int y)
{
	Tile *clickedTile = map->getTileByMouseCoords(x, y, 0, 0);

	if(!clickedTile)
		return;
	
	MapObject *topobject = NULL;
	topobject = clickedTile->getTopObject();

	if(topobject)
	{
		if(topobject->getOwner() == currentPlayer)
		{
			currentPlayer->selectObject(topobject);
		}
	}
}

void Scenario::mouseRMBClicked(int x, int y)
{
	//Utility::Logger::getInstance()->log("\nMouse RMB clicked on (%i,%i)\n", x, y);
	Tile *clickedTile = map->getTileByMouseCoords(x, y, 0, 0);

	if(!clickedTile)
		return;

	MapObject *selected = currentPlayer->getSelectedObject();

	if(!selected)
	{
		Utility::Logger::getInstance()->log("Nothing clicked\n");
		return;
	}

	Utility::Logger::getInstance()->log("Selected type: %s\n", selected->getType()->getType().c_str());

	if(selected->getType()->getType() == "Unit") //ugh, hate it
	{
		Utility::Logger::getInstance()->log("Moving unit\n");
		Unit *target = static_cast<Unit*>(selected);
		MovementOrder *order = new MovementOrder(target, clickedTile, map);
	}
}

void Scenario::keyPressed(int key)
{
	switch(key)
	{
		case SDLK_e:
			nextTurn();
			break;
	};
}
