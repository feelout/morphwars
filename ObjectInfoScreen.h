#ifndef OBJECT_INFO_SCREEN_H
#define OBJECT_INFO_SCREEN_H

#include "Widget.h"

namespace Core
{
	class MapObject;
	class Unit;
}

namespace Gui
{
	class ObjectInfoScreen : public Widget
	{
		protected:
			Core::MapObject *object;
		public:
			ObjectInfoScreen(Core::MapObject *object, Widget *parent=NULL);

			virtual void draw(Graphics::Surface *target);
	};

	class UnitInfoScreen : public ObjectInfoScreen
	{
		public:
			UnitInfoScreen(Core::Unit *object, Widget *parent=NULL);
			virtual void draw(Graphics::Surface *target);
	};

	/*class BuildingInfoScreen : public ObjectInfoScreen
	{
		virtual void draw(Graphics::Surface *target);
	};*/
};

#endif // OBJECT_INFO_SCREEN_H
