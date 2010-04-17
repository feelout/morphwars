#include "EngineState.h"
#include "Logger.h"

using namespace Core;

EngineState::EngineState(std::string name)
	: enabled(false), drawing(false), name(name)
{
	Utility::Logger::getInstance()->log("Engine state %s created.\n", name.c_str());
}

bool EngineState::isEnabled() const
{
	return enabled;
}

void EngineState::setEnabled(bool enabled)
{
	this->enabled = enabled;
}

bool EngineState::doDrawing() const
{
	return drawing;
}

void EngineState::setDrawingState(bool drawing)
{
	this->drawing = drawing;
}

std::string EngineState::getName() const
{
	return name;
}

void EngineState::pause()
{
	enabled = false;
	drawing = true;
}

void EngineState::resume()
{
	enabled = true;
	drawing = true;
}

bool EngineState::mouseMoved(int x, int y)
{
	return false;
}

bool EngineState::mouseLMBClicked(int x, int y)
{
	return false;
}

bool EngineState::mouseRMBClicked(int x, int y)
{
	return false;
}

bool EngineState::keyPressed(int key)
{
	return false;
}
