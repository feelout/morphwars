#ifndef WIDGET_H
#define WIDGET_H

#include "EventDispatcher.h"
#include "assist.h"

namespace Gui
{
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
			* True if widget draws itself and responds to actions
			*/
			bool active;
		public:
			/**
			* Create widget with given bounding rect
			* @param x x coord of the bounding rect
			* @param y y coord of the bounding rect
			* @param w width of the bounding rect
			* @param h height of the bounding rect
			*/
			Widget(int x, int y, int w, int h);
			/**
			* Create widget with given bounding rect
			* @param rect bounding rect
			*/
			Widget(Rect rect);
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
	};
}