#include "Widget.h"
#include "Logger.h"

using namespace Gui;

Widget::Widget(Rect frame, Widget *parent)
	: active(true), focused(false), requestedFrame(frame)
{
	setFrame(frame);
}

void Widget::frameUpdated()
{
}

Rect Widget::getFrame() const
{
	return frame;
}

void Widget::setFrame(Rect frame)
{
	this->frame = frame;

	frameUpdated();
}

Rect Widget::getRequestedFrame() const
{
	return requestedFrame;
}

bool Widget::isFocused() const
{
	return focused;
}

void Widget::setFocused(bool focused)
{
	this->focused = focused;
}

Widget* Widget::getParent() const
{
	return parent;
}

void Widget::setParent(Widget *parent)
{
	this->parent = parent;
}

bool Widget::mouseMoved(int x, int y)
{
	return false;
}

bool Widget::mouseLMBClicked(int x, int y)
{
	return false;
}

bool Widget::mouseRMBClicked(int x, int y)
{
	return false;
}

bool Widget::keyPressed(int key)
{
	return false;
}
