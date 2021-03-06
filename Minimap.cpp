#include "Minimap.h"
#include "Map.h"
#include "Drawer.h"

using namespace Gui;

Minimap::Minimap(Rect frame, Core::Map *owner)
	: Widget(frame), owner(owner), minimap(NULL)
{
	// Rendering map to temp surface
	Graphics::Surface nonScaledMinimap((owner->getWidth()+1)*Core::TILE_WIDTH, (owner->getHeight() / 2)*Core::TILE_HEIGHT);

	// Disable fog of war by using stub FOV
	Core::FieldOfView empty_fov(owner->getWidth(), owner->getHeight());

	for(int y=0; y < owner->getHeight(); ++y)
	{
		for(int x=0; x < owner->getWidth(); ++x)
		{
			empty_fov.setTileVisible(x, y, true);
		}
	}

	// Temporarily extend clip to whole map
	Rect true_clip = owner->getClipping();
	Rect clip = true_clip;
	clip.w = owner->getWidth();
	clip.h = owner->getHeight();

	owner->setClipping(clip);
	owner->draw(&nonScaledMinimap, &empty_fov, false);
	owner->setClipping(true_clip);

	// Zoom to needed dimensions
	// FIXME: preserve aspect ratio
	
	float aspect_ratio = 0.0;

	if(nonScaledMinimap.getWidth() > nonScaledMinimap.getHeight())
	{
		aspect_ratio = static_cast<float>(frame.w) / nonScaledMinimap.getWidth();
	}
	else
	{
		aspect_ratio = static_cast<float>(frame.h) / nonScaledMinimap.getHeight();
	}

	/*minimap = nonScaledMinimap.zoom(static_cast<float>(frame.w) / nonScaledMinimap.getWidth(),
		       	static_cast<float>(frame.h) / nonScaledMinimap.getHeight(), false);*/
	minimap = nonScaledMinimap.zoom(aspect_ratio, aspect_ratio, false);
}

bool Minimap::mouseLMBClicked(int x, int y)
{
	if(!frame.isPointInRect(x, y))
		return false;
	//Shift map clipping rect
	return false;
}

void Minimap::draw(Graphics::Surface *target)
{
	// Draw minimap, then clipping rect
	minimap->blit(target, frame.x, frame.y);
	Graphics::Drawer(target).drawRect(frame, RGBColor::WHITE);
}
