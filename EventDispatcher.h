#ifndef EVENT_DISPATCHER_H
#define EVENT_DISPATCHER_H

#include <SDL/SDL.h>
#include <vector>

/**
 * Interface for all classes needed to receive messages
 */
class IEventListener
{
	public:
		/**
		 * Fired when mouse was moved
		 * @param x x coordinate of mouse
		 * @param y y coordinate of mouse
		 */
		virtual bool mouseMoved(int x,int y)=0;
		/**
		 * Fired when mouse left button was clicked
		 * @param x x coordinate of mouse
		 * @param y y coordinate of mouse
		 */
		virtual bool mouseLMBClicked(int x,int y)=0;
		/**
		 * Fired when mouse right button was clicked
		 * @param x x coordinate of mouse
		 * @param y y coordinate of mouse
		 */
		virtual bool mouseRMBClicked(int x,int y)=0;

		/**
		 * Fired when key was pressed
		 * @param key key code
		 */
		virtual bool keyPressed(int key)=0;
};

/**
 * Dispatches messages among listeners
 */
class EventDispatcher
{
	private:
		/**
		 * References to all listeners
		 */
		std::vector<IEventListener*> listeners;

	public:
		/**
		 * Dummy constructor
		 */
		EventDispatcher();

		/**
		 * Passes event to listeners
		 * @param event event itself
		 */
		void passEvent(SDL_Event event);
		/**
		 * Adds listener
		 * @param listener listener to add
		 */
		void attachListener(IEventListener* listener);
		/**
		 * Removes listener
		 * @param listener listener to remove
		 */
		void detachListener(IEventListener* listener);
};

#endif //EVENT_DISPATCHER_H
