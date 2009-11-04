#ifndef ORDER_PACK_H
#define ORDER_PACK_H

#include "Container.h"

namespace Gui
{
	/**
	 * OrderPack holds all orders unit can perform
	 */
	class OrderPack : public Container
	{
		public:
			static const int ORDER_BUTTON_WIDTH = 40;
			static const int ORDER_BUTTON_HEIGHT = 40;
		public:
			OrderPack(Rect frame);

			virtual void draw(Graphics::Drawer *target);
	};
};

#endif //ORDER_PACK_H
