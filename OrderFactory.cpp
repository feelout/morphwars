#include "OrderFactory.h"
#include "Order.h"
#include "Logger.h"
#include "PythonGlue.h"

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

bool OrderFactory::isRegistered(string name)
{
	return creators.count(name) > 0;
}

boost::shared_ptr<Order> OrderFactory::createOrder(string name, MapObject *object, Map *map)
{
	Logger::getInstance()->log("Order creation requested : %s\n", name.c_str());
	Logger::getInstance()->log("Order object : (%d,%d)\n", object->getTile()->getX(),
			object->getTile()->getY());
	if(creators.count(name) > 0)
		return creators[name]->createOrder(object, map);
	else
	{
		Utility::RunPythonScript("Orders/"+name+".py");
		if(creators.count(name) > 0) {
			//return creators[name]->createOrder(object, map);
			boost::shared_ptr<Order> order = creators[name]->createOrder(object, map);
			Logger::getInstance()->log("Object after running createOrder : (%d,%d)\n",
					object->getTile()->getX(), object->getTile()->getY());
			return order;
		} else
		{
			Utility::Logger::getInstance()->log("Failed to find order %s\n", name.c_str());
			return boost::shared_ptr<Order>();
		}
	}
}
