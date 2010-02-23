#ifndef RENDERER_H
#define RENDERER_H

#include <SDL/SDL.h>
#include "Surface.h"

namespace Graphics
{
	class Renderer
	{
		private:
			SDL_Surface *frameBufferSDL;
			Surface *frameBuffer;
			int width, height;

		public:
			Renderer();
			~Renderer();
			bool init(int width, int height, bool fullscreen);
			Surface *getBuffer();
			void flipBuffers();
			void clear();

			int getWidth() const;
			int getHeight() const;
	};
}

#endif //RENDERER_H
