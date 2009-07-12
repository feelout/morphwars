#include <stdio.h>
#include "AnimationPack.h"
#include "AnimationManager.h"
#include "Logger.h"
#include "tinyxml.h"

using namespace Graphics;

AnimationPack::AnimationPack()
{
}

AnimationPack::AnimationPack(Surface *strip, std::string definition)
{
	FILE *f = fopen(definition.c_str(), "r");

	if(!f)
	{
		Utility::Logger::getInstance()->log("Failed to load animation pack definition %i\n", definition.c_str());
		return;
	}

	fclose(f);

	TiXmlDocument xmldoc(definition.c_str());

	TiXmlNode *parent = xmldoc.FirstChild("animationpack");

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
