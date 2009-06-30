#ifndef SCENARIO_H
#define SCENARIO_H

#include <string>
#include <list>
#include "Map.h"
#include "Player.h"

namespace Core
{
	class Scenario
	{
		private:
			/** Pass <map> TiXMLNode to Map class **/
			bool loadFromFile(std::string path);

			std::list<Player*> players;
			Map *map;
		public:
			Scenario(std::string path);
			~Scenario();
	};
}

#endif //SCENARIO_H
