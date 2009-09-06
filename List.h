#ifndef LIST_H
#define LIST_H

#include <vector>
#include <string>
#include "Widget.h"

namespace Gui
{
	// Emitted when list item is clicked
	typedef void (*ListItemClickCallback)(std::string);
	// Emitted when list current item is changed
	typedef void (*ListCurrentItemChangedCallback)(std::string);

	/**
	 * List widget - showes orderer vertical list of string items
	 */
	class List : public Widget
	{
		public:
			/* List item`s height */
			static const int ITEM_HEIGHT = 25;
		protected:
			std::vector<std::string> items;
			int topItem; //for scrolling
			int currentItem;
			int maxItems; // how many items to show

			//ListItemClickCallback clickedCb;
			//ListCurrentItemChangedCallback itemChangedCb;

			/* Called when item is selected (e.g. clicked) */
			virtual void itemSelected(int n)=0;
			/* Called when current item is changed (e.g. in mouse focus) */
			virtual void currentItemChanged(int n)=0;
			virtual void frameUpdated();
			virtual void setCurrentItem(int n);
		public:
			/** Creates empty list with given frame
			 * @param frame Widget`s frame
			 */
			List(Rect frame);
			/** Creates list and fills it with items
			 * @param frame Widget`s frame
			 * @param content Vector to copy items from
			 */
			List(Rect frame, std::vector<std::string> &content);

			//void setOnItemClicked(ListItemClickCallback cb);
			//void setOnCurrentItemChanged(ListCurrentItemChangedCallback cb);

			virtual bool mouseMoved(int x, int y);
			virtual bool mouseLMBClicked(int x, int y);

			virtual void draw(Graphics::Drawer *target);
	};
}

#endif //LIST_H
