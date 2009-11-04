#include "AnimationManager.h"
#include "Surface.h"
#include "Logger.h"
#include <stdio.h>

using namespace Graphics;

AnimationManager* AnimationManager::instance = NULL;

AnimationManager::AnimationManager()
{
	//ctor
}

bool AnimationManager::loadAnimation(std::string type)
{
	Surface *sfcStrip = new Surface("Gfx/"+type+".png");

	FILE *f_def = fopen(("Gfx/"+type+".def").c_str(), "r");

	if(!f_def)
	{
		Utility::Logger::getInstance()->log("Definition file for %s not found.\n", type.c_str());
		return false;
	}

	int frameW, frameH, frameTime, looped;

	fscanf(f_def,"%ix%i\n%i\n%i\n",&frameW, &frameH, &frameTime, &looped);

	Animation *anim = new Animation(sfcStrip, frameW, frameH, frameTime);
	anim->setLooped(looped == 1);

	animations[type] = anim;

	delete sfcStrip;
	fclose(f_def);

	return true;
}

AnimationManager* AnimationManager::getInstance()
{
	if(!instance)
	{
		instance = new AnimationManager();
	}

	return instance;
}

Animation* AnimationManager::getAnimation(std::string type)
{
	if(!animations[type])
	{
		if(loadAnimation(type))
		{
			Utility::Logger::getInstance()->log("Animation %s loaded\n", type.c_str());
		}
		else
		{
			Utility::Logger::getInstance()->log("Failed to load animation %s\n", type.c_str());
			return NULL;
		}
	}

	/* We return copy of animation to allow separate states for each object */
	Animation *result = new Animation(animations[type]);
	/* Register animation copy, so it could be updated */
	registeredAnimations.push_back(result);

	return result;
}

void AnimationManager::addAnimation(std::string name, Animation *animation)
{
	if(animations[name])
	{
		Utility::Logger::getInstance()->log("AnimationManager: Type %s already exists.\n", name.c_str());
		return;
	}
	animations[name] = animation;
	registeredAnimations.push_back(animation);
}

void AnimationManager::updateAnimations()
{
	std::list<Animation*>::iterator i;

	for(i = registeredAnimations.begin(); i != registeredAnimations.end(); ++i)
	{
		if(*i)
		{
			(*i)->update();
		}
		else //Animation was freed
		{
			registeredAnimations.erase(i);
		}
	}
}
