#include "Engine.h"

int main(int argc, char *argv[])
{
	Engine::init(800, 640, false);
	Engine::getInstance()->runGameCycle();

	return 0;
}
