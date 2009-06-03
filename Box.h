#ifndef BOX_H
#define BOX_H

#include "Container.h"

namespace Gui
{
	class Box : public Container
	{
		protected:
			virtual void organiseChildren()=0;
			virtual void calculateRequest()=0;
		public:
			Box(Rect frame, Widget *parent = NULL);

			void addForward(Widget *widget);
			void addBackward(Widget *widget);

			virtual void setFrame(Rect frame);
	};
}

#endif //BOX_H
