#ifndef SURFACE_H
#define SURFACE_H

#include <SDL/SDL.h>
#include <string>

namespace Graphics
{
	class Surface
	{
		private:
			SDL_Surface *surface;
		public:
			Surface(SDL_Surface *surface);
			Surface(int width, int hieght);
			Surface(std::string filename);

			SDL_Surface *getSurface();
	};
}

#endif
