#ifndef FORCE_H
#define FORCE_H

#include <vector>

namespace Core
{
	class Player;

	class Force
	{
		private:
			std::vector<Player*> players;	
			int num;
		public:
			Force(int num);

			void addPlayer(Player *player);
			bool isPlayerInForce(Player *player);

			int getNumber() const;
	};
}

#endif //FORCE_H
