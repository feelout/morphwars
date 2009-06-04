#ifndef ASSIST_H
#define ASSIST_H

/** Assist.h provides assisting function for using across classes
 */

#include <SDL/SDL.h>

/**
 * Creates SDL_Surface
 * @param w width of the surface
 * @param h height of the surface
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
		this.x = x;
		this.y = y;
		this.w = w;
		this.h = h;
	}
	
	bool isPointInRect(int px, int py)
	{
		return ( (px <= x+w) && (px >= x) && (py <= y+h) && (py >= y) );
	}
};

/**
 * Represents RGB model color
 */
struct RGBColor
{
	int r,g,b;
}
#endif //ASSIST_H
