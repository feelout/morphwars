#include <cstdio>
#include "Engine.h"
#include "Surface.h"
#include "Renderer.h"
#include "Timer.h"
#include "TileSet.h"
#include "Logger.h"

using namespace Graphics;
using namespace Utility;
using namespace Core;

int main(int argc, char *argv[])
{
	Engine::init(800, 640, false);
	Engine::getInstance()->runGameCycle();
	
	/*Renderer renderer;
	renderer.init(800, 640, false);
	Drawer(renderer.getBuffer()).fillRect(Rect(0,0,800,640), RGBColor(0, 120, 50, 255));

	Surface loaded("Tests/Pepper.png");
	loaded.blit(renderer.getBuffer(), 0, 0);

	Surface created(100, 100);
	created.blit(renderer.getBuffer(), 100, 0);

	loaded.blit(&created, 0, 0);
	created.blit(renderer.getBuffer(), 200, 0); //should see pepper

	renderer.flipBuffers();
	Timer::delay(1000); */

	return 0;
}
