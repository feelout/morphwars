#include "OrderManager.h"

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
	orders.push_back(order);
}

void OrderManager::processOrders()
{
	std::vector<Order*>::const_iterator i;

	for(i = orders.begin(); i != orders.end(); ++i)
	{
		(*i)->process();
	}
}
