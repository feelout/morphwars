#include "Container.h"
#include "Logger.h"
#include <algorithm>

using namespace Gui;

Container::Container(Rect frame, Widget *parent)
	: Widget(frame, parent), focusedWidget(NULL)
{
}

void Container::frameUpdated()
{
	recalculateChildrenFrames();
}

bool Container::addChild(Widget *child)
{
	//Utility::Logger::getInstance()->log("Container::addChild()\n");
	children.push_back(child);
	focusedWidget = child;
	child->setFocused(true);
	recalculateChildrenFrames();
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
		child->setParent(NULL);
		child->setFocused(false);
	}

	children.erase(i);

	recalculateChildrenFrames();
	return true;
}

void Container::removeAll()
{
	std::list<Widget*>::iterator i;
	
	for(i = children.begin(); i != children.end(); ++i)
	{
		(*i)->setParent(NULL);
	}

	children.clear();
}

void Container::recalculateChildrenFrames()
{
}

void Container::draw(Graphics::Surface *target)
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
