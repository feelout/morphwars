#include "Menu.h"
#include "Engine.h"
#include "Logger.h"
#include "ScenarioSelector.h"
#include <stdio.h>

using namespace Core;

const int MENU_BUTTON_WIDTH = 240;
const int MENU_BUTTON_HEIGHT = 50;

const int MenuState::BUTTONS_NUM;

void CampaignButtonCallback()
{
}

void ScenarioButtonCallback()
{
	//Engine::getInstance()->loadScenario("Maps\\Test\\Test.xml");
	Graphics::Renderer *renderer = Engine::getInstance()->getRenderer();
	Engine::getInstance()->changeState(new ScenarioSelectingState(renderer->getWidth(), renderer->getHeight()));
}

void LoadButtonCallback()
{
}

void OptionsButtonCallback()
{
}

void CreditsButtonCallback()
{
}

void QuitButtonCallback()
{
	Utility::Logger::getInstance()->log("Quit button clicked\n");
	Engine::getInstance()->stop();
}

MenuState::MenuState(int screen_w, int screen_h)
	: EngineState("MainMenu")
{
	if( (screen_h < MENU_BUTTON_HEIGHT*BUTTONS_NUM) || (screen_w < MENU_BUTTON_WIDTH) )
	{
		Utility::Logger::getInstance()->log("Not enough screen space for showing main menu");
		return;
	}

	std::string btn_str("Gfx/Menu/Button");
	char buf[10];

	for(int i=0; i < BUTTONS_NUM; ++i)
	{
		Rect frame(screen_w-MENU_BUTTON_WIDTH, i*MENU_BUTTON_HEIGHT, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);
		sprintf(buf, "%i", i);
		//Graphics::Surface surf(btn_str + buf + ".png"); //Here`s the bug
		//buttons[i] = new Gui::ImageButton(frame, surf);
		//BUG! Temporary object gets destroyed!!
		buttons[i] = new Gui::ImageButton(frame, Graphics::Surface(btn_str + buf + ".png"));
	}

	buttons[CAMPAIGN]->setOnClicked(CampaignButtonCallback);
	buttons[SCENARIO]->setOnClicked(ScenarioButtonCallback);
	buttons[LOADGAME]->setOnClicked(LoadButtonCallback);
	buttons[OPTIONS]->setOnClicked(OptionsButtonCallback);
	buttons[CREDITS]->setOnClicked(CreditsButtonCallback);
	buttons[QUIT]->setOnClicked(QuitButtonCallback);
}

void MenuState::draw(Graphics::Surface *target)
{
	for(int i=0; i < BUTTONS_NUM; ++i)
	{
		buttons[i]->draw(target);
	}
}

void MenuState::process()
{
}

bool MenuState::mouseLMBClicked(int x, int y)
{
	for(int i=0; i < BUTTONS_NUM; ++i)
	{
		if(buttons[i]->mouseLMBClicked(x,y))
			return true;
	}

	return false;
}
