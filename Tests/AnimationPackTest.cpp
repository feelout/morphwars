#include "../Renderer.h"
#include "../Animation.h"
#include "../AnimationPack.h"
#include "../AnimationManager.h"
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

	Surface *boriskaSurf = new Surface("../Objects/Units/Boriska/Gfx.png");
	AnimationPack *boriska = new AnimationPack(boriskaSurf, "../Objects/Units/Boriska/Animations.xml");
	boriska->changeToAnimation("Boriska-SW");
	boriska->getCurrent()->start();

	Drawer *target = new Drawer(Renderer::getInstance()->getBuffer());

	unsigned int startTime = Timer::currentTicks();

	while(Timer::currentTicks() < (startTime + 5000))
	{
		boriska->getCurrent()->draw(target, 0, 0);
		AnimationManager::getInstance()->updateAnimations();
		Renderer::getInstance()->flipBuffers();
		Timer::delay(10);
	}

	delete boriskaSurf;
	delete boriska;
	delete target;

	Renderer::getInstance()->shutdown();
}
