#ifndef CONTAINER_H
#define CONTAINER_H

#include <vector>
#include "Widget.h"

/**
 * Container is the widget holding other widgets
 */
class Container : public Widget
{
	private:
		/**
		 * Collection of all child widgets
		 */
		std::vector<Widget*> children;
		
	public:
		/**
		 * Constructor inherited from Widget
		 * @param x x coord of bounding rect
		 * @param y y coord of bounding rect
		 * @param w width of bounding rect
		 * @param h height of bounding rect
		 */
		Container(int x, int y, int w, int h);
		/**
		 * Constructor inherited from Widget
		 * @param rect bounding rect
		 */
		Container(Rect rect);
		
		/**
		 * Call all children`s destructors
		 */
		~Container();
		
		/**
		 * Adds widget to container
		 * @param child widget to be added
		 */
		void addChild(Widget *child);
		
		virtual void mouseMoved(int x, int y);
		virtual void mouseLMBClicked(int x, int y);
		virtual void mouseRMBClicked(int x, int y);
		virtual void keyPressed(int key);
};