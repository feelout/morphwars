#include "../Renderer.h"
#include "../Timer.h"
#include "../Surface.h"
#include "../Drawer.h"
#include "../Logger.h"
#include <SDL/SDL_image.h>

using namespace Graphics;
using namespace Utility;


SDL_Surface *getImage(const char *filename)
{
	SDL_Surface *loadedImage=NULL;
	SDL_Surface *optimizedImage=NULL;

	loadedImage = IMG_Load(filename);
	optimizedImage = SDL_DisplayFormat(loadedImage); //mp Alpha?
	SDL_FreeSurface(loadedImage);

	Uint32 colorkey = SDL_MapRGB(optimizedImage->format, 0xFF, 0, 0xFF);
	SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, colorkey);

	return optimizedImage;
}

int main()
{
	if(!Renderer::getInstance()->init(640, 480, false))
	{
		Logger::getInstance()->log("Failed to set up video mode\n");
		return -1;
	}

	Surface *animSurf = new Surface("Animation.png");
	//Surface *transSurf = new Surface("Transparent.png");
	Surface *colorkey = new Surface("ColorKey.png");

	Drawer *target = new Drawer(Renderer::getInstance()->getBuffer());
	
	Rect blitRect;
	blitRect.x = 0;
	blitRect.y = 0;
	blitRect.w = 40;
	blitRect.h = 40;

	SDL_Surface *ck_surf = colorkey->getSurface();
	//SDL_Surface *ck_surf = getImage("ColorKey.png");
	SDL_Rect srcRect, dstRect;
	srcRect.w = ck_surf->w;
	srcRect.h = ck_surf->h;
	srcRect.x = 0;
	srcRect.y = 0;
	dstRect.w = ck_surf->w;
	dstRect.h = ck_surf->h;
	dstRect.x = 100;
	dstRect.y = 400;

	Surface *second = new Surface(40, 40);
	
	animSurf->blit(second, &blitRect, 0, 0);

	unsigned int startTime = Timer::currentTicks();

	while(Timer::currentTicks() < (startTime + 2000))
	{
		/* Direct blit of loaded surface to buffer */
		animSurf->blit(Renderer::getInstance()->getBuffer(), 100, 100);
		/* Direct blit of part of loaded surface to buffer */
		animSurf->blit(Renderer::getInstance()->getBuffer(), &blitRect, 100, 150);
		/* Indirect blit to buffer */
		second->blit(Renderer::getInstance()->getBuffer(), 100, 200);
		/* Blit of transparent surface */
		//transSurf->blit(Renderer::getInstance()->getBuffer(), 200, 200);
		/* Blit of surface with a colorkey */
		//colorkey->blit(Renderer::getInstance()->getBuffer(), 100, 400);
		SDL_BlitSurface(ck_surf, &srcRect, Renderer::getInstance()->getBuffer()->getSurface(), &dstRect);
		/* Flipping render buffers */
		Renderer::getInstance()->flipBuffers();
		Timer::delay(10);
	}

	delete animSurf;
	delete target;
	delete second;

	Renderer::getInstance()->shutdown();
}
