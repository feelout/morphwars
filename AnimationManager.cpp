#include "AnimationManager.h"
#include "Surface.h"

using namespace Graphics;

AnimationManager* AnimationManager::instance = NULL;

AnimationManager::AnimationManager()
{
    //ctor
}

bool AnimationManager::loadAnimation(std::string type)
{
    Surface *sfsStrip = new Surface("animations/"+type+".png");
    //Animation *anim = new Animation(//Shit, we need definition file
}

AnimationManager* AnimationManager::getInstance()
{
    if(!instance)
    {
        instance = new AnimationManager();
    }

    return instance;
}
