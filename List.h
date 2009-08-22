#ifndef LIST_H
#define LIST_H

#include <vector>
#include <String>
#include "Widget.h"

namespace Gui
{
	// Emitted when list item is clicked
	typedef void (*ListItemClickCallback)(std::string);
	// Emitted when list current item is changed
	typedef void (*ListCurrentItemChangedCallback)(std::string);

	class List : public Widget
	{
		public:
			static const int ITEM_HEIGHT = 25;
		protected:
			std::vector<std::string> items;
			int topItem; //for scrolling
			int currentItem;
			int maxItems; // how many items to show

			//ListItemClickCallback clickedCb;
			//ListCurrentItemChangedCallback itemChangedCb;

			virtual void itemSelected(int n)=0;
			virtual void currentItemChanged(int n)=0;
			virtual void frameUpdated();
			virtual void setCurrentItem(int n);
		public:
			List(Rect frame);
			List(Rect frame, std::vector<std::string> &content);

			//void setOnItemClicked(ListItemClickCallback cb);
			//void setOnCurrentItemChanged(ListCurrentItemChangedCallback cb);

			virtual void mouseMoved(int x, int y);
			virtual void mouseLMBClicked(int x, int y);

			virtual void draw(Graphics::Drawer *target);
	};
}

#endif //LIST_H
