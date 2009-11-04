#ifndef MINIMAP_H
#define MINIMAP_H

#include "Widget.h"
#include "Surface.h"

namespace Core
{
	class Map;
}

namespace Gui
{
	class Minimap : public Widget
	{
		private:
			Graphics::Surface *minimap;
			Core::Map *owner;
		public:
			Minimap(Rect frame, Core::Map *owner);

			virtual bool mouseLMBClicked(int x, int y);
			virtual void draw(Graphics::Drawer *target);
	};
}

#endif //MINIMAP_H

