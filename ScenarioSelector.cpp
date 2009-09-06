#include "ScenarioSelector.h"
#include "tinyxml.h"
#include "assist.h"
#include "Engine.h"
#include "Logger.h"
#include "FontSystem.h"

using namespace Gui;
using namespace Core;

const int PREVIEW_IMAGE_HEIGHT = 100;

/* ScenarioSelector */
Gui::ScenarioSelector::ScenarioSelector(Rect frame, Core::ScenarioSelectingState *state)
	: List(frame), state(state)
{
	Utility::Logger::getInstance()->log("Creating list of files in folder\n");
	std::vector<std::string> scenarios = ListFilesInFolder("Maps");
	items.assign(scenarios.begin(), scenarios.end());

	std::vector<std::string>::iterator iter;

	for(iter = items.begin(); iter != items.end(); ++iter)
	{
		Utility::Logger::getInstance()->log("%s\n", iter->c_str());
	}

	if(scenarios.size() != 0)
	{
		setCurrentItem(0); 	
	}
}

void Gui::ScenarioSelector::itemSelected(int n)
{
	Engine::getInstance()->loadScenario("Maps/"+items[n]+"/"+items[n]+".xml");
}

void Gui::ScenarioSelector::currentItemChanged(int n)
{
	state->setCurrentScenario(items[n]);
}

/* ScenarioSelectingState */
ScenarioSelectingState::ScenarioSelectingState(int screen_w, int screen_h)
	: EngineState("Scenario selecting"), selector(NULL), preview(NULL), info("")
{
	Utility::Logger::getInstance()->log("Creating selector\n");
	selector = new Gui::ScenarioSelector(Rect(5, PREVIEW_IMAGE_HEIGHT+5, screen_w-10, screen_h - PREVIEW_IMAGE_HEIGHT - 10), this);
}

ScenarioSelectingState::~ScenarioSelectingState()
{
	delete selector;
}

void ScenarioSelectingState::setCurrentScenario(std::string scenario)
{
	std::string path = "Maps/" + scenario + "/";
	TiXmlDocument xmldoc((path+scenario+".xml").c_str());

	if(!xmldoc.LoadFile())
	{
		Utility::Logger::getInstance()->log("Scenario %s not found\n", path.c_str());
		return;
	}

	TiXmlElement *el_scenario = xmldoc.FirstChildElement("scenario");

	std::string name = el_scenario->Attribute("name");
	std::string author = el_scenario->Attribute("author");
	std::string version = el_scenario->Attribute("version");
	//TODO: add map dimensions
	info = name + " " + version + " by " + author;

	if(preview)
	{
		delete preview;
		preview = NULL;
	}

	//FIXME : zoom if needed
	preview = new Graphics::Surface(path+"preview.png");
}

void ScenarioSelectingState::process()
{
}

void ScenarioSelectingState::draw(Graphics::Drawer *target)
{
	//FIXME : draw everything
	preview->blit(target->getTarget(), 5, 5);
	target->drawRect(Rect(5, 5, preview->getWidth(), preview->getHeight()), RGBColor::WHITE);
	selector->draw(target);
	Graphics::FontSystem::getInstance()->print(target, info, preview->getWidth()+10, 5, RGBColor(255, 0, 0));
}

bool ScenarioSelectingState::mouseMoved(int x, int y)
{
	return selector->mouseMoved(x,y);
}

bool ScenarioSelectingState::mouseLMBClicked(int x, int y)
{
	return selector->mouseLMBClicked(x,y);
}
