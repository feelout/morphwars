#include "Map.h"
#include "Logger.h"
#include "tinyxml.h"

using namespace Core;

Map::Map(int width, int height, std::string tilesetname)
	: width(width), height(height)
{
	tiles = new Tile*[width*height];
	tileset = new TileSet(tilesetname);

	for(int y=0; y < height; ++y)
	{
		for(int x=0; x < width; ++x)
		{
			tiles[x+y*width] = new Tile(x, y, tileset->getType(0));
			tiles[x+y*width]->setImageType(CENTER);
		}
	}
}

Map::Map(std::string name)
{
	Utility::Logger::getInstance()->log("Loading map %s\n", name.c_str());
	if(loadMap(name))
	{
		Utility::Logger::getInstance()->log("Map %s loaded successfully\n", name.c_str());
	}
	else
	{
		Utility::Logger::getInstance()->log("Couldn`t load map %s\n", name.c_str());
	}
}

bool Map::loadMap(std::string name)
{
	std::string folder = "Maps/"+name+"/";

	TiXmlDocument xmlmap((folder+name+".xml").c_str());
	xmlmap.LoadFile();
	/* Map metadata */
	TiXmlElement *map = xmlmap.FirstChildElement("map");

	if(!map)
	{
		Utility::Logger::getInstance()->log("Couldn`t find <map> element.\n");
		return false;
	}

	author = map->Attribute("author");
	name = map->Attribute("name");
	version = map->Attribute("version");

	Utility::Logger::getInstance()->log("Loading map %s %s by %s\n", name.c_str(), version.c_str(), author.c_str());

	/* Terrain */
	TiXmlElement *terrain = map->FirstChildElement("terrain");

	if(!terrain)
	{
		Utility::Logger::getInstance()->log("Couldn`t find <terrain> element.\n");
		return false;
	}

	terrain->QueryIntAttribute("width", &width);
	terrain->QueryIntAttribute("height", &height);

	Utility::Logger::getInstance()->log("Dimensions: %ix%i\n", width, height);

	tiles = new Tile*[width*height];

	// Creating tileset
	tileset = new TileSet(terrain->FirstChildElement("tileset")->GetText());

	// Cells themselves
	TiXmlNode *cells_node = terrain->FirstChildElement("cells");

	int y=0;
	int x=0;
	for(TiXmlElement *row = cells_node->FirstChildElement("row");  row; row = row->NextSiblingElement("row"), ++y)
	{
		x = 0;
		for(TiXmlElement *cell = row->FirstChildElement("cell"); cell; cell = cell->NextSiblingElement("cell"), ++x)
		{
			int tilenum;
			sscanf(cell->GetText(), "%i", &tilenum);

			tiles[x+y*width] = new Tile(x,y,tileset->getType(tilenum));
		}
	}

	calculateSurfaces();

	// TODO: Objects

	/* TODO: Players */

	TiXmlElement *players_root_el = map->FirstChildElement("players");

	for(TiXmlElement *player_el = players_root_el->FirstChildElement("player"); player_el; player_el =
players_root_el->NextSiblingElement("player"))
	{
		// Create players, their unit, buildings
		// Neutral players will be here also
	}
}

void Map::calculateSurfaces()
{
	// TODO: Stub
	for(int y=0; y < height; ++y)
	{
		for(int x=0; x < width; ++x)
		{
		}
	}
}

Map::~Map()
{
	for(int i=0; i < width*height; ++i)
	{
		tiles[i]->setImageType(CENTER);
		delete tiles[i];
	}
	delete[] tiles;
	delete tileset;
}
