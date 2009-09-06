#include "Widget.h"
#include "Logger.h"

using namespace Gui;

Widget::Widget(Rect frame)
	: active(true), focused(false)
{
	setFrame(frame);
}

void Widget::frameUpdated()
{
	Utility::Logger::getInstance()->log("Widget::frameUpdated()\n");
}

Rect Widget::getFrame() const
{
	return frame;
}

void Widget::setFrame(Rect frame)
{
	Utility::Logger::getInstance()->log("Widget::setFrame()\n");
	this->frame = frame;

	frameUpdated();
}

bool Widget::isFocused() const
{
	return focused;
}

void Widget::setFocused(bool focused)
{
	this->focused = focused;
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
