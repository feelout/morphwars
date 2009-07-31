#include "OrderManager.h"
#include "Logger.h"

using namespace Core;

OrderManager* OrderManager::instance = NULL;

OrderManager* OrderManager::getInstance()
{
	if(!instance)
	{
		instance = new OrderManager();
	}
	return instance;
}

void OrderManager::addOrder(Order *order)
{
	Utility::Logger::getInstance()->log("OrderManager::addOrder()\n");
	orders.push_back(order);
}

void OrderManager::processOrders()
{
	std::vector<Order*>::iterator i;

	for(i = orders.begin(); i != orders.end();)
	{
		Utility::Logger::getInstance()->log("Processing order\n");
		if((*i)->isDone())
		{
			Utility::Logger::getInstance()->log("Order is already done\n");
			delete (*i);
			i = orders.erase(i);
			Utility::Logger::getInstance()->log("Orders remaining: %i\n", orders.size());
		}
		else
		{
			(*i++)->process();
		}
	}
}
