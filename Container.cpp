#include "Container.h"

Container::Container(int x, int y, int w, int h)
	: Widget(x,y,w,h)
{
}

Container::Container(Rect rect)
	: Widget(rect)
{
}

Container::~Container()
{
	children.clear(); // TODO: find out, whether it calls destructors
}

void Container::addChild(Widget *child)
{
	children.push_back(child);
}

void Container::mouseMoved(int x, int y)
{
	std::vector<Widget*>::iterator i;
	
	for(i = children.begin(); i != children.end(); i++)
	{
		if( (*i)->isActive() && (*i)->isPointInRect(x, y) )
			*i->mouseMoved(x, y);
	}
}

void Container::mouseLMBClicked(int x, int y)
{
	std::vector<Widget*>::iterator i;
	
	for(i = children.begin(); i != children.end(); i++)
	{
		if( (*i)->isActive() && (*i)->isPointInRect(x, y) )
			*i->mouseLMBClicked(x, y);
	}
}

void Container::mouseRMBClicked(int x, int y)
{
	std::vector<Widget*>::iterator i;
	
	for(i = children.begin(); i != children.end(); i++)
	{
		if( (*i)->isActive() && (*i)->isPointInRect(x, y) )
			*i->mouseRMBClicked(x, y);
	}
}

void Container::keyPressed(int key)
{
	std::vector<Widget*>::iterator i;
	
	for(i = children.begin(); i != children.end(); i++)
	{
		if( (*i)->isActive() && (*i)->isPointInRect(x, y) )
			*i->keyPressed(key);
	}
}