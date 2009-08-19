#include "Renderer.h"
#include <algorithm>

using namespace Graphics;

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
	delete frameBuffer;
	SDL_Quit();
}


bool Renderer::init(int width, int height, bool fullscreen)
{
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		return false;
	}

	int flags = SDL_SWSURFACE | SDL_SRCALPHA;

	if(fullscreen)
	{
		flags = flags | SDL_FULLSCREEN;
	}

	frameBufferSDL = SDL_SetVideoMode(width, height, 32, flags);

	if(!frameBufferSDL)
		return false;
		
	frameBuffer = new Surface(frameBufferSDL);
	
	return true;
}

Surface *Renderer::getBuffer()
{
	return frameBuffer;
}

void Renderer::flipBuffers()
{
	SDL_Flip(frameBufferSDL);
}

void Renderer::clear()
{
	SDL_FillRect(frameBufferSDL, NULL, 0);
}

