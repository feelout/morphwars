#ifndef PLACEHOLDER_H
#define PLACEHOLDER_H

#include "Widget.h"

namespace Gui
{
	class Placeholder : public Widget
	{
		protected:
			Widget *child;

			virtual void frameUpdated();
		public:
			Placeholder(Rect frame, Widget *parent=NULL);

			void setChild(Widget *child);
			
			virtual void draw(Graphics::Surface *target);
	};
}

#endif // PLACEHOLDER_H
