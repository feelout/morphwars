#ifndef ENGINE_H
#define ENGINE_H

#include <SDL/SDL.h>

class Engine
{
	private:
		static Engine *instance;
		//static int *instance;

		// TODO: Extract w,h,fs and screen to Renderer class
		int width, height;
		bool fullscreen;
		SDL_Surface *screen;

		bool quit;

		Engine(int w, int h, bool fs);
		~Engine(); //cleanup

		bool setupSDL();
	public:
		static void init(int width, int height, bool fullscreen);
		static Engine* getInstance();

		void runGameCycle();
		void stop();
};

#endif // ENGINE_H
