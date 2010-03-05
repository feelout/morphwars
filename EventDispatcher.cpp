#include "EventDispatcher.h"
#include "Engine.h"
#include "Logger.h"
#include "MouseSelector.h"

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
					if((*i)->keyPressed(event.key.keysym.sym))
						return;
				}
			}
			break;
		case SDL_MOUSEMOTION:
			{
				Core::MouseState::getInstance()->mouseMoved(event.motion.x, event.motion.y);
				std::vector<IEventListener*>::iterator i;

				for(i = listeners.begin(); i != listeners.end(); i++)
				{
					if((*i)->mouseMoved(event.motion.x, event.motion.y))
						return;
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			{
				//Utility::Logger::getInstance()->log("Mouse button down\n");
				//Utility::Logger::getInstance()->log("Total listeners: %i\n", listeners.size());
				std::vector<IEventListener*>::iterator i;

				//DEBUG
				int k=0;
				//ENDDEBUG

				for(i = listeners.begin(); i != listeners.end(); i++)
				{
					//Utility::Logger::getInstance()->log("%i\n", k++);
					if(event.button.button == SDL_BUTTON_LEFT)
					{
						if((*i)->mouseLMBClicked(event.button.x, event.button.y))
							return;
					}
					else if(event.button.button == SDL_BUTTON_RIGHT)
					{
						if((*i)->mouseRMBClicked(event.button.x, event.button.y))
							return;
					}
				}
			}
			break;
		default:
			//Utility::Logger::getInstance()->log("Unhandled SDL_Event\n");
			break;
	}
}
