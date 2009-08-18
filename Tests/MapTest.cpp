#include "../Renderer.h"
#include "../Animation.h"
#include "../Timer.h"
#include "../Surface.h"
#include "../Drawer.h"
#include "../Logger.h"
#include "../Scenario.h"

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
	Scenario *scenario = new Scenario("../Maps/Test/Test.xml");
	//Scenario *scenario = new Scenario("../Maps/Test/Test.xml");
	
	unsigned int startTime = Timer::currentTicks();

	while(Timer::currentTicks() < (startTime + 3000))
	{
		scenario->draw(target, 0, 0);
		Renderer::getInstance()->flipBuffers();
		Timer::delay(10);
	}

	delete scenario;
	delete target;

	Renderer::getInstance()->shutdown();
}
