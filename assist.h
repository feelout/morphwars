#ifndef ASSIST_H
#define ASSIST_H

/** Assist.h provides assisting function for using across classes
 */

#include <SDL/SDL.h>

/**
 * Creates SDL_Surface
 * @param w width of the surface
 * @param h height of the surface
 *
 * @deprecated
 */
SDL_Surface* createSurface(int w, int h);

/**
 * Represents rectangular area
 */
struct Rect
{
	int x,y;
	int w,h;
	
	Rect(int x,int y, int w, int h)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}

	Rect()
	{
		Rect(0,0,0,0);
	}
	
	bool isPointInRect(int px, int py) const
	{
		return ( (px <= x+w) && (px >= x) && (py <= y+h) && (py >= y) );
	}

	SDL_Rect toSDLRect() const
	{
		SDL_Rect rect;
		rect.x = x;
		rect.y = y;
		rect.w = w;
		rect.h = h;
		return rect;
	}
};

/**
 * Represents RGB model color
 */
struct RGBColor
{
	unsigned char r,g,b;

	RGBColor(int r, int g, int b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}
};
#endif //ASSIST_H
