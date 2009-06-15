#include "../Renderer.h"
#include "../Animation.h"
#include "../Timer.h"
#include "../Surface.h"

using namespace Graphics;
using namespace Utility;

int main()
{
	if(!Renderer::init(640, 480, false))
	{
		printf("Failed to set up video mode\n");
		return -1;
	}

	Surface *animSurf = new Surface("animation.png");
	Animation *anim = new Animation(animSurf, 40, 40, 500);
	anim->setLooped(true);
	anim->start();

	Drawer *target = new Drawer(Renderer::getInstance()->getBuffer());

	unsigned int startTime = Timer::currentTicks();

	while(Timer::currentTicks() < (startTime + 5000))
	{
		anim->update();
		anim->draw(target);
		Renderer::getInstance()->flipBuffers();
		Timer::delay(100);
	}

	delete animSurf;
	delete anim;
	delete target;

	Renderer::shutdown();
}
