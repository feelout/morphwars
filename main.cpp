#include "Engine.h"
#include "Map.h"
#include "Logger.h"
#include "TileSet.h"

int main(int argc, char **argv)
{
	Engine::init(800, 640, false);
	Engine::getInstance()->runGameCycle();
}
