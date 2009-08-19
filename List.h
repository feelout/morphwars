#ifndef LIST_H
#define LIST_H

#include <vector>
#include <String>
#include "Widget.h"

namespace Gui
{
	class List : public Widget
	{
		public:
			static const int ITEM_HEIGHT = 25;
		protected:
			std::vector<std::string> items;
			int topItem; //for scrolling
			int currentItem;
			int maxItems; // how many items to show

			virtual void itemSelected(int n)=0;
			virtual void frameUpdated();
		public:
			//TODO,FIXME: Remember to set frame
			List(Rect frame);
			List(Rect frame, std::vector<std::string> &content);

			virtual void mouseMoved(int x, int y);
			virtual void mouseLMBClicked(int x, int y);

			virtual void draw(Graphics::Drawer *target);
	};
}

#endif //LIST_H
