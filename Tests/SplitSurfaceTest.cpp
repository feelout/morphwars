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
	
	std::vector<Surface*> surfaces = Surface::splitSpriteStrip(animSurf, 40, 40);
	std::vector<Surface*>::iterator i;

	Drawer *target = new Drawer(Renderer::getInstance()->getBuffer());

	unsigned int startTime = Timer::currentTicks();

	int j;
	while(Timer::currentTicks() < (startTime + 5000))
	{
		j=0;
		for(i = surfaces.begin(); i != surfaces.end(); ++i, j++)
		{
			(*i)->blit(Renderer::getInstance()->getBuffer(), 50*j, 0);
		}
		
		Renderer::getInstance()->flipBuffers();
		Timer::delay(10);
	}

	delete animSurf;
	delete target;

	Renderer::getInstance()->shutdown();
}
