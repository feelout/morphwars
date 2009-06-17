#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include <map>
#include <string>
#include "Animation.h"

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

            std::map<std::string, Animation*> animations;

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
    };
}

#endif // ANIMATIONMANAGER_H
