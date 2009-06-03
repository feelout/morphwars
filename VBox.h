#ifndef VBOX_H
#define VBOX_H

#include "Box.h"

namespace Gui
{
	class VBox : public Box
	{
		protected:
			virtual void organiseChildren();
			virtual void calculateRequest();
		public:
			VBox(Rect frame, Widget *parent = NULL);

			void setFrame(Rect frame);
	};
}

#endif //VBOX_H
