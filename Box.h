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

			/* VBox and HBox differ only in dimension (height, width) they
			 * operate on. With those methods we pull common strategy up */
			virtual int getDimension(Rect &childFrame)=0;
			virtual void setDimension(Rect &childFrame, int value)=0;

			virtual void setMinorDimension(Rect &childFrame, int value)=0;
			virtual int getMinorDimension(Rect &childFrame)=0;

			virtual int getSubDimension(Rect &childFrame)=0;
			virtual void setSubDimension(Rect &childFrame, int value)=0;

			virtual void setMinorSubDimension(Rect &childFrame, int value)=0;
			virtual int getMinorSubDimension(Rect &childFrame)=0;
		public:
			Box(Rect frame, int spacing, Widget *parent=NULL);
			virtual ~Box();

			virtual void addChild(Widget *child, bool expandable, int spacing);
			virtual void removeChild(Widget *child);
			virtual void removeAll();

			virtual void recalculateChildrenFrames();
	};

	class HBox : public Box
	{
		protected:
			virtual int getDimension(Rect &childFrame);
			virtual void setDimension(Rect &childFrame, int value);

			virtual void setMinorDimension(Rect &childFrame, int value);
			virtual int getMinorDimension(Rect &childFrame);

			virtual int getSubDimension(Rect &childFrame);
			virtual void setSubDimension(Rect &childFrame, int value);

			virtual void setMinorSubDimension(Rect &childFrame, int value);
			virtual int getMinorSubDimension(Rect &childFrame);
		public:
			HBox(Rect frame, int spacing, Widget *parent=NULL);

			//virtual void recalculateChildrenFrames();
	};

	class VBox : public Box
	{
		protected:
			virtual int getDimension(Rect &childFrame);
			virtual void setDimension(Rect &childFrame, int value);

			virtual void setMinorDimension(Rect &childFrame, int value);
			virtual int getMinorDimension(Rect &childFrame);

			virtual int getSubDimension(Rect &childFrame);
			virtual void setSubDimension(Rect &childFrame, int value);

			virtual void setMinorSubDimension(Rect &childFrame, int value);
			virtual int getMinorSubDimension(Rect &childFrame);
		public:
			VBox(Rect frame, int spacing, Widget *parent=NULL);

			//virtual void recalculateChildrenFrames();
	};
}

#endif //BOX_H
