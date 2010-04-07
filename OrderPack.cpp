#include "OrderPack.h"
#include "MouseSelector.h"
#include "Surface.h"
#include "Drawer.h"
#include "Logger.h"

using namespace Gui;
using namespace Core;

const int OrderPack::ORDER_BUTTON_WIDTH;
const int OrderPack::ORDER_BUTTON_HEIGHT;

OrderPack::OrderPack(Rect frame)
	: HBox(frame, 5)
{
	const int BUTTONS_NUM = 4;
	std::string names[BUTTONS_NUM] = {"Gfx/Actions/Move.png", "Gfx/Actions/Stop.png", "Gfx/Actions/Attack.png",
				"Gfx/Actions/Skill.png"};
	ButtonClickCallback callbacks[BUTTONS_NUM] = {MoveButtonCb, StopButtonCb, AttackButtonCb, SkillButtonCb};
	ImageButton *currentButton = NULL;

	for(int i=0; i < BUTTONS_NUM; ++i)
	{
		/*currentButton = new ImageButton(Rect(5 + frame.x + (ORDER_BUTTON_WIDTH+5)*i, frame.y+5,
					ORDER_BUTTON_WIDTH, ORDER_BUTTON_HEIGHT), Graphics::Surface(names[i]));*/
		currentButton = new ImageButton(Rect(0,0, ORDER_BUTTON_WIDTH, ORDER_BUTTON_HEIGHT),
				Graphics::Surface(names[i]));
		currentButton->setOnClicked(callbacks[i]);
		addChild(currentButton, false, 0);
	}
}

void OrderPack::draw(Graphics::Surface *target)
{
	HBox::draw(target);
	Graphics::Drawer(target).drawRect(frame, RGBColor::WHITE);
}

