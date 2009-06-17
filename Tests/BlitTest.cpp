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
	Drawer *target = new Drawer(Renderer::getInstance()->getBuffer());
	
	Rect blitRect;
	blitRect.x = 0;
	blitRect.y = 0;
	blitRect.w = 40;
	blitRect.h = 40;

	Surface *second = new Surface(40, 40);
	
	animSurf->blit(second, &blitRect, 0, 0);

	unsigned int startTime = Timer::currentTicks();

	while(Timer::currentTicks() < (startTime + 2000))
	{
		/* Direct blit of loaded surface to buffer */
		animSurf->blit(Renderer::getInstance()->getBuffer(), 100, 100);
		/* Direct blit of part of loaded surface to buffer */
		animSurf->blit(Renderer::getInstance()->getBuffer(), &blitRect, 100, 150);
		/* Indirect blit to buffer */
		second->blit(Renderer::getInstance()->getBuffer(), 100, 200);
		/* Flipping render buffers */
		Renderer::getInstance()->flipBuffers();
		Timer::delay(10);
	}

	delete animSurf;
	delete target;
	delete second;

	Renderer::getInstance()->shutdown();
}
