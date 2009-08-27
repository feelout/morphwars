#ifndef WIDGET_H
#define WIDGET_H

#include "EventDispatcher.h"
#include "Drawer.h"

namespace Gui
{
	class Widget : public IEventListener
	{
		protected:
			Rect frame;
			bool active;
			bool focused;

			/**
			 * Is called, when widget frame changes
			 */
			virtual void frameUpdated();
		public:
			Widget(Rect frame);

			Rect getFrame() const;
			void setFrame(Rect frame);

			bool isFocused() const;
			void setFocused(bool focused);

			virtual void draw(Graphics::Drawer *target)=0;

			virtual bool mouseMoved(int x, int y);
			virtual bool mouseLMBClicked(int x, int y);
			virtual bool mouseRMBClicked(int x, int y);
			virtual bool keyPressed(int key);
	};
}

#endif //WIDGET_H
