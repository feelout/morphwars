#ifndef BUTTON_H
#define BUTTON_H

#include "Widget.h"
#include "Surface.h"

namespace Gui
{
	typedef void (*ButtonClickCallback)();

	class Button : public Widget
	{
		protected:
			ButtonClickCallback callback;
		public:
			Button(Rect frame);

			void setOnClicked(ButtonClickCallback cb);
			virtual void mouseLMBClicked(int x, int y);
			virtual void draw(Graphics::Drawer *target)=0;
	};

	class ImageButton : public Button
	{
		protected:
			Graphics::Surface surface;
		public:
			ImageButton(Rect frame, Graphics::Surface surface);

			virtual void draw(Graphics::Drawer *target);
	};
}

#endif //BUTTON_H