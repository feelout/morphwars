#include "Surface.h"
#include <SDL/SDL_image.h>

using namespace Graphics;

Surface::Surface(SDL_Surface *surface)
	: surface(surface)
{
}

Surface::Surface(int width, int height)
{

	Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif
	surface = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCCOLORKEY, width , height, rmask, 32, gmask, bmask, amask);
}

Surface::Surface(std::string filename)
{
	surface = IMG_Load(filename.c_str());
}

SDL_Surface* Surface::getSurface()
{
	return surface;
}
