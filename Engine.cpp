#include "Engine.h"
#include "Logger.h"
#include "OrderManager.h"
#include "AnimationManager.h"
#include "MouseSelector.h"
#include "Drawer.h"
#include "PythonGlue.h"

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
	Core::MouseState::getInstance()->setActionType(Core::MouseState::SELECT);

	Utility::InitPythonGlueCode();
}

void Engine::runGameCycle()
{
	SDL_Event event;
	//Drawer drawer(renderer->getBuffer());
	Surface *buffer = renderer->getBuffer();

	while(!quit)
	{
		renderer->clear();
		equalizer->frameStarted();

		while(SDL_PollEvent(&event))
		{
			dispatcher->passEvent(event);
		}

		if(stateStack->top())
		{
			stateStack->top()->process();
			stateStack->top()->draw(buffer);
		}

		AnimationManager::getInstance()->updateAnimations();
		Core::MouseState::getInstance()->draw(buffer);

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
	Utility::Logger::getInstance()->log("Engine::loadScenario(%s)\n", path.c_str());
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
	Utility::Logger::getInstance()->log("State changed to %s\n", newState->getName().c_str());
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

EventDispatcher* Engine::getEventDispatcher() const
{
	return dispatcher;
}
