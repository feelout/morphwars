#include "VBox.h"

using namespace Gui;

VBox::VBox(Rect frame, Widget *parent = NULL)
	: Box(frame, parent)
{
}

void VBox::calculateRequest()
{
	std::list<Widget*>::iterator i;	

	request.w = 0;
	request.h = 0;

	// Collecting minimum dimensions
	for(i = children.begin(); i != children.end(); ++i)
	{
		SizePolicy policy = *i->getHorizontalSizePolicy();

		if(policy != FRIENDLY && policy != MAXIMUM) //if can not shrink
		{
			request.w += *i->getRequestedFrame().w;
		}
		
		policy = *i->getVerticalSizePolicy();

		if(policy != FRIENDLY && policy != MAXIMUM)
		{
			if(request.h < *i->getRequestedFrame().h)
			{
				request.h = *i->getRequestedFrame().h;
			}
		}
	}

	xpolicy = MINIMUM;
	ypolicy = MINIMUM;
}

void VBox::organizeChildren()
{
	// TODO: Doesn`t yet handle CONCRETE vertical policy
	Box::setFrame(frame);

	std::list<Widget*>::iterator i;
	Rect newFrame = {frame.x, frame.y, frame.w, 0};

	if(frame.h == request.h) //We got what we want, not more, not less
	{
		// Set every children`s frame as requested
		for(i = children.begin(); i != children.end(); ++i)
		{
			newFrame.y += newFrame.h; //Shift by previous widget`s height
			newFrame.h = *i->getRequestedFrame().h;
			*i->setFrame(newFrame);
		}
	}
	else if(frame.h >= request.h) //We`ve got some extra space
	{
		// Give to expanding,minimum and friendly ones
		/*
		 * First of all, find out how many children allow expanding
		 */
		int widgetsAllowingExpanding = 0;

		for(i = children.begin(); i != children.end(); ++i)
		{
			SizePolicy vpolicy = *i->getVerticalSizePolicy();

			if( (policy == MINIMUM) || (policy == FRIENDLY) )
			{
				++widgetsAllowingExpanding;
			}
		}

		// Calculate amount of extra space to give every possible widget
		int extraSpacePerWidget = (frame.h - request.h) / widgetsAllowingExpanding;

		Rect newFrame = {frame.x, frame.y, frame.w, 0};

		for(i = children.begin(); i != children.end(); ++i)
		{
			newFrame.y += newFrame.h;
			SizePolicy vpolicy = *i->getVerticalSizePolicy();
			newFrame.h = *i->getRequestedFrame().h;
			//If can expand, add extra space
			if( (vpolicy == MINIMUM) || (vpolicy == FRIENDLY) )
			{
				newFrame.h += extraSpacePerWidget;
			}
			*i->setFrame(newFrame);
		}
	}
	else //We are short of space
	{
		// Shrink everything shrinkable
		int minimumHeight=0;
		int widgetsAllowingShrinking=0;

		std::list<Widget*>::iterator i;

		for(i = children.begin(); i != children.end(); ++i)
		{
			SizePolicy vpolicy = *i->getVerticalSizePolicy();

			if( (policy == MINIMUM) || (policy == CONCRETE) )
			{
				minimumHeight += *i->getRequestedFrame().h;
			}
			else
			{
				++widgetsAllowingShrinking;
			}
		}

		int spaceToShrinkPerWidget = (request.h - frame.h) / widgetsAllowingShrinking;

		Rect newFrame = {frame.x, frame.y, frame.w, 0};

		for(i = children.begin(); i != children.end(); ++i)
		{
			newFrame.y += newFrame.h;
			SizePolicy vpolicy = *i->getVerticalSizePolicy();
			newFrame.h = *i->getRequestedFrame().h;
			if( (vpolicy == MAXIMUM) || (vpolicy == FRIENDLY) )
			{
				newFrame.h -= spaceToShrinkPerWidget;
			}
			*i->setFrame(newFrame);
		}
	}
}

void VBox::setFrame(Rect frame)
{
	this.frame = frame;
	organizeChildren();
}
