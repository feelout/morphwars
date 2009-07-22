#ifndef ASTAR_H
#define ASTAR_H

#include "Tile.h"

namespace Core
{
	namespace AStar
	{
		class Node
		{
			private:
				Tile *src;
				Node *parent;
				int cost;
			public:
				Node(Node *parent, Tile *src, MovementType movementtype, Tile *target);

				int getCost() const;
				Tile *getSource() const;

				bool operator==(const Node &other) const;
		};
	}
}

#endif //ASTAR_H
