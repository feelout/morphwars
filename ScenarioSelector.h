#ifndef SCENARIOSELECTOR_H
#define SCENARIOSELECTOR_H

#include "List.h"
#include "EngineState.h"

namespace Core
{
	class ScenarioSelectingState;
};

namespace Gui
{
	class ScenarioSelector : public List
	{
		protected:
			virtual void itemSelected(int n);
			virtual void currentItemChanged(int n);

			Core::ScenarioSelectingState *state;
		public:
			ScenarioSelector(Rect frame, Core::ScenarioSelectingState *state);
	};
};

namespace Core
{
	//FIXME: add go back button
	class ScenarioSelectingState : public EngineState
	{
		private:
			Gui::ScenarioSelector *selector;
			Graphics::Surface *preview;
			std::string info;
		public:	
			ScenarioSelectingState(int screen_w, int screen_h);
			~ScenarioSelectingState();

			void setCurrentScenario(std::string scenario);

			virtual void process();
			virtual void draw(Graphics::Drawer *target);

			virtual bool mouseMoved(int x, int y);
			virtual bool mouseLMBClicked(int x, int y);
	};
};

#endif //SCENARIOSELECTOR_H
