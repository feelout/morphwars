#ifndef CONTAINER_H
#define CONTAINER_H

#include <list>
#include "Widget.h"

namespace Gui
{
	/**
	* Container is the widget holding other widgets
	*/
	class Container : public Widget
	{
		protected:
			/**
			* Collection of all child widgets
			*/
			std::list<Widget*> children;
			
			/**
			* Put children on their places, changing their frames
			* Main method for each container type
			*/
			virtual void organiseChildren()=0;
			virtual void calculateRequest()=0;
		public:
			/**
			* Constructor inherited from Widget
			* @param rect bounding rect
			*/
			Container(Rect frame, Widget *parent = NULL);
			
			/**
			* Call all children`s destructors
			*/
			~Container();
			
			/**
			* Adds widget to container
			* @param child widget to be added
			*/
			void addWidget(Widget *child);
			
			/**
			* Removes widget from the container
			* @param child widget to be removed
			*/
			void removeWidget(Widget *child);
			
			virtual void mouseMoved(int x, int y);
			virtual void mouseLMBClicked(int x, int y);
			virtual void mouseRMBClicked(int x, int y);
			virtual void keyPressed(int key);

			virtual void setFrame(Rect frame);
	};
}
