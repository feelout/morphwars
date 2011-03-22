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

void OrderManager::addOrder(boost::shared_ptr<Order> order)
{
	/// XXX DEBUG
	Utility::Logger::getInstance()->log("OrderManager::addOrder(%s)\n", order->getText().c_str());
	if(order.get())
		Utility::Logger::getInstance()->log("OrderManager : order is not NULL\n");
	else
		Utility::Logger::getInstance()->log("OrderManager : order is NULL!!!\n");
	orders.push_back(order);
	Utility::Logger::getInstance()->log("OrderManager : total of %d orders now\n", orders.size());
}

void OrderManager::processOrders()
{
	std::vector< boost::shared_ptr<Order> >::iterator i;
	std::vector< boost::shared_ptr<Order> >::iterator j;

	for(i = orders.begin(); i != orders.end();)
	{
		Utility::Logger::getInstance()->log("Processing order %s\n", (*i)->getText().c_str());
		if((*i)->isDone())
		{
			Utility::Logger::getInstance()->log("Order is already done\n");
			//delete (*i);
			Order *ord = (*i).get();
			i = orders.erase(i); //XXX NB!!!! This invalidated pending ptr
			Utility::Logger::getInstance()->log("Orders remaining: %i\n", orders.size());
		}
		else
		{
			Utility::Logger::getInstance()->log("Order is not done\n");
			(*i++)->process();
		}
	}
}
