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
	//Utility::Logger::getInstance()->log("Box::addChild()\n");
	entries.push_back(new BoxEntry(child, expandable, spacing));
	Container::addChild(child);
}

void Box::removeChild(Widget *child)
{
	Container::removeChild(child);
	//Utility::Logger::getInstance()->log("Box::removeChild()\n");

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

void Box::removeAll()
{
	Container::removeAll();
	std::vector<BoxEntry*>::iterator i;

	for(i = entries.begin(); i != entries.end(); ++i)
	{
		delete (*i);
	}

	entries.clear();
}

void Box::recalculateChildrenFrames()
{
	//Utility::Logger::getInstance()->log("Box : Recalculating children frames\n");
	//Utility::Logger::getInstance()->log("Box : Total %i child widgets\n", entries.size());
	std::vector<BoxEntry*>::iterator i;

	int requestedDimension = 0;
	int numOfExpandableWidgets = 0;
	int spacingNeeded = spacing;
	int requestedSubDimension = 0;

	requestedFrame.w = 0;
	requestedFrame.h = 0;

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

		//Utility::Logger::getInstance()->log("childRequest = (%i,%i)\n", childRequest.w, childRequest.h);

		requestedDimension += getDimension(childRequest);

		if(getSubDimension(childRequest) > requestedSubDimension) 
		{
			requestedSubDimension = getSubDimension(childRequest);
		}
	}

	setDimension(requestedFrame, getDimension(requestedFrame) + requestedDimension + spacingNeeded);
	//setSubDimension(requestedFrame, getSubDimension(requestedFrame) + spacing * 2);
	
	setSubDimension(requestedFrame, requestedSubDimension + spacing * 2);

	//Utility::Logger::getInstance()->log("Requested frame : %i,%i\n", requestedFrame.w, requestedFrame.h);

	if(requestedFrame.w > frame.w || requestedFrame.h > frame.h)
	{
		// FIXME: Think of something to do (maybe parent for all widgets will be sublcall
		// of Container, which will be responsible for space allocation
		//Utility::Logger::getInstance()->log("HBox: not enough space for placing all widgets\n");
		return;
	}

	// Amount of additional space allocated to expandable widgets
	int additionalSpace = 0;
	if(numOfExpandableWidgets > 0)
	{
		additionalSpace = (getDimension(frame) - getDimension(requestedFrame)) / numOfExpandableWidgets;
	}
	
	//Utility::Logger::getInstance()->log("Total additional space : %i\n", (getDimension(frame) - getDimension(requestedFrame)));
	//Utility::Logger::getInstance()->log("Additional space for widgets : %i\n", additionalSpace);

	// Next child x coordinate
	int insertionPoint = getMinorDimension(frame) + spacing;

	for(i = entries.begin(); i != entries.end(); ++i)
	{
		insertionPoint += (*i)->spacing;
		Rect childFrame = (*i)->widget->getRequestedFrame();

		//Utility::Logger::getInstance()->log("Child requested %i,%i\n", childFrame.w, childFrame.h);

		if((*i)->expandable)
		{
			setDimension(childFrame, getDimension(childFrame) + additionalSpace);
		}

		setSubDimension(childFrame, getSubDimension(frame) - 2 * spacing);

		setMinorDimension(childFrame, insertionPoint);
		setMinorSubDimension(childFrame, getMinorSubDimension(frame) + spacing);

		//Utility::Logger::getInstance()->log("Child got frame %i,%i,%i,%i\n", childFrame.x,
		//		childFrame.y, childFrame.w, childFrame.h);

		(*i)->widget->setFrame(childFrame);

		insertionPoint += (getDimension(childFrame) + (*i)->spacing + spacing);
	}
}

/**********************/
/* HBox class members */
/**********************/

int HBox::getDimension(Rect &childFrame)
{
	return childFrame.w;
}

void HBox::setDimension(Rect &childFrame, int value)
{
	childFrame.w = value;
}

