#include "Animation.h"
#include "Timer.h"

using namespace Graphics;

Animation::Animation(Surface *imgStrip, int width, int height, int frameTime)
	: frameTime(frameTime), loop(false), animating(false)
{
    setCurrentFrame(0);
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
	lastFrameTime =Timer::currentTicks();
}

Surface* Animation::getFrame(int framenum)
{
	return surface[framenum];
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
	getFrame(getCurrentFrame())->blit(target->getSurface(), x, y);
}

void Animation::update()
{
    if(animating)
    {
        if(Timer::currentTicks() >= (lastFrameTime + frameTime))
        {
            setCurrentFrame(++currentFrame);
        }
    }
}
