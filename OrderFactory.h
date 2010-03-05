#ifndef ORDER_FACTORY_H
#define ORDER_FACTORY_H

#include <string>
#include <map>

using namespace std;

namespace Core
{
	class Order;
	class MapObject;
	class Map;

	/* Interface for creating different orders. Subclass for each order */
	class IOrderCreator
	{
		public:
			virtual Order* createOrder(MapObject *object, Map *map)=0;
	};

	/* Creates orders by name */
	class OrderFactory
	{
		private:
			static OrderFactory *instance;

			map<string, IOrderCreator*> creators;
		public:
			static OrderFactory* getInstance();

			void registerCreator(string name, IOrderCreator *creator);
			Order* createOrder(string name, MapObject *object, Map *map); 

	};
};

#endif //ORDER_FACTORY_H
