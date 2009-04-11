#ifndef ENGINE_H
#define ENGINE_H

#include <SDL/SDL.h>
#include "EventDispatcher.h"

/**
 * Engine coordinates all subsystems and game cycle
 */
class Engine
{
	private:
		/**
		 * The only singleton instance
		 */
		static Engine *instance;
		//static int *instance;

		// TODO: Extract w,h,fs and screen to Renderer class
		/**
		 * Width of window
		 */
		int width;
		/**
		 * Height of window
		 */
		int height;
		/**
		 * Fullscreen mode
		 */
		bool fullscreen;
		/**
		 * Videobuffer surface
		 */
		SDL_Surface *screen;

		/**
		 * Stops game cycle after iteration
		 */
		bool quit;

		/**
		 * Dispatches events to all present subsytems
		 */
		EventDispatcher *dispatcher;

		/**
		 * Construct window and initializes subsystems
		 * @param w Width of window
		 * @param h Height of window
		 * @param fs Fullscreen mode
		 */
		Engine(int w, int h, bool fs);
		/**
		 * Destructor - performs cleanup
		 */
		~Engine(); //cleanup

		/**
		 * Initializes SDL and sets video mode
		 */
		bool setupSDL();
	public:
		/**
		 * Initialize Engine with given parametres
		 * @param width Width of window
		 * @param height Height of window
		 * @param fullscreen Fullscreen mode
		 */
		static void init(int width, int height, bool fullscreen);
		/**
		 * Returns singleton instance
		 */
		static Engine* getInstance();

		/**
		 * Starts game cycle
		 */
		void runGameCycle();
		/**
		 * Stops engine
		 */
		void stop();
};

#endif // ENGINE_H
