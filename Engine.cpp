#include "Engine.h"
#include "Logger.h"

Engine *Engine::instance = NULL;

// Static methods
void Engine::init(int width, int height, bool fullscreen)
{
	/*
	   Engine is created only once. Maybe will need change
	   in future - for example, to recreate everything after
	   changing resolution
	*/
	if(!instance)
	{
		instance = new Engine(width, height, fullscreen);
	}
}

Engine* Engine::getInstance()
{
	return instance;
}

// Instance methods
Engine::Engine(int w, int h, bool fs)
	: width(w), height(h), fullscreen(fs), quit(false)
{
	Logger::getInstance()->log("Engine created: %ix%i\n");

	if(setupSDL())
	{
		Logger::getInstance()->log("SDL init succeeded\n");
	}
	else
	{
		Logger::getInstance()->log("SDL init failed\n");
	}
}

bool Engine::setupSDL()
{
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		Logger::getInstance()->log("SDL subsystems init failed\n");
		return false;
	}

	int flags = SDL_SWSURFACE;

	if(fullscreen)
	{
		flags = flags | SDL_FULLSCREEN;
	}

	screen = SDL_SetVideoMode(width, height, 32, flags);

	if(!screen)
	{
		Logger::getInstance()->log("Setting video mode failed\n");
		return false;
	}

	SDL_WM_SetCaption("Unknown", NULL);

	return true;
}

void Engine::runGameCycle()
{
	SDL_Event event;

	while(!quit)
	{
		// Event processing
		while(SDL_PollEvent(&event))
		{
			/*
			   Mb process to some EventDispatcher, which sends signals
			   to all connected listeners (IListener). Probably better
			   that putting too much responsibility on one class
			*/
			if(event.type == SDL_QUIT)
			{
				quit = true;
			}
		}
		// Do all game logic and drawing here
		
		SDL_Delay(10); // Not to eat all CPU power
	}

	delete instance; //mb will throw segfaults, we`ll see
}

Engine::~Engine()
{
	Logger::getInstance()->close();
}


