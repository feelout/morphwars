#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include <map>
#include <string>
#include <list>
#include "Animation.h"

/**
 * SHIT! we need to update animations individually. So, instead
 * of returning the only reference to gived animations, we must
 * create a copy. But we can cache images, of course. */
namespace Graphics
{
    /**
     * Animation manager is entry point for loading
     * animations, also being a cache.
     */
    class AnimationManager
    {
        private:
            AnimationManager();

            static AnimationManager *instance;

	    /**
	     * List of all animation classes
	     */
            std::map<std::string, Animation*> animations;
	    /**
	     * List of all animation copies in game.
	     */
	    std::list<Animation*> registeredAnimations;

	    /**
	     * Loads animation with given type
	     * TODO: Maybe, move to Animation class
	     */
            bool loadAnimation(std::string type);
        public:
            /**
             * Get singleton instance
             * @return the only instance
             */
            static AnimationManager* getInstance();

            /**
             * Get requested animation
             * @param type Animation type
             * @return animation with given type
             */
            Animation *getAnimation(std::string type);

	    void addAnimation(std::string name, Animation *animation);

	    /**
	     * Update all registered animations. Call each frame.
	     */
	    void updateAnimations();
    };
}

#endif // ANIMATIONMANAGER_H
