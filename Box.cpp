#include "Box.h"
#include "Logger.h"

using namespace Gui;

Box::Box(Rect frame, int spacing, Widget *parent)
	: Container(frame, parent), spacing(spacing)
{
}

Box::~Box()
{
	std::vector<BoxEntry*>::iterator i;

	for(i = entries.begin(); i != entries.end(); ++i)
	{
		delete (*i);
	}
}

void Box::addChild(Widget *child, bool expandable, int spacing)
{
	Container::addChild(child);
	entries.push_back(new BoxEntry(child, expandable, spacing));
}

void Box::removeChild(Widget *child)
{
	Container::removeChild(child);
	std::vector<BoxEntry*>::iterator ch;

	std::vector<BoxEntry*>::iterator i;

	for(i = entries.begin(); i != entries.end(); ++i)
	{
		if((*i)->widget == child)
		{
			entries.erase(i);
			return;
		}
	}
}
/**********************/
/* HBox class members */
/**********************/
HBox::HBox(Rect frame, int spacing, Widget *parent)
	: Box(frame, spacing, parent)
{
}

void HBox::recalculateChildrenFrames()
{
	std::vector<BoxEntry*>::iterator i;

	int requestedWidth = 0;
	int numOfExpandableWidgets = 0;
	int spacingNeeded = spacing;
	int requestedHeight = 0;

	for(i = entries.begin(); i != entries.end(); ++i)
	{
		if((*i)->expandable)
		{
			numOfExpandableWidgets += 1;
		}
		
		/* Spacing passed to box constructor is always added between widgets.
		 * On child adding additional spacing for that child is set */
		spacingNeeded += ((*i)->spacing * 2 + spacing);

		Rect childRequest = (*i)->widget->getRequestedFrame();

		requestedWidth += childRequest.w;

		if(childRequest.h > requestedHeight) 
		{
			requestedHeight = childRequest.h;
		}
	}

	requestedFrame.w = requestedWidth + spacingNeeded;
	requestedFrame.h = requestedHeight;

	if(requestedFrame.w > frame.w || requestedFrame.h > frame.h)
	{
		// FIXME: Think of something to do (maybe parent for all widgets will be sublcall
		// of Container, which will be responsible for space allocation
		Utility::Logger::getInstance()->log("HBox: not enough space for placing all widgets\n");
		return;
	}

	// Amount of additional space allocated to expandable widgets
	int additionalSpace = (frame.w - requestedFrame.w) / numOfExpandableWidgets;

	// Next child x coordinate
	int insertionPoint = frame.x+spacing;

	for(i = entries.begin(); i != entries.end(); ++i)
	{
		insertionPoint += (*i)->spacing;
		Rect childFrame = (*i)->widget->getRequestedFrame();

		if((*i)->expandable)
		{
			childFrame.w += additionalSpace;
		}

		childFrame.x = insertionPoint;
		childFrame.y = frame.y;

		(*i)->widget->setFrame(childFrame);

		insertionPoint += (childFrame.w + (*i)->spacing + spacing);
	}
}
