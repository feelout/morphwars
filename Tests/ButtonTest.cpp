#include "../Renderer.h"
#include "../Timer.h"
#include "../Surface.h"
#include "../Drawer.h"
#include "../Logger.h"
#include "../Button.h"
#include <SDL/SDL_image.h>

using namespace Graphics;
using namespace gui;
using namespace Utility;

int main()
{
	if(!Renderer::getInstance()->init(640, 480, false))
	{
		Logger::getInstance()->log("Failed to set up video mode\n");
		return -1;
	}

	Drawer drawer(Renderer::getInstance()->getBuffer());
	ImageButton button(Rect(10, 10, 100, 50), Surface("Button.png"));

	unsigned int startTime = Timer::currentTicks();

	while(Timer::currentTicks() < (startTime + 2000))
	{
		button.draw(&drawer);
		Renderer::getInstance()->flipBuffers();
		Timer::delay(10);
	}

	Renderer::getInstance()->shutdown();
}
