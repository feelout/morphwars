#include "Engine.h"
#include "Logger.h"
#include "Renderer.h"

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
	: quit(false)
{
	Utility::Logger::getInstance()->log("Engine created: %ix%i\n");

	if(Renderer::getInstance()->init(w,h,fs))
	{
		Utility::Logger::getInstance()->log("Renderer init succeeded\n");
	}
	else
	{
		Utility::Logger::getInstance()->log("Rendere init failed\n");
	}

	dispatcher = new EventDispatcher();
	equalizer = new FPSEqualizer();
}

void Engine::runGameCycle()
{
	SDL_Event event;

	while(!quit)
	{
		equalizer->frameStarted();
		// Event processing
		while(SDL_PollEvent(&event))
		{
			dispatcher->passEvent(event);
		}
		// Do all game logic and drawing here

		Renderer::getInstance()->flipBuffers();
		equalizer->frameEnded();
	}

	delete instance; //mb will throw segfaults, we`ll see
}

void Engine::stop()
{
	Utility::Logger::getInstance()->log("Stopping engine\n");
	quit = true;
}

Engine::~Engine()
{
	delete dispatcher;
	Logger::getInstance()->close();
}


