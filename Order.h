#ifndef ORDER_H
#define ORDER_H

#include <string>
#include "Unit.h"
#include "Map.h"
#include "OrderFactory.h"

namespace Core
{
	/*class Order
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
			//DEBUG
			~Order();
			//ENDDEBUG

			virtual void process()=0;
			virtual void stop();
			bool isDone() const;
	};*/


	class Order
	{
		protected:
			MapObject *object;
			Map *map;
			Tile *target;

			bool done;
		public:
			Order(MapObject *unit, Map *map);
			virtual ~Order();

			virtual void execute(Tile *target);
			virtual void process();
			virtual void stop();

			bool isDone() const;

			virtual std::string getText() const;
	};

	class MovementOrder : public Order
	{
		protected:
			std::vector<Tile*> waypoints;
			std::vector<Tile*>::iterator currentWaypoint;

			bool makePath();
		public:
			MovementOrder(MapObject *object, Map *map);

			virtual void execute(Tile *target);
			virtual void process();
	};

	class MovementOrderCreator : public IOrderCreator
	{
		public:
			virtual Order* createOrder(MapObject *object, Map *map);
	};
}

#endif //ORDER_H
