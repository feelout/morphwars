#ifndef CONTAINER_H
#define CONTAINER_H

#include "Widget.h"
#include <list>

namespace Gui
{
	class Container : public Widget
	{
		protected:
			std::list<Widget*> children;
			Widget *focusedWidget;
			bool strict;
		public:
			/** Container constructor
			 * @param frame Container widget frame
			 * @param string If true, container will not allow
			 *  adding children outside of container`s frame
			 */
			Container(Rect frame, bool strict=false);

			bool addChild(Widget *child);
			bool removeChild(Widget *child);

			virtual void draw(Graphics::Drawer *target);
			virtual bool mouseMoved(int x, int y);
			virtual bool mouseLMBClicked(int x, int y);
			virtual bool mouseRMBClicked(int x, int y);
			virtual bool keyPressed(int key);
	};
}

#endif //CONTAINER_H
