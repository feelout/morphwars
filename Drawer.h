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

			void putPixel(int x, int y, RGBColor color);
			RGBColor getPixel(int x, int y);

			void drawLine(int x1, int y1, int x2, int y2, RGBColor color);
			void drawRect(Rect rect, RGBColor color);
			void fillRect(Rect rect, RGBColor color);

			Surface *getTarget() const;
	};
}

#endif //DRAWER_H
