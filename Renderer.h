#ifndef RENDERER_H
#define RENDERER_H

#include <SDL/SDL.h>

class Renderer
{
	private:
		SDL_Surface *frameBuffer;
		
		static Renderer *instance;

		Renderer();
	public:
		static Renderer* getInstance();

		bool init(int width, int height, bool fullscreen);
		SDL_Surface *getBuffer();
		void flipBuffers();
		void shutdown();
};

#endif //RENDERER_H
