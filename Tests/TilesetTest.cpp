#include "../Renderer.h"
#include "../Timer.h"
#include "../Surface.h"
#include "../Drawer.h"
#include "../Logger.h"
#include "../TileSet.h"

using namespace Graphics;
using namespace Utility;
using namespace Core;

int main()
{
	if(!Renderer::getInstance()->init(640, 480, false))
	{
		Logger::getInstance()->log("Failed to set up video mode\n");
		return -1;
	}

	Drawer *target = new Drawer(Renderer::getInstance()->getBuffer());
	TileSet *tileset = new TileSet("Default");

	Surface *first = tileset->getType(0)->getTileImage(CENTER);
	Surface *second = tileset->getType(1)->getTileImage(SOUTHWEST);

	unsigned int startTime = Timer::currentTicks();

	while(Timer::currentTicks() < (startTime + 2000))
	{
		first->blit(target->getTarget(), 10, 10);
		second->blit(target->getTarget(), 10, 60);
		Renderer::getInstance()->flipBuffers();
		Timer::delay(10);
	}

	delete target;

	Renderer::getInstance()->shutdown();
}
