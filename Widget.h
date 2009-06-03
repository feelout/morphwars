#ifndef WIDGET_H
#define WIDGET_H

#include "EventDispatcher.h"
#include "assist.h"

namespace Gui
{
	enum SizePolicy
	{
		CONCRETE, //Cannot shrink or expand, use request
		MINIMUM, //Cannot shrink, can expand
		MAXIMUM, //Cannot expand, can shrink
		FRIENDLY //Will accept everything
	};
	/**
	* Widget is base GUI class
	*/
	class Widget : public IEventListener
	{
		private:
			/**
			* Bounding rect
			*/
			Rect frame;
			/**
			* Frame requested by widget
			*/
			Rect request;
			/**
			* SizePolicy on X axis
			*/
			SizePolicy xpolicy;
			/**
			* Size policy on Y axis
			*/
			SizePolicy ypolicy;
			/**
			* True if widget draws itself and responds to actions
			*/
			bool active;

			/**
			 * Parent widget
			 */
			Widget *parent;
		public:
			/**
			* Create widget with given bounding rect
			* @param rect bounding rect
			*/
			Widget(Rect rect, Widget *parent=NULL);
			/**
			* Pure virtual destructor
			*/
			virtual ~Widget()=0;
			
			/**
			* Shows whether widget is active
			* @return true if widget is active
			*/
			bool isActive() const;
			/**
			* Set widget`s active state
			* @param active state
			*/
			void setActive(bool active);
			
			/**
			* Checks if given point is inside the bounding rect
			* @param x x coord of the point
			* @param y y coord of the point
			* @return true if points is inside widget`s frame
			*/
			bool isPointInFrame(int x, int y) const;
			
			/**
			* Returns current widget frame
			* @return widget frame
			*/
			Rect getFrame() const;
			
			/**
			* Returns widget requested frame
			* @return requested frame
			*/
			Rect getRequestedFrame() const;

			/**
			 * Sets widget`s frame.
			 * Is virtual because containers need to adjust their
			 * children`s frames after call to this
			 * @param frame new widget frame
			 */
			virtual void setFrame(Rect frame);

			SizePolicy getHorizontalSizePolicy() const;
			SizePolicy getVerticalSizePolicy() const;

			virtual void setParent(Widget *parent);
			Widget* getParent() const;
	};
}

#endif //WIDGET_H