void HBox::setMinorDimension(Rect &childFrame, int value)
{
	childFrame.x = value;
}

int HBox::getMinorDimension(Rect &childFrame)
{
	return childFrame.x;
}

int HBox::getSubDimension(Rect &childFrame)
{
	return childFrame.h;
}

void HBox::setSubDimension(Rect &childFrame, int value)
{
	childFrame.h = value;
}

void HBox::setMinorSubDimension(Rect &childFrame, int value)
{
	childFrame.y = value;
}

int HBox::getMinorSubDimension(Rect &childFrame)
{
	return childFrame.y;
}

HBox::HBox(Rect frame, int spacing, Widget *parent)
	: Box(frame, spacing, parent)
{
}

/*
 * VBox members
 */

int VBox::getSubDimension(Rect &childFrame)
{
	return childFrame.w;
}

void VBox::setSubDimension(Rect &childFrame, int value)
{
	childFrame.w = value;
}

void VBox::setMinorSubDimension(Rect &childFrame, int value)
{
	childFrame.x = value;
}

int VBox::getMinorSubDimension(Rect &childFrame)
{
	return childFrame.x;
}

int VBox::getDimension(Rect &childFrame)
{
	return childFrame.h;
}

void VBox::setDimension(Rect &childFrame, int value)
{
	childFrame.h = value;
}

void VBox::setMinorDimension(Rect &childFrame, int value)
{
	childFrame.y = value;
}

int VBox::getMinorDimension(Rect &childFrame)
{
	return childFrame.y;
}

VBox::VBox(Rect frame, int spacing, Widget *parent)
	: Box(frame, spacing, parent)
{
}

/*void HBox::recalculateChildrenFrames()
{
	Utility::Logger::getInstance()->log("HBox : Recalculating children frames\n");
	Utility::Logger::getInstance()->log("HBox : Total %i child widgets\n", entries.size());
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
		}*/
		
		/* Spacing passed to box constructor is always added between widgets.
		 * On child adding additional spacing for that child is set */
		/*spacingNeeded += ((*i)->spacing * 2 + spacing);

		Rect childRequest = (*i)->widget->getRequestedFrame();

		requestedWidth += childRequest.w;

		if(childRequest.h > requestedHeight) 
		{
			requestedHeight = childRequest.h;
		}
	}

	requestedFrame.w = requestedWidth + spacingNeeded;
	requestedFrame.h = requestedHeight + spacing * 2;

	Utility::Logger::getInstance()->log("Requested frame : %i,%i\n", requestedFrame.w, requestedFrame.h);

	if(requestedFrame.w > frame.w || requestedFrame.h > frame.h)
	{
		// FIXME: Think of something to do (maybe parent for all widgets will be sublcall
		// of Container, which will be responsible for space allocation
		Utility::Logger::getInstance()->log("HBox: not enough space for placing all widgets\n");
		return;
	}

	// Amount of additional space allocated to expandable widgets
	int additionalSpace = 0;
	if(numOfExpandableWidgets > 0)
	{
		additionalSpace = (frame.w - requestedFrame.w) / numOfExpandableWidgets;
	}

	// Next child x coordinate
	int insertionPoint = frame.x+spacing;

	for(i = entries.begin(); i != entries.end(); ++i)
	{
		insertionPoint += (*i)->spacing;
		Rect childFrame = (*i)->widget->getRequestedFrame();

		Utility::Logger::getInstance()->log("Child requested %i,%i\n", childFrame.w, childFrame.h);

		if((*i)->expandable)
		{
			childFrame.w += additionalSpace;
		}

		childFrame.h = frame.h - 2 * spacing;

		childFrame.x = insertionPoint;
		childFrame.y = frame.y + spacing;

		Utility::Logger::getInstance()->log("Child got frame %i,%i,%i,%i\n", childFrame.x,
				childFrame.y, childFrame.w, childFrame.h);

		(*i)->widget->setFrame(childFrame);

		insertionPoint += (childFrame.w + (*i)->spacing + spacing);
	}
}*/
