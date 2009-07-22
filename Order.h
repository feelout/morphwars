#ifndef ORDER_H
#define ORDER_H

#include "Unit.h"
#include "Map.h"

namespace Core
{
	class Order
	{
		protected:
			// Unit, whom the order was issued
			Unit *unit;
			// Image to show on action button
			Graphics::Surface *buttonImage;
			// Image to show next to unit
			Graphics::Surface *unitImage;
			//TODO: Add separation on active/passive
			
			bool done;
		public:
			Order(Unit *unit);

			virtual void process()=0;
			virtual void stop();
			bool isDone() const;
	};

	class MovementOrder : Order
	{
		protected:
			Tile* target;
			Map *map;
			std::vector<Tile*> waypoints;
			std::vector<Tile*>::iterator currentWaypoint;

			bool makePath();
		public:
			MovementOrder(Unit *unit, Tile *target, Map *map);

			virtual void process();
	};
}

#endif //ORDER_H
