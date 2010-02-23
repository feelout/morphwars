#include "OrderPack.h"
#include "MouseSelector.h"
#include "Surface.h"
#include "Drawer.h"
#include "Logger.h"

using namespace Gui;
using namespace Core;

const int OrderPack::ORDER_BUTTON_WIDTH;
const int OrderPack::ORDER_BUTTON_HEIGHT;

void MoveButtonCb()
{
	Core::MouseState::getInstance()->setActionType(MouseState::MOVE);
	Utility::Logger::getInstance()->log("MoveButtonCb called\n");
}

void StopButtonCb()
{
	Core::MouseState::getInstance()->setActionType(MouseState::SELECT);
}

void AttackButtonCb()
{
	Core::MouseState::getInstance()->setActionType(MouseState::ATTACK);
}

void SkillButtonCb()
{
	Core::MouseState::getInstance()->setActionType(MouseState::SKILL);
}

OrderPack::OrderPack(Rect frame)
	: Container(frame)
{
	const int BUTTONS_NUM = 4;
	std::string names[BUTTONS_NUM] = {"Gfx/Actions/Move.png", "Gfx/Actions/Stop.png", "Gfx/Actions/Attack.png",
				"Gfx/Actions/Skill.png"};
	ButtonClickCallback callbacks[BUTTONS_NUM] = {MoveButtonCb, StopButtonCb, AttackButtonCb, SkillButtonCb};
	ImageButton *currentButton = NULL;

	for(int i=0; i < BUTTONS_NUM; ++i)
	{
		currentButton = new ImageButton(Rect(5 + frame.x + (ORDER_BUTTON_WIDTH+5)*i, frame.y+5,
					ORDER_BUTTON_WIDTH, ORDER_BUTTON_HEIGHT), Graphics::Surface(names[i]));
		currentButton->setOnClicked(callbacks[i]);
		addChild(currentButton);
	}
}

void OrderPack::draw(Graphics::Surface *target)
{
	Container::draw(target);
	Graphics::Drawer(target).drawRect(frame, RGBColor::WHITE);
}

