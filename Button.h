#ifndef BUTTON_H
#define BUTTON_H

#include <list>
#include "Widget.h"
#include "Surface.h"

namespace Gui
{
	//typedef void (*ButtonClickCallback)();
	class Button;

	class IButtonListener
	{
		public:
			virtual bool buttonClicked(Button *sender)=0;
	};

	class Button : public Widget
	{
		protected:
			//ButtonClickCallback callback;
			std::list<IButtonListener *> listeners;
		public:
			Button(Rect frame);

			//void setOnClicked(ButtonClickCallback cb);
			void addListener(IButtonListener *listener);
			void removeListener(IButtonListener *listener);

			virtual bool mouseLMBClicked(int x, int y);
			virtual void draw(Graphics::Surface *target)=0;
	};

	class ImageButton : public Button
	{
		protected:
			Graphics::Surface surface;
		public:
			ImageButton(Rect frame, Graphics::Surface surface);

			virtual void draw(Graphics::Surface *target);
	};
}

#endif //BUTTON_H
