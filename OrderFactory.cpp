#include "OrderFactory.h"
#include "Order.h"
#include "Logger.h"

using namespace Core;
using Utility::Logger;

OrderFactory* OrderFactory::instance = NULL;

OrderFactory* OrderFactory::getInstance()
{
	if(!instance)
	{
		instance = new OrderFactory();
	}
	return instance;
}

void OrderFactory::registerCreator(string name, IOrderCreator *creator)
{
	creators[name] = creator;
	Logger::getInstance()->log("Order type registered : %s\n", name.c_str());
}

Order* OrderFactory::createOrder(string name, MapObject *object, Map *map)
{
	Logger::getInstance()->log("Order creation requested : %s\n", name.c_str());
	if(creators.count(name) > 0)
		return creators[name]->createOrder(object, map);
	else
		return 0;
}
