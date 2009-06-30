#include "Scenario.h"
#include "Logger.h"

using namespace Core;

Scenario::Scenario(std::string path)
{
	if(!loadFromFile(path))
	{
		Utility::Logger::getInstance()->log("Failed to load scenario %s\n", path);
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

	TiXmlDocument xmldoc(path);

	if(!xmldoc.LoadFile())
	{
		Utility::Logger::getInstance()->log("Failed to load scenario XML file.\n");
	}

	TiXmlElement *root = xmldoc.FirstChildElement("scenario");

	/** Map and tileset **/
	Utility::Logger::getInstance()->log("Loading map\n");
	map = new Map(root->FirstChildElement("map"));

	Utility::Logger::getInstance()->log("Loading object types\n");
	/** Unit and Building types **/
	TiXmlElement *parent, *child;
	
	parent = root->FirstChildElement("types");

	while(child = parent->IterateChildren("unit", child))
	{
		UnitTypeManager::getInstance()->getType(child->ValueStr());
	}

	while(child = parent->IterateChildren("building", child))
	{
		BuildingTypeManager::getInstance()->getType(child->ValueStr());
	}

	/** Players **/
	Utility::Logger::getInstance()->log("Loading players\n");

	TiXmlElement *section, *current;
	Player *currentPlayer;
	Unit *currentUnit;
	Building *currentBuilding;

	parent = root->FirstChildElement("players");

	while(child = parent->IterateChildren("player", child))
	{
		std::string s_fraction = child->Attribute("fraction");
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
		currentPlayer = new Player(child->Attribute("name"),  fraction, RGBColor(child->QueryIntAttribute("r"),
					child->QueryIntAttribute("g"), child->QueryIntAttribute("b")));

		section = child->FirstChildElement("units");

		while(current = section->IterateChildren("unit", current))
		{
			currentUnit = new Unit(UnitTypeManager::getInstance()->getType(current->Attribute("type")),
						map->getTile(current->QueryIntAttribute("x"),current->QueryIntAttribute("y")),
						currentPlayer);
		}

		section = child->FirstChildElement("buildings");

		while(current = section->IterateChildren("building", current))
		{
			//TODO: Create building
		}

		players.push_back(currentPlayer);
	}

	//TODO: setup scripts
	return true;
}
