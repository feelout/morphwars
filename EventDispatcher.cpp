#include "EventDispatcher.h"
#include "Engine.h"
#include "Logger.h"

EventDispatcher::EventDispatcher()
{
}

void EventDispatcher::attachListener(IEventListener* listener)
{
	listeners.push_back(listener);
}

void EventDispatcher::detachListener(IEventListener* listener)
{
	std::vector<IEventListener*>::iterator i;

	for(i = listeners.begin(); i != listeners.end(); i++)
	{
		if(*i == listener)
		{
			listeners.erase(i);
			return;
		}
	}
}

void EventDispatcher::passEvent(SDL_Event event)
{
	switch(event.type)
	{
		case SDL_QUIT:
			Engine::getInstance()->stop();
			break;
		default:
			Logger::getInstance()->log("Unhandled SDL_Event\n");
			break;
	}
}
