#include "MouseSelector.h"
#include "Logger.h"
#include <SDL/SDL.h>

using namespace Core;

MouseState* MouseState::instance = NULL;

// TODO: Disable SDL cursor
MouseState::MouseState()
	: x(0), y(0)
{
	cursor[0] = new Graphics::Surface("Gfx/Cursors/Select.png");
	cursor[1] = new Graphics::Surface("Gfx/Cursors/Move.png");
	cursor[2] = new Graphics::Surface("Gfx/Cursors/Attack.png");
	cursor[3] = new Graphics::Surface("Gfx/Cursors/Skill.png");

	SDL_ShowCursor(SDL_DISABLE);

	setActionType(SELECT);
}

MouseState::~MouseState()
{
	for(int i=0; i < 4; ++i)
	{
		delete cursor[i];
	}

	SDL_ShowCursor(SDL_ENABLE);
}

MouseState* MouseState::getInstance()
{
	if(!instance)
		instance = new MouseState();

	return instance;
}

void MouseState::draw(Graphics::Drawer *target)
{
	cursor[currentType]->blit(target->getTarget(), x, y);
}

void MouseState::setActionType(ActionType type)
{
	currentType = type;
}

MouseState::ActionType MouseState::getActionType() const
{
	return currentType;
}

bool MouseState::mouseMoved(int x, int y)
{
	this->x = x;
	this->y = y;

	return false;
}

/*bool MouseState::mouseLMBClicked(int x, int y)
{
	return false;
}

bool MouseState::mouseRMBClicked(int x, int y)
{
	return false;
}

bool MouseState::keyPressed(int key)
{
	return false;
}*/
