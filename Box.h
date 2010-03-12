#ifndef BOX_H
#define BOX_H

#include "Container.h"
#include <vector>

using std::vector;

namespace Gui
{
	class Box : public Container
	{
		private:
			struct BoxEntry
			{
				Widget 	*widget;
				bool	expandable;
				int	spacing;

				BoxEntry(Widget* w, bool exp, int sp)
					: widget(w), expandable(exp), spacing(sp) 
				{
				}
			};
		protected:
			int spacing;

			vector<BoxEntry*> entries;

		public:
			Box(Rect frame, int spacing, Widget *parent=NULL);
			virtual ~Box();

			virtual void addChild(Widget *child, bool expandable, int spacing);
			virtual void removeChild(Widget *child);

			virtual void recalculateChildrenFrames()=0;
	};

	class HBox : public Box
	{
		public:
			HBox(Rect frame, int spacing, Widget *parent=NULL);

			virtual void recalculateChildrenFrames();
	};
}

#endif //BOX_H
