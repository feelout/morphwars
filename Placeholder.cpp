#include "Placeholder.h"

using namespace Gui;

Placeholder::Placeholder(Rect frame, Widget *parent)
	: Widget(frame, parent)
{
}

void Placeholder::frameUpdated()
{
	child->setFrame(frame);
}

void Placeholder::setChild(Widget *child)
{
	this->child = child;
	this->child->setFrame(frame);
}

void Placeholder::draw(Graphics::Surface *target)
{
	child->draw(target);
}
