#ifndef WIDGET_H
#define WIDGET_H

#include "EventDispatcher.h"
#include "Surface.h"

namespace Gui
{
	class Widget : public IEventListener
	{
		protected:
			Rect frame; // Widget frame in pixels
			Rect requestedFrame; // Contains minimal width and height of widget	
			Widget *parent;	// Widget parent, responsible for space allocation
			bool active; // Does widget respond to events?
			bool focused; // Is currently in focus?

			/**
			 * Is called, when widget frame changes
			 */
			virtual void frameUpdated();
		public:
			Widget(Rect frame, Widget *parent=NULL);

			Rect getFrame() const;
			void setFrame(Rect frame);

			Rect getRequestedFrame() const;

			bool isFocused() const;
			void setFocused(bool focused);

			Widget* getParent() const;
			void setParent(Widget *parent);

			virtual void draw(Graphics::Surface *target)=0;

			virtual bool mouseMoved(int x, int y);
			virtual bool mouseLMBClicked(int x, int y);
			virtual bool mouseRMBClicked(int x, int y);
			virtual bool keyPressed(int key);
	};
}

#endif //WIDGET_H
