#include "Renderer.h"

Renderer* Renderer::instance = NULL;

Renderer::Renderer()
{
}

Renderer* Renderer::getInstance()
{
	if(!instance)
		instance = new Renderer();
	return instance;
}

bool Renderer::init(int width, int height, bool fullscreen)
{
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		return false;
	}

	int flags = SDL_SWSURFACE;

	if(fullscreen)
	{
		flags = flags | SDL_FULLSCREEN;
	}

	frameBuffer = SDL_SetVideoMode(width, height, 32, flags);

	if(!frameBuffer)
		return false;
}

SDL_Surface *Renderer::getBuffer()
{
	return frameBuffer;
}

void Renderer::flipBuffers()
{
	SDL_Flip(frameBuffer);
}
