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

Surface::~Surface()
{
    SDL_FreeSurface(surface);
}

SDL_Surface* Surface::getSurface()
{
	return surface;
}

void Surface::blit(Surface *target, int x, int y)
{
    Rect clip = {0, 0, surface->w, surface->h};
    blit(target, clip, x, y);
}

void Surface::blit(Surface *target, Rect clip, int x, int y)
{
    Rect dstrect = {x, y, target->w, target->h);
    SDL_BlitSurface(surface->getSurface(), &clip, target->getSurface(), &dstrect);
}

std::vector<Surface*> Surface::splitSpriteStrip(Surface *strip, int frameWidth, int frameHeight)
{
    std::vector<Surface*> result;

    int frameCount = strip->w / frameWidth;
    Rect frameRect;
    SDL_Surface *frame;

    for(int i=0; i < frameCount; ++i)
    {
        frame = new Surface(frameWidth, frameHeight);
        frameRect.x = i*frameWidth;
        frameRect.y = 0;
        frameRect.w = frameWidth;
        frameRect.h = frameHeight;

        strip->blit(frame, frameRect, 0, 0);

        result.push_back(frame);
    }

    return result;
}
