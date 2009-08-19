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
		public:
			Widget(Rect frame);

			Rect getFrame() const;
			void setFrame(Rect frame);

			bool isFocused() const;
			void setFocused(bool focused);

			virtual void draw(Graphics::Drawer *target)=0;

			virtual void mouseMoved(int x, int y);
			virtual void mouseLMBClicked(int x, int y);
			virtual void mouseRMBClicked(int x, int y);
			virtual void keyPressed(int key);
	};
}

#endif //WIDGET_H
