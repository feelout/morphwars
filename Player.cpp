#include <algorithm>
#include "Player.h"
#include "Logger.h"
#include "Tile.h"
#include "Unit.h"

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

	for(int i=0; i < w*h; ++i)
	{
		tiles[i] = false;
	}
}

bool FieldOfView::isTileVisible(int x, int y)
{
	return tiles[x+y*w];
}

void FieldOfView::setTileVisible(int x, int y, bool visible)
{
	if((x < 0) || (x >= w) || (y < 0) || (y >= h)) return;

	tiles[x+y*w] = visible;
}

void Player::updateFOV()
{
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
	: name(name), fraction(fraction), force(force), color(color), energy(0)
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
