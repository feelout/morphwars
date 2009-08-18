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
		case SDL_KEYDOWN:
			{
				std::vector<IEventListener*>::iterator i;

				for(i = listeners.begin(); i != listeners.end(); i++)
				{
					(*i)->keyPressed(event.key.keysym.sym);
				}
			}
			break;
		case SDL_MOUSEMOTION:
			{
				std::vector<IEventListener*>::iterator i;

				for(i = listeners.begin(); i != listeners.end(); i++)
				{
					(*i)->mouseMoved(event.motion.x, event.motion.y);
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			{
				std::vector<IEventListener*>::iterator i;

				for(i = listeners.begin(); i != listeners.end(); i++)
				{
					if(event.button.button == SDL_BUTTON_LEFT)
					{
						(*i)->mouseLMBClicked(event.button.x, event.button.y);
					}
					else if(event.button.button == SDL_BUTTON_RIGHT)
					{
						(*i)->mouseRMBClicked(event.button.x, event.button.y);
					}
				}
			}
			break;
		default:
			//Utility::Logger::getInstance()->log("Unhandled SDL_Event\n");
			break;
	}
}
