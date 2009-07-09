#include "Scenario.h"
#include "Logger.h"
#include "UnitTypeManager.h"

using namespace Core;

Scenario::Scenario(std::string path)
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
	Utility::Logger::getInstance()->log("Loading map\n");
	map = new Map(root->FirstChildElement("map"));

	Utility::Logger::getInstance()->log("Loading object types\n");
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
	Utility::Logger::getInstance()->log("Loading players\n");

	TiXmlNode *section=NULL, *current=NULL;
	Player *currentPlayer;
	Unit *currentUnit;
	Building *currentBuilding;

	parent = root->FirstChildElement("players");
	child = NULL;

	while(child = parent->IterateChildren("player", child))
	{
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

		currentPlayer = new Player(child->ToElement()->Attribute("name"),  fraction, RGBColor(r,g,b));

		section = child->FirstChildElement("units");

		while(current = section->IterateChildren("unit", current))
		{
			int x,y;
			current->ToElement()->QueryIntAttribute("x", &x);
			current->ToElement()->QueryIntAttribute("y", &y);
			currentUnit = new Unit(UnitTypeManager::getInstance()->getType(current->ToElement()->Attribute("type")),
						map->getTile(x,y), currentPlayer);
		}

		section = child->FirstChildElement("buildings");

		current = NULL;

		while(current = section->IterateChildren("building", current))
		{
			//TODO: Create building
		}

		players.push_back(currentPlayer);
	}

	//TODO: setup scripts
	return true;
}
