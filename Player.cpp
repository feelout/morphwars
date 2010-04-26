#include <algorithm>
#include "Player.h"
#include "Logger.h"
#include "Tile.h"
#include "Unit.h"
#include "Map.h"
#include "PlayerController.h"

using namespace Core;

bool BackwardCompareUnits(Unit *u1, Unit *u2)
{
	Tile *t1 = u1->getTile();
	Tile *t2 = u2->getTile();

	/*if(t1->getY() != t2->getY())
	{
		return t1->getY() > t2->getY();
	}
	else
	{
		return t1->getX() > t2->getX();
	}*/
	return true;
}

FieldOfView::FieldOfView(int w, int h)
	: w(w), h(h)
{
	tiles = new bool[w*h];
	
	clear();
}

void FieldOfView::clear()
{
	for(int i=0; i < w*h; ++i)
	{
		tiles[i] = false;
	}
}

bool FieldOfView::isTileVisible(int x, int y) const
{
	return tiles[x+y*w];
}

void FieldOfView::setTileVisible(int x, int y, bool visible)
{
	if((x < 0) || (x >= w) || (y < 0) || (y >= h)) return;

	tiles[x+y*w] = visible;
}

FieldOfView& FieldOfView::operator = (const FieldOfView& other)
{
	for(int i=0; i < w*h; ++i)
	{
		tiles[i] = other.tiles[i];
	}
}

bool FieldOfView::operator == (const FieldOfView& other)
{
	for(int i=0; i < w*h; ++i)
	{
		if(tiles[i] != other.tiles[i]) return false;
	}

	return true;
}

void Player::updateFOV()
{
	fov->clear();
	//TODO: find the way to combine vectors
	for(std::vector<Unit*>::iterator i = units.begin(); i != units.end(); ++i)
	{
		std::vector< std::pair<int, int> > neighbours = (*i)->getTile()->getNeighbours();
		std::vector< std::pair<int, int> >::iterator nb_iter;

		for(nb_iter = neighbours.begin(); nb_iter != neighbours.end(); ++nb_iter)
		{
			fov->setTileVisible(nb_iter->first, nb_iter->second, true);
		}

		fov->setTileVisible((*i)->getTile()->getX(), (*i)->getTile()->getY(), true);
	}

	//FIXME: Uncomment after completing Building class
	/*for(std::vector<Building*>::iterator i = buildings.begin(); i != buildings.end(); ++i)
	{
		x = (*i)->getTile()->getX();
		y = (*i)->getTile()->getY();

		for(int dy=-1; dy < 2; ++dy)
		{
			for(int dx=-1; dx < 2; ++dx)
			{
				fov->setTileVisible(x+dx, y+dy, true);
			}
		}
	}*/
}

Player::Player(std::string name, Fraction fraction, Force *force, RGBColor color, int mapWidth, int mapHeight)
	: name(name), fraction(fraction), force(force), color(color), energy(0), 
	/*selected(NULL),*/ done(true), current(false), endedTurn(true)
{
	fov = new FieldOfView(mapWidth, mapHeight);
	force->addPlayer(this);
	Utility::Logger::getInstance()->log("Player %s created.\n", name.c_str());
}

void Player::addUnit(Unit *unit)
{
	units.push_back(unit);

	//fixme
	std::sort(units.begin(), units.end(), BackwardCompareUnits); 

	updateFOV();
}

void Player::addBuilding(Building *building)
{
	buildings.push_back(building);

	updateFOV();
}

FieldOfView* Player::getFieldOfView() const
{
	return fov;
}

Force* Player::getForce() const
{
	return force;
}

std::string Player::getName() const
{
	return name;
}

/*void Player::selectObject(MapObject *object)
{
	Utility::Logger::getInstance()->log("Object selected by player %s\n", name.c_str());
	selected = object;
}

MapObject* Player::getSelectedObject() const
{
	return selected;
}*/

bool Player::isDone() const
{
	return done;
}

void Player::setDone(bool done)
{
	this->done = done;
}

bool Player::isCurrent() const
{
	return current;
}

bool Player::hasEndedTurn() const
{
	return endedTurn;
}

void  Player::endTurn()
{
	Utility::Logger::getInstance()->log("Player %s : ended turn\n", name.c_str());
	endedTurn = true;
}

void Player::setCurrent(bool current)
{
	if(current)
		Utility::Logger::getInstance()->log("Player %s is now current player\n", name.c_str());
	this->current = current;
}

void Player::onTurnBegin()
{
	std::vector<Unit*>::iterator i;

	for(i = units.begin(); i != units.end(); ++i)
	{
		(*i)->onTurnBegin();
	}

	controller->newTurn();
	endedTurn = false;
}

void Player::setController(PlayerController *controller)
{
	this->controller = controller;
}

PlayerController* Player::getController() const
{
	return controller;
}

void Player::renderObjects(Graphics::Surface *target, const FieldOfView *fov, const Map *map)
{
	//FIXME: Add buildings
	std::vector<Unit*>::const_iterator i;

	int dx,dy;
	int tilex,tiley;

	for(i = units.begin(); i != units.end(); ++i)
	{
		tilex = (*i)->getTile()->getX();
		tiley = (*i)->getTile()->getY();

		if(!map->getClipping().isPointInRect(tilex, tiley))
			continue;

		//FIXME: Change coordinates when shifted
		dx = (tiley % 2) * TILE_WIDTH/2 + map->getFrame().x - map->getClipping().x * TILE_WIDTH;
		dy = TILE_HEIGHT - TILE_TERRAIN_HEIGHT - map->getFrame().y + map->getClipping().y * (TILE_HEIGHT / 2);

		if(fov->isTileVisible(tilex, tiley))
		{
			(*i)->draw(target,dx+(tilex*TILE_WIDTH),
				(tiley*(TILE_HEIGHT_OFFSET)-dy));
		}
	}
}
