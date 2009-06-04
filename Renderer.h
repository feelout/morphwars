#ifndef RENDERER_H
#define RENDERER_H

#include <SDL/SDL.h>

class Renderer
{
	private:
		SDL_Surface *frameBuffer;
		SDL_Surface *backBuffer;
		
		static Renderer *instance;
	public:
		static Renderer* getInstance();

		bool init(int width, int height, bool fullscreen);
		SDL_Surface *getBuffer();
		void shutdown();
}

#endif //RENDERER_H
