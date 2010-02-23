#include "FontSystem.h"
#include "Logger.h"

using namespace Graphics;

FontSystem* FontSystem::instance = NULL;

FontSystem::FontSystem()
{
}

FontSystem::FontSystem(const FontSystem& other)
{
}

bool FontSystem::loadFont(FontInfo info)
{
	if(!TTF_WasInit() && TTF_Init() ==  -1)
	{
		Utility::Logger::getInstance()->log("Failed to load SDL_ttf\n");
		return false;
	}

	TTF_Font *result = TTF_OpenFont(("Fonts/"+info.first+".ttf").c_str(), info.second);

	if(result == NULL)
	{
		Utility::Logger::getInstance()->log("Failed to open font %s %i\n", info.first.c_str(), info.second);
		return false;
	}

	Utility::Logger::getInstance()->log("Font %s %i loaded\n", info.first.c_str(), info.second);
	fonts[info] = result;

	return true;
}

FontSystem* FontSystem::getInstance()
{
	if(!instance)
		instance = new FontSystem();

	return instance;
}

void FontSystem::print(Surface *target, std::string text, int x, int y, RGBColor color, 
		FontInfo info, RenderQuality quality)
{
	std::map<FontInfo, TTF_Font*>::iterator i;

	i = fonts.find(info);

	if(i == fonts.end())
	{
		if(!loadFont(info))
			return;
	}

	SDL_Surface *rendered = NULL;
	if(quality == BLENDED)
	{
		rendered = TTF_RenderText_Blended(fonts[info], text.c_str(), color.toSDLColor());
	}
	else if(quality ==  SOLID)
	{
		rendered = TTF_RenderText_Solid(fonts[info], text.c_str(), color.toSDLColor());
	}

	SDL_Rect targetRect = {x, y, rendered->w, rendered->h};
	SDL_BlitSurface(rendered, NULL, target->getSurface(), &targetRect);

	SDL_FreeSurface(rendered);
}
