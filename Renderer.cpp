#include "Renderer.h"

using namespace Graphics;

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

void Renderer::shutdown()
{
	delete frameBuffer;
}