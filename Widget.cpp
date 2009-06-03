#include "Widget.h"

using namespace Gui;

Widget::Widget(Rect rect, Widget *parent = NULL)
{
	//this->frame = rect;
	setFrame(rect);
	setParent(parent);
	setActive(true);	
	
	xpolicy = FRIENDLY;
	ypolicy = FRIENDLY;
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

void Widget::setParent(Widget *parent)
{
	this->parent = parent;
}

Widget* Widget::getParent()
{
	return parent;
}
