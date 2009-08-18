#include <vector>
#include "../Renderer.h"
#include "../Timer.h"
#include "../Surface.h"
#include "../Drawer.h"
#include "../Logger.h"

using namespace Graphics;
using namespace Utility;

int main()
{
	if(!Renderer::getInstance()->init(640, 480, false))
	{
		Logger::getInstance()->log("Failed to set up video mode\n");
		return -1;
	}

	Surface *animSurf = new Surface("Animation.png");
	Surface *darkened = Surface::createShadowedSurface(animSurf, 0.5);

	Drawer *target = new Drawer(Renderer::getInstance()->getBuffer());

	unsigned int startTime = Timer::currentTicks();

	while(Timer::currentTicks() < (startTime + 5000))
	{
		animSurf->blit(Renderer::getInstance()->getBuffer(), 10, 10);
		darkened->blit(Renderer::getInstance()->getBuffer(), 10, 100);
		Renderer::getInstance()->flipBuffers();
		Timer::delay(10);
	}

	delete animSurf;
	delete target;

	Renderer::getInstance()->shutdown();
}
