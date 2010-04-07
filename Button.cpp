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

bool Button::mouseLMBClicked(int x, int y)
{
	if(frame.isPointInRect(x,y))
	{
		callback();
		return true;
	}

	return false;
}

ImageButton::ImageButton(Rect frame, Graphics::Surface surface)
	: Button(frame), surface(surface)
{
	//Utility::Logger::getInstance()->log("ImageButton: (%i,%i,%i,%i), surface (%i,%i)\n",
			//frame.x, frame.y, frame.w, frame.h, this->surface.getWidth(), this->surface.getHeight());
}

void ImageButton::draw(Graphics::Surface*target)
{
	surface.blit(target, frame.x, frame.y);
}
