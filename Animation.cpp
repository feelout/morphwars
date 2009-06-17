#include "Animation.h"
#include "Timer.h"

using namespace Graphics;

Animation::Animation(Surface *imgStrip, int width, int height, int frameTime)
	: frameTime(frameTime), loop(false), animating(false)
{
	setCurrentFrame(0);
	surfaces = Surface::splitSpriteStrip(imgStrip, width, height);
}

void Animation::setFrameTime(unsigned int frameTime)
{
	this->frameTime = frameTime;
}

unsigned int Animation::getFrameTime()
{
	return frameTime;
}

void Animation::setLooped(bool looped)
{
	this->loop = looped;
}

bool Animation::isLooped()
{
	return loop;
}

int Animation::getCurrentFrame()
{
	return currentFrame;
}

void Animation::setCurrentFrame(int frame)
{
	currentFrame = frame;
	if(frame >= surfaces.size())
	{
		currentFrame = 0;
	}
	lastFrameTime = Utility::Timer::currentTicks();
}

Surface* Animation::getFrame(int framenum)
{
	return surfaces[framenum];
}

void Animation::start()
{
	setCurrentFrame(0);
	animating = true;
}

void Animation::stop()
{
	setCurrentFrame(0);
	animating = false;
}

void Animation::pause()
{
	animating = false;
}

void Animation::resume()
{
	animating = true;
}

void Animation::draw(Drawer *target, int x, int y)
{
	//Ugly. TODO: Think about moving blit to Drawer
	getFrame(getCurrentFrame())->blit(target->getTarget(), x, y);
}

void Animation::update()
{
	if(animating)
	{
		if(Utility::Timer::currentTicks() >= (lastFrameTime + frameTime))
		{
			setCurrentFrame(++currentFrame);
		}
	}
}
