#ifndef SIDEBAR_H
#define SIDEBAR_H

namespace Gui
{
	class Sidebar
	{
		private:
			Core::Scenario *owningScenario;
			Container *mainContainer;
			//Minimap *minimap;
			//buttons go here
		public:
			Sidebar(Rect frame, Core::Scenario *scenario);
};

#endif //SIDEBAR_H
