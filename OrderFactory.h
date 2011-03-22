#ifndef ORDER_FACTORY_H
#define ORDER_FACTORY_H

#include <string>
#include <map>
#include <boost/shared_ptr.hpp>

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
			virtual boost::shared_ptr<Order> createOrder(MapObject *object, Map *map)=0;
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
			bool isRegistered(string name);
			boost::shared_ptr<Order> createOrder(string name, MapObject *object, Map *map); 

	};
};

#endif //ORDER_FACTORY_H
