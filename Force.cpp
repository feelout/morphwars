#include "Force.h"
#include "Player.h"

using namespace Core;

Force::Force(int num)
	: num(num)
{
}

void Force::addPlayer(Player *player)
{

	if(!isPlayerInForce(player))
	{
		players.push_back(player);
	}
}

bool Force::isPlayerInForce(Player *player)
{

	std::vector<Player*>::iterator i;

	for(i = players.begin(); i != players.end(); ++i)
	{
		if((*i) == player)
		{
			return true;
		}
	}

	return false;
}

int Force::getNumber() const
{
	return num;
}
