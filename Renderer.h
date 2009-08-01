#ifndef RENDERER_H
#define RENDERER_H

#include <SDL/SDL.h>
#include "Surface.h"
#include "Drawer.h"

namespace Graphics
{
	class Renderer
	{
		private:
			SDL_Surface *frameBufferSDL;
			Surface *frameBuffer;

			static Renderer *instance;

			Renderer();
		public:
			static Renderer* getInstance();

			bool init(int width, int height, bool fullscreen);
			Surface *getBuffer();
			void flipBuffers();
			void shutdown();
	};
}

#endif //RENDERER_H
