#include "Container.h"
#include "Logger.h"
#include <algorithm>

using namespace Gui;

Container::Container(Rect frame, bool strict)
	: Widget(frame), strict(strict), focusedWidget(NULL)
{
}

bool Container::addChild(Widget *child)
{
	if(strict)
	{
		if(!frame.isRectInRect(child->getFrame()))
		{
			return false;
		}
	}

	children.push_back(child);
	focusedWidget = child;
	child->setFocused(true);
}

bool Container::removeChild(Widget *child)
{
	std::list<Widget*>::iterator i;

	i = std::find(children.begin(), children.end(), child);

	if(i == children.end())
		return false;

	if(child == focusedWidget)
	{
		focusedWidget = NULL;
		child->setFocused(false);
	}

	children.erase(i);
	return true;
}

void Container::draw(Graphics::Drawer *target)
{
	std::list<Widget*>::iterator i;

	for(i = children.begin(); i != children.end(); ++i)
	{
		(*i)->draw(target);
	}
}

bool Container::mouseMoved(int x, int y)
{
	std::list<Widget*>::iterator i;

	for(i = children.begin(); i != children.end(); ++i)
	{
		if((*i)->mouseMoved(x, y))
			return true;
	}
	
	return false;
}

bool Container::mouseLMBClicked(int x, int y)
{
	std::list<Widget*>::iterator i;

	for(i = children.begin(); i != children.end(); ++i)
	{
		if((*i)->mouseLMBClicked(x, y))
			return true;
	}

	return false;
}

bool Container::mouseRMBClicked(int x, int y)
{
	std::list<Widget*>::iterator i;

	for(i = children.begin(); i != children.end(); ++i)
	{
		if((*i)->mouseRMBClicked(x, y))
			return true;
	}

	return false;
}

bool Container::keyPressed(int key)
{
	std::list<Widget*>::iterator i;

	for(i = children.begin(); i != children.end(); ++i)
	{
		if((*i)->keyPressed(key))
			return true;
	}

	return false;
}
