#include <stdio.h>
#include "AnimationPack.h"
#include "AnimationManager.h"
#include "Logger.h"
#include "tinyxml.h"
#include "Timer.h"

using namespace Graphics;

AnimationPack::AnimationPack()
{
}

AnimationPack::AnimationPack(Surface *strip, std::string definition)
	: current(NULL), previous(NULL)
{
	Utility::Logger::getInstance()->log("Loading animation pack %s\n", definition.c_str());

	TiXmlDocument xmldoc(definition.c_str());
	if(!xmldoc.LoadFile())
	{
		Utility::Logger::getInstance()->log("Failed to load animation pack XML file : %s.\n", definition.c_str());
		return;
	}

	TiXmlElement *parent = xmldoc.FirstChildElement("animationpack");

	if(!parent)
	{
		Utility::Logger::getInstance()->log("Missing <animationpack> parent tag.\n");
		return;
	}

	TiXmlNode *currentChild=NULL;
	TiXmlElement *currentXMLAnimation=NULL;
	Animation *currentAnimation;

	std::string name;
	int width, height;
	int frameTime;
	int strip_y=0; //Y coordinate of current animation strip on pack strip 

	while(currentChild = parent->IterateChildren(currentChild))
	{
		currentXMLAnimation = currentChild->ToElement();

		name = currentXMLAnimation->Attribute("name");
		currentXMLAnimation->QueryIntAttribute("frameTime", &frameTime);
		currentXMLAnimation->QueryIntAttribute("width", &width);
		currentXMLAnimation->QueryIntAttribute("height", &height);

		Surface *animationImageStrip = new Surface(strip->getWidth(), height);
		Rect blittingRect(0, strip_y, strip->getWidth(), strip_y+height);
		strip->blit(animationImageStrip, &blittingRect, 0, 0);

		currentAnimation = new Animation(animationImageStrip, width, height, frameTime);

		AnimationManager::getInstance()->addAnimation(name, currentAnimation);
		addAnimation(name, currentAnimation);

		strip_y += width;
	}

	current = currentAnimation;
}

AnimationPack::AnimationPack(const AnimationPack& source)
	: current(NULL)
{
	std::map<std::string, Animation*>::iterator i;

	for(i = animations.begin(); i != animations.end(); ++i)
	{
		addAnimation(i->first, AnimationManager::getInstance()->getAnimation(i->first));
	}

	i = animations.begin();

	changeToAnimation(i->first);
}

AnimationPack::~AnimationPack()
{
	// TODO: Check if map::clear() calls destructors
	animations.clear();
}

void AnimationPack::addAnimation(std::string name, Animation *animation)
{
	//Utility::Logger::getInstance()->log("AddAnimation(%s)\n", name.c_str());
	animations[name] = animation;
}

void AnimationPack::changeToAnimation(std::string name)
{
	//Utility::Logger::getInstance()->log("AnimationPack::changeToAnimation(%s)\n", name.c_str());
	//DEBUG
	if(previous)
	{
		previous = animations[name];
		return;
	}

	if(current)
	{
		current->stop();
	}
	//ENDDEBUG
	//Utility::Logger::getInstance()->log("Animations total: %i\n", animations.size());

	if(!animations[name])
	{
		Utility::Logger::getInstance()->log("AnimationPack: missing animation requested: %s\n", name.c_str());
		return;
	}

	if(current)
	{
		current->stop();
	}

	current = animations[name];
} 

void AnimationPack::playTimedAnimation(std::string name, unsigned int time)
{
	timedAnimationStart = Utility::Timer::currentTicks();
	timedAnimationPlayingTime = time;
	previous = current;
	current = animations[name];

	AnimationManager::getInstance()->addTimedAnimationPack(this);
}

Animation* AnimationPack::getCurrent() const
{
	return current;
}

AnimationPack& AnimationPack::operator = (const AnimationPack& other)
{
	//Utility::Logger::getInstance()->log("AnimationPack::operator=\n");

	current = NULL;

	if(this != &other)
	{
		std::map<std::string, Animation *>::const_iterator i;

		//Utility::Logger::getInstance()->log("\tCopying animations\n");
		for(i = other.animations.begin(); i != other.animations.end(); ++i)
		{
			Animation *anim = AnimationManager::getInstance()->getAnimation(i->first);
			//Utility::Logger::getInstance()->log("\t\tCopying %s\n", i->first.c_str());
			addAnimation(i->first, AnimationManager::getInstance()->getAnimation(i->first));
		}

		i = animations.begin();

		changeToAnimation(i->first);
	}
	return *this;
}

bool AnimationPack::updateTimedAnimation()
{
	if(!previous)
		return true;

	if(Utility::Timer::currentTicks() - timedAnimationStart > timedAnimationPlayingTime)
	{
		current = previous;
		previous = NULL;
	}

	return false;
}
