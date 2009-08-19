#include "Button.h"
#include "Logger.h"

using namespace Gui;

Button::Button(Rect frame)
	: Widget(frame), callback(NULL)
{
}

void Button::setOnClicked(ButtonClickCallback cb)
{
	this->callback = cb;
}

void Button::mouseLMBClicked(int x, int y)
{
	if(frame.isPointInRect(x,y))
	{
		callback();
	}
}

ImageButton::ImageButton(Rect frame, Graphics::Surface surface)
	: Button(frame), surface(surface)
{
	Utility::Logger::getInstance()->log("ImageButton: (%i,%i,%i,%i), surface (%i,%i)\n",
			frame.x, frame.y, frame.w, frame.h, this->surface.getWidth(), this->surface.getHeight());
}

void ImageButton::draw(Graphics::Drawer *target)
{
	surface.blit(target->getTarget(), frame.x, frame.y);
}