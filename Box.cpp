#include "Box.h"

using namespace Gui;

Box::Box(Rect rect, Widget *parent = NULL)
	: Container(rect, parent)
{
}

void Box::addForward(Widget *widget)
{
	children.push_front(widget);
	calculateRequest();
	organiseChildren();
}

void Box::addBackward(Widget *widget)
{
	children.push_back(widget);
	calculateRequest();
	organiseChildren();
}

void Box::setFrame(Rect frame)
{
	Contianer::setFrame(frame);
}
