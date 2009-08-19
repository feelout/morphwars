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

		public:
			Renderer();
			~Renderer();
			bool init(int width, int height, bool fullscreen);
			Surface *getBuffer();
			void flipBuffers();
			void clear();
	};
}

#endif //RENDERER_H
