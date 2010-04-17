#include "Animation.h"
#include "Timer.h"
#include "Logger.h"

using namespace Graphics;

Animation::Animation(Surface *imgStrip, int width, int height, int frameTime)
	: frameTime(frameTime), loop(false), animating(false)
{
	setCurrentFrame(0);
	surfaces = Surface::splitSpriteStrip(imgStrip, width, height);
}

Animation::Animation(Animation *tocopy)
	: surfaces(tocopy->surfaces), loop(tocopy->loop), currentFrame(0), frameTime(tocopy->frameTime),
	animating(false)
{
}

void Animation::setFrameTime(unsigned int frameTime)
{
	this->frameTime = frameTime;
}

unsigned int Animation::getFrameTime() const
{
	return frameTime;
}

void Animation::setLooped(bool looped)
{
	this->loop = looped;
}

bool Animation::isLooped() const
{
	return loop;
}

int Animation::getCurrentFrame() const
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

Surface* Animation::getFrame(int framenum) const
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

void Animation::draw(Surface *target, int x, int y) const
{
	//Ugly. TODO: Think about moving blit to Drawer
	getFrame(getCurrentFrame())->blit(target, x, y);
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
