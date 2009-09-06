#ifndef MOUSE_SELECTOR_H
#define MOUSE_SELECTOR_H

#include "Drawer.h"
#include "Engine.h"

namespace Core
{
	class Scenario;

	class MouseState
	{
		public:
			enum ActionType {SELECT=0, MOVE, ATTACK, SKILL};
		private:
			static MouseState *instance;

			Graphics::Surface *cursor[4];
			ActionType currentType;

			int x,y;

			MouseState();
			MouseState(MouseState &other);
			~MouseState();
		public:
			static MouseState *getInstance();

			void draw(Graphics::Drawer *target);
			void setActionType(ActionType type);
			ActionType getActionType() const;

			bool mouseMoved(int x, int y);
	};
}

#endif //MOUSE_SELECTOR_H
