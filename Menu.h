#ifndef MENU_H
#define MENU_H

#include "Button.h"
#include "EngineState.h"

namespace Core 
{
	/* Main menu. Serves for switching to other states */
	class MenuState : public EngineState
	{
		public:
			enum MenuButton
			{
				CAMPAIGN=0,
				SCENARIO,
				LOADGAME,
				OPTIONS,
				CREDITS,
				QUIT	
			};
		private:
			static const int BUTTONS_NUM = 6;

			Gui::Button* buttons[BUTTONS_NUM];
		public:
			MenuState(int screen_w, int screen_h);

			virtual void draw(Graphics::Surface *target);
			virtual void process();
			virtual bool mouseLMBClicked(int x, int y);
	};
}

#endif //MENU_H
