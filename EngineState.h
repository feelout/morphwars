#ifndef ENGINE_STATE_H
#define ENGINE_STATE_H

#include <string>
#include "EventDispatcher.h"
#include "Surface.h"

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

			//DEBUG
			std::string getName() const;
			//ENDDEBUG

			virtual void pause();
			virtual void resume();

			virtual void draw(Graphics::Surface *target)=0;
			virtual void process()=0;

			virtual bool mouseMoved(int x, int y);
			virtual bool mouseLMBClicked(int x, int y);
			virtual bool mouseRMBClicked(int x, int y);
			virtual bool keyPressed(int key);
	};
}

#endif //ENGINE_STATE_H
