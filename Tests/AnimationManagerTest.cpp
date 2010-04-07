#include "../AnimationManager.h"
#include "../Animation.h"
#include "../Drawer.h"
#include "../Timer.h"
#include "../Renderer.h"
#include "../Logger.h"

using namespace Graphics;

int main()
{
	if(!Renderer::getInstance()->init(640, 480, false))
	{
		Utility::Logger::getInstance()->log("Failed to set up video mode\n");
		return -1;
	}

	Animation *anim = AnimationManager::getInstance()->getAnimation("TestBall");
	anim->start();

	Drawer *target = new Drawer(Renderer::getInstance()->getBuffer());

	unsigned int startTime = Utility::Timer::currentTicks();

	while(Utility::Timer::currentTicks() < (startTime + 2000))
	{
		anim->update();
		anim->draw(target, 10, 10);
		Renderer::getInstance()->flipBuffers();
		Utility::Timer::delay(10);
	}

	delete anim;
	delete target;

	Renderer::getInstance()->shutdown();
};
