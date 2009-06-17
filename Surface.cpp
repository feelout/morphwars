#include "Surface.h"
#include "Logger.h"
#include <SDL/SDL_image.h>

using namespace Graphics;

Surface::Surface(SDL_Surface *surface)
	: surface(surface)
{
}

Surface::Surface(int width, int height)
{
	surface = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCCOLORKEY, width , height, 32, 0, 0, 0, 0);
	if(!surface)
	{
		Utility::Logger::getInstance()->log("Failed to create Surface %ix%i\n", width, height);
	}
}

Surface::Surface(std::string filename)
{
	surface = IMG_Load(filename.c_str());

	if(!surface)
	{
		Utility::Logger::getInstance()->log("Couldn`t load image file %s\n", filename.c_str());
	}
}

Surface::~Surface()
{
	SDL_FreeSurface(surface);
}

SDL_Surface* Surface::getSurface() const
{
	return surface;
}

void Surface::blit(Surface *target, int x, int y)
{
	Rect *clip = new Rect(0, 0, surface->w, surface->h);
	blit(target, clip, x, y);
	delete clip;
}

void Surface::blit(Surface *target, Rect *clip, int x, int y)
{
	//Rect *dstrect = new Rect(x, y, target->getSurface()->w, target->getSurface()->h);
	Rect *dstrect = new Rect(x, y, clip->w, clip->h);
	SDL_Rect cl = clip->toSDLRect();
	SDL_Rect dst = dstrect->toSDLRect();
	//SDL_BlitSurface(surface, &clip->toSDLRect(), target->getSurface(), &dstrect->toSDLRect());
	SDL_BlitSurface(surface, &cl, target->getSurface(), &dst);
	delete dstrect;
}

std::vector<Surface*> Surface::splitSpriteStrip(Surface *strip, int frameWidth, int frameHeight)
{
	std::vector<Surface*> result;

	int frameCount = strip->getSurface()->w / frameWidth;
	Rect frameRect;
	Surface *frame;

	for(int i=0; i < frameCount; ++i)
	{
		frame = new Surface(frameWidth, frameHeight);
		frameRect.x = i*frameWidth;
		frameRect.y = 0;
		frameRect.w = frameWidth;
		frameRect.h = frameHeight;

		strip->blit(frame, &frameRect, 0, 0);

		result.push_back(frame);
	}

	return result;
}
