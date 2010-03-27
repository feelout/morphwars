#include "Placeholder.h"
#include "Logger.h"

using namespace Gui;

Placeholder::Placeholder(Rect frame, Widget *parent)
	: Widget(frame, parent), child(NULL)
{
}

void Placeholder::frameUpdated()
{
	if(child)
		child->setFrame(frame);
}

void Placeholder::setChild(Widget *child)
{
	this->child = child;
	this->child->setFrame(frame);
}

void Placeholder::draw(Graphics::Surface *target)
{
	//Utility::Logger::getInstance()->log("Placeholder %i :: draw\n", number);
	if(child)
	{
		child->draw(target);
	}
}
