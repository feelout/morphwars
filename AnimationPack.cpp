#include "AnimationPack.h"

using namespace Graphics;

AnimationPack::AnimationPack()
{
}

AnimationPack::~AnimationPack()
{
	// TODO: Check if map::clear() calls destructors
	animations.clear();
}

void AnimationPack::addAnimation(std::string name, Animation *animation)
{
	animations[name] = animation;
}

void AnimationPack::changeToAnimation(std::string name)
{
	current->stop();
	current = animations[name];
}

Animation* AnimationPack::getCurrent() const
{
	return current;
}
