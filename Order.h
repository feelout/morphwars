#ifndef ORDER_H
#define ORDER_H

#include <string>
#include "Unit.h"
#include "Map.h"
#include "OrderFactory.h"

namespace Core
{
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

	class AttackOrder : public Order
	{
		public:
			AttackOrder(MapObject *object, Map *map);

			virtual void execute(Tile *target);
			virtual void process();
	};

	class AttackOrderCreator : public IOrderCreator
	{
		public:
			virtual Order* createOrder(MapObject *object, Map *map);
	};
}

#endif //ORDER_H
