#ifndef ENGINE_STATE_H
#define ENGINE_STATE_H

#include <string>
#include "EventDispatcher.h"
#include "Drawer.h"

//TODO: State management should be in form of stack
namespace Core
{
	class EngineState : public IEventListener
	{
		protected:
			/* Is state enabled currently (does check events) */
			bool enabled;
			/* Do state drawing? */
			bool drawing;
			/* State caption */
			std::string name;
		public:
			EngineState(std::string name);

			bool isEnabled() const;
			void setEnabled(bool enabled);

			bool doDrawing() const;
			void setDrawingState(bool drawing);

			virtual void pause();
			virtual void resume();

			virtual void draw(Graphics::Drawer *target)=0;
			virtual void process()=0;

			virtual void mouseMoved(int x, int y);
			virtual void mouseLMBClicked(int x, int y);
			virtual void mouseRMBClicked(int x, int y);
			virtual void keyPressed(int key);
	};
}

#endif //ENGINE_STATE_H
