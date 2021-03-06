#ifndef ASSIST_H
#define ASSIST_H

/** Assist.h provides assisting function for using across classes
 */

#include <SDL/SDL.h>
#include <vector>
#include <string>

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

	bool isRectInRect(const Rect& other)
	{
		return ( (other.x >= x) && (other.x+other.w <= x+w) && (other.y >= y) && (other.y+other.h <= y+h) );
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
	static const RGBColor RED;
	static const RGBColor YELLOW;
	static const RGBColor BLUE;
	static const RGBColor GREEN;
	static const RGBColor WHITE;
	static const RGBColor BLACK;

	unsigned char r,g,b;

	RGBColor(int r, int g, int b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}

	RGBColor()
	{
		RGBColor(0, 0, 0);
	}
	
	SDL_Color toSDLColor()
	{
		SDL_Color result = {r, g, b, 0};
		return result;
	}

	bool operator==(const RGBColor& other) const
	{
		if(&other == this)
			return true;

		return ( (r == other.r) && (g == other.g) && (b == other.b) );
	}
};

std::vector<std::string> ListFilesInFolder(std::string dirname);
std::string br_itoa(int n);

#endif //ASSIST_H
