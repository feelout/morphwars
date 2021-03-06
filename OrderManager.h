#ifndef ORDER_MANAGER_H
#define ORDER_MANAGER_H

#include <vector>
#include "Order.h"

//TODO: add OrderManager::processOrders() & AnimationManager::processAnimations to game cycle
namespace Core
{
	class OrderManager
	{
		private:
			static OrderManager* instance;

			std::vector< boost::shared_ptr<Order> > orders;
		public:
			static OrderManager* getInstance();

			void addOrder(boost::shared_ptr<Order> order);
			void processOrders();
	};
}

#endif //ORDER_MANAGER_H
