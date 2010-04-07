#include "../Renderer.h"
#include "../Animation.h"
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
	Animation *anim = new Animation(animSurf, 40, 40, 500);
	anim->setLooped(true);
	anim->start();

	Drawer *target = new Drawer(Renderer::getInstance()->getBuffer());

	unsigned int startTime = Timer::currentTicks();

	while(Timer::currentTicks() < (startTime + 5000))
	{
		anim->update();
		anim->draw(target, 50, 50);
		Renderer::getInstance()->flipBuffers();
		Timer::delay(10);
	}

	delete anim;
	delete animSurf;
	delete target;

	Renderer::getInstance()->shutdown();
}
