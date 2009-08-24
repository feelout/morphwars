#include "Surface.h"
#include "Drawer.h"
#include "Logger.h"
#include <SDL/SDL_image.h>
#include <SDL_rotozoom.h>

using namespace Graphics;

SDL_Surface* Surface::createEmptySurface(int width, int height)
{
	//Utility::Logger::getInstance()->log("Creating empty surface: %ix%i\n", width, height);
	SDL_Surface *result = NULL;
	result = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCCOLORKEY, width , height, 32,
			0, 0, 0, 0);
	if(!result)
	{
		Utility::Logger::getInstance()->log("Failed to create Surface %ix%i\n", width, height);
		return NULL;
	}

	Uint32 colorkey = SDL_MapRGB(result->format, 0, 0, 0); //what a hack...
	SDL_SetColorKey(result, SDL_SRCCOLORKEY, colorkey);

	return result;
}
Surface::Surface(SDL_Surface *surface)
	: surface(surface)
{
	//Utility::Logger::getInstance()->log("Creating surface from SDL_Surface\n");
}

Surface::Surface(int width, int height)
{
	//Utility::Logger::getInstance()->log("Creating surface with dimenstions\n");
	surface = createEmptySurface(width, height);

	Drawer filler(this);
	Rect fillingRect(0,0,width,height);
	RGBColor fillingColor(0, 0, 0);
	filler.fillRect(fillingRect, fillingColor);

}

Surface::Surface(std::string filename)
{
	//Utility::Logger::getInstance()->log("Surface::Surface(%s)\n", filename.c_str());
	SDL_Surface *loaded = IMG_Load(filename.c_str());

	if(!loaded)
	{
		Utility::Logger::getInstance()->log("Couldn`t load image file %s\n", filename.c_str());
		return;
	}

	// Optimize loaded surface
	surface = SDL_DisplayFormat(loaded);
	SDL_FreeSurface(loaded);

	Uint32 colorkey = SDL_MapRGB(surface->format, 0xFF, 0, 0xFF);
	SDL_SetColorKey(surface, SDL_SRCCOLORKEY, colorkey); 
}

Surface::Surface(const Surface& other)
{
	//Utility::Logger::getInstance()->log("Surface copy constructor called\n");
	this->surface = createEmptySurface(other.getWidth(), other.getHeight());

	//FIXME: Fill with alpha, check if scenario works
	Drawer filler(this);
	Rect fillingRect(0,0,other.getWidth(),other.getHeight());
	RGBColor fillingColor(0, 0, 0);
	filler.fillRect(fillingRect, fillingColor);

	other.blit(this, 0, 0);
}

Surface::~Surface()
{
	SDL_FreeSurface(surface); }

int Surface::getWidth() const
{
	return surface->w;
}

int Surface::getHeight() const
{
	return surface->h;
}



SDL_Surface* Surface::getSurface() const
{
	return surface;
}

void Surface::blit(Surface *target, int x, int y) const
{
	Rect *clip = new Rect(0, 0, surface->w, surface->h);
	blit(target, clip, x, y);
	delete clip;
}

void Surface::blit(Surface *target, Rect *clip, int x, int y) const
{
	Rect *dstrect = new Rect(x, y, clip->w, clip->h);
	SDL_Rect cl = clip->toSDLRect();
	SDL_Rect dst = dstrect->toSDLRect();
	SDL_BlitSurface(surface, &cl, target->getSurface(), &dst);
	delete dstrect;
}

Surface& Surface::operator=(const Surface& other)
{
	this->surface = createEmptySurface(other.getWidth(), other.getHeight());		
	other.blit(this, 0, 0);
	return *this;
}

Surface* Surface::zoom(double x_factor, double y_factor, bool antialias)
{
	Utility::Logger::getInstance()->log("Zooming surface: xf = %d, yf = %d\n", x_factor, y_factor);
	SDL_Surface *result = zoomSurface(surface, x_factor, y_factor, antialias ? 1 : 0);
	return new Surface(result);
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

Surface* Surface::createShadowedSurface(Surface *src, float factor)
{
	Surface *darkened = new Surface(src->getWidth(), src->getHeight());
	src->blit(darkened, 0, 0);

	Drawer drawer(darkened);
	RGBColor color;

	for(int y=0; y < darkened->getHeight(); ++y)
	{
		for(int x=0; x < darkened->getWidth(); ++x)
		{
			color = drawer.getPixel(x,y);
			//Check for color key
			if((color.r == 0xFF) && (color.g == 0) && (color.b == 0xFF)) continue;

			color.r *= factor;
			color.g *= factor;
			color.b *= factor;

			drawer.putPixel(x, y, color);
		}
	}
	
	return darkened;
}
