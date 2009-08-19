#include "Widget.h"

using namespace Gui;

Widget::Widget(Rect frame)
	: active(true), focused(false)
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
}

bool Widget::isFocused() const
{
	return focused;
}

void Widget::setFocused(bool focused)
{
	this->focused = focused;
}

void Widget::mouseMoved(int x, int y)
{
}

void Widget::mouseLMBClicked(int x, int y)
{
}

void Widget::mouseRMBClicked(int x, int y)
{
}

void Widget::keyPressed(int key)
{
}
