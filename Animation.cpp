#include "Animation.h"

using namespace Graphics;

Animation::Animation(Surface *imgStrip, int width, int height, int frameTime)
	: currentFrame(0), frameTime(frameTime), loop(false), animating(false)
{
	surfaces = Surface::splitSpriteStrip(imgStrip, width, height);
}

void Animation::setFrameTime(int frameTime)
{
	this->frameTime = frameTime;
}

int Animation::getFrameTime()
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
}

Surface* Animation::getFrame(int framenum)
{
	return surface[framenum];
}

void Animation::start()
{
	currentFrame = 0;
	animating = true;
}

void Animation::stop()
{
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
	target->blit(getFrame(getCurrentFrame()), x, y);
}
