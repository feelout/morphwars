#include "Minimap.h"
#include "Map.h"

using namespace Gui;

Minimap::Minimap(Rect frame, Core::Map *owner)
	: Widget(frame), owner(owner), minimap(NULL)
{
	// Rendering map to temp surface
	Graphics::Surface nonScaledMinimap((owner->getWidth()+1)*Core::TILE_WIDTH, (owner->getHeight() / 2)*Core::TILE_HEIGHT);
	Graphics::Drawer nonScaledDrawer(&nonScaledMinimap);
	Core::FieldOfView empty_fov(owner->getWidth(), owner->getHeight());

	owner->draw(&nonScaledDrawer, &empty_fov);
	minimap = nonScaledMinimap.zoom(static_cast<float>(frame.w) / nonScaledMinimap.getWidth(),
		       	static_cast<float>(frame.h) / nonScaledMinimap.getHeight(), false);
}

void Minimap::mouseLMBClicked(int x, int y)
{
	//Shift map clipping rect
}

void Minimap::draw(Graphics::Drawer *target)
{
	// Draw minimap, then clipping rect
}
