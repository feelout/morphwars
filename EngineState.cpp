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

void EngineState::mouseMoved(int x, int y)
{
}

void EngineState::mouseLMBClicked(int x, int y)
{
}

void EngineState::mouseRMBClicked(int x, int y)
{
}

void EngineState::keyPressed(int key)
{
}
