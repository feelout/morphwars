#ifndef DRAWER_H
#define DRAWER_H

#include "Surface.h"
#include "assist.h"

namespace Graphics
{
	class Drawer
	{
		private:
			Surface *target;
		public:
			Drawer(Surface *target);

			void putPixel(int x, int y, const RGBColor& color);
			RGBColor getPixel(int x, int y) const;

			void drawLine(int x1, int y1, int x2, int y2, const RGBColor& color);
			void drawRect(Rect rect, const RGBColor& color);
			void fillRect(Rect rect, const RGBColor& color);

			Surface *getTarget() const;
	};
}

#endif //DRAWER_H
