#ifndef FONTSYSTEM_H
#define FONTSYSTEM_H

#include <SDL/SDL_ttf.h>
#include <string>
#include <map>
#include <utility>
#include "assist.h"
#include "Drawer.h"

namespace Graphics
{
	// Family, size of the font
	typedef std::pair<std::string, int> FontInfo;
	// Font render quality
	enum RenderQuality
	{
		SOLID,
		BLENDED
	};

	class FontSystem
	{
		private:
			static FontSystem *instance;

			std::map<FontInfo, TTF_Font*> fonts;
			FontSystem();
			FontSystem(const FontSystem& other);
			~FontSystem();

			bool loadFont(FontInfo info);
		public:
			static FontSystem* getInstance();

			void print(Drawer *target, std::string text, int x, int y, RGBColor color, 
					FontInfo info = std::make_pair("Vera", 12), RenderQuality quality = SOLID);
	};
}

#endif //FONTSYSTEM_H
