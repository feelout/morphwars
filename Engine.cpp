#include "Engine.h"
#include "Logger.h"
#include "OrderManager.h"
#include "AnimationManager.h"

Engine *Engine::instance = NULL;

const int DEFAULT_FPS = 60;

using namespace Graphics;

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
	: quit(false), currentScenario(NULL), renderer(NULL)
{
	Utility::Logger::getInstance()->log("Engine created: %ix%i\n", w, h);
	
	renderer = new Renderer();

	if(renderer->init(w,h,fs))
	{
		Utility::Logger::getInstance()->log("Renderer init succeeded\n");
	}
	else
	{
		Utility::Logger::getInstance()->log("Renderer init failed\n");
	}

	dispatcher = new EventDispatcher();
	equalizer = new Utility::FPSEqualizer(DEFAULT_FPS);
	stateStack = new Core::StateStack();
	menu = new Core::MenuState(w,h);
	//stateStack->push(menu);
	//dispatcher->attachListener(menu);
	changeState(menu);
}

void Engine::runGameCycle()
{
	SDL_Event event;
	Drawer drawer(renderer->getBuffer());

	while(!quit)
	{
		renderer->clear();
		equalizer->frameStarted();
		// Event processing
		while(SDL_PollEvent(&event))
		{
			dispatcher->passEvent(event);
		}
		// Do all game logic and drawing here

		/*if(currentScenario)
		{
			Core::OrderManager::getInstance()->processOrders();
			currentScenario->draw(&drawer, 0, 0);
		}*/
		if(stateStack->top())
		{
			stateStack->top()->process();
			stateStack->top()->draw(&drawer);
		}
		AnimationManager::getInstance()->updateAnimations();
		renderer->flipBuffers();
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
	Utility::Logger::getInstance()->log("Deleting engine instance\n");
	if(currentScenario)
	{
		delete currentScenario;
	}
	delete dispatcher;
	delete equalizer;
	delete stateStack; //TODO: call state destructors in ~StateStack()
	delete menu;
	delete renderer;

	Utility::Logger::getInstance()->close();
}

bool Engine::loadScenario(std::string path)
{
	// FIXME: shutdown current scenario
	if(currentScenario)
	{
		Utility::Logger::getInstance()->log("Already playing scenario.");
		return false;
	}

	currentScenario = new Core::Scenario(path);
	//dispatcher->attachListener(currentScenario);
	changeState(currentScenario);
	return true;
}

//TEMPORATY - May include duplicating states
void Engine::changeState(Core::EngineState *newState)
{
	if(stateStack->top())
	{
		dispatcher->detachListener(stateStack->top());
	}
	dispatcher->attachListener(newState);
	stateStack->push(newState);
}

Graphics::Renderer* Engine::getRenderer() const
{
	return renderer;
}
