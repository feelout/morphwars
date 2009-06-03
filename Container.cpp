#include "Container.h"

using namespace Gui;

Container::Container(Rect rect, Widget *parent = NULL)
	: Widget(rect, parent)
{
}

Container::~Container()
{
	children.clear(); // TODO: find out, whether it calls destructors
}

void Container::addWidget(Widget *child)
{
	children.push_back(child);
	child->setParent(this);
	organiseChildren();
}

void Container::removeWidget(Widget *child)
{
	//TODO: maybe there is a better way
	std::list<Widget*>::iterator i;
	
	for(i = children.begin(); i !+ children.end(); ++i)
	{
		if(*i == child)
		{
			delete *i;
			return;
		}
	}
	
	organiseChildren();
}

void Container::mouseMoved(int x, int y)
{
	std::list<Widget*>::iterator i;
	
	for(i = children.begin(); i != children.end(); ++i)
	{
		if( (*i)->isActive() && (*i)->isPointInRect(x, y) )
			*i->mouseMoved(x, y);
	}
}

void Container::mouseLMBClicked(int x, int y)
{
	std::list<Widget*>::iterator i;
	
	for(i = children.begin(); i != children.end(); ++i)
	{
		if( (*i)->isActive() && (*i)->isPointInRect(x, y) )
			*i->mouseLMBClicked(x, y);
	}
}

void Container::mouseRMBClicked(int x, int y)
{
	std::list<Widget*>::iterator i;
	
	for(i = children.begin(); i != children.end(); ++i)
	{
		if( (*i)->isActive() && (*i)->isPointInRect(x, y) )
			*i->mouseRMBClicked(x, y);
	}
}

void Container::keyPressed(int key)
{
	std::list<Widget*>::iterator i;
	
	for(i = children.begin(); i != children.end(); ++i)
	{
		if( (*i)->isActive() && (*i)->isPointInRect(x, y) )
			*i->keyPressed(key);
	}
}

void Container::setFrame(Rect frame)
{
	Widget::setFrame(frame);
	organizeChildren();
}
