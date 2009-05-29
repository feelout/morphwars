#include "Widget.h"

using namespace Gui;

Widget::Widget(int x, int y, int w, int h)
{
	frame.x = x;
	frame.y = y;
	frame.w = w;
	frame.h = h;
	
	active = true;
}

Widget::Widget(Rect rect)
{
	this->frame = rect;
	
	active = true;
}

bool Widget::isActive()
{
	return active;
}

void Widget::setActive(bool active)
{
	this->active = active;
}

bool Widget::isPointInFrame(int x, int y)
{
	return frame.isPointInRect(x,y);
}
