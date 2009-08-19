#include "Drawer.h" 
#include <SDL/SDL_gfxPrimitives.h>

using namespace Graphics;

Drawer::Drawer(Surface *target)
	: target(target)
{
}

void Drawer::putPixel(int x, int y, RGBColor color)
{
	pixelRGBA(target->getSurface(), x, y, color.r, color.g, color.b, 255);
}

RGBColor Drawer::getPixel(int x, int y)
{
	Uint8 *p = (Uint8 *)target->getSurface()->pixels + y * target->getSurface()->pitch + x * 4; // 4 = Bpp
	Uint32 pixel = *(Uint32 *)p;
	RGBColor result;
	SDL_GetRGB(pixel, target->getSurface()->format, &result.r, &result.g, &result.b);

	return result;
}

void Drawer::drawLine(int x1, int y1, int x2, int y2, RGBColor color)
{
	lineRGBA(target->getSurface(), x1, y1, x2, y2, color.r , color.g, color.b, 255);
}

void Drawer::drawRect(Rect rect, RGBColor color)
{
	rectangleRGBA(target->getSurface(), rect.x, rect.y, rect.x+rect.w, rect.y+rect.h, color.r, color.g, color.b, 255);
}

void Drawer::fillRect(Rect rect, RGBColor color)
{
	//boxRGBA(target->getSurface(), rect.x, rect.y, rect.x+rect.w, rect.y+rect.h, color.r, color.g, color.b, color.a);
	SDL_Rect sdlrect = {rect.x, rect.y, rect.w, rect.h};
	SDL_FillRect(target->getSurface(), &sdlrect, SDL_MapRGB(target->getSurface()->format, color.r, color.b, color.b));
}

Surface* Drawer::getTarget() const
{
    return target;
}
