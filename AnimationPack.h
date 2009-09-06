#ifndef ANIMATIONPACK_H
#define ANIMATIONPACK_H

#include <map>
#include <string>
#include "Animation.h"

namespace Graphics
{
	/**
	 * Animation pack holds a collection
	 * of named animations. For example, each
	 * Unit requires animation depending on its state:
	 * Moving (one for each direction), attacking (also),
	 * dying, using skill */
	class AnimationPack
	{
		protected:
			std::map<std::string, Animation *> animations;
			Animation *current;

		public:
			AnimationPack();
			/**
			 * Creates animation pack from a file
			 * @param strip Surface to make animations from
			 * @param definition Path to a definition file
			 */
			AnimationPack(Surface *strip, std::string definition);

			AnimationPack(const AnimationPack& source);
			~AnimationPack();

			/**
			 * Adds animation to pack
			 * @param id of the animation. May be different from one`s true name
			 * @param animation Animation itself
			 */
			void addAnimation(std::string name, Animation *animation);
			/**
			 * Sets current animation
			 * @param name Animation name
			 */
			void changeToAnimation(std::string name);

			/**
			 * Returns current animation
			 * @return Current animtion
			 */
			Animation* getCurrent() const;

			AnimationPack& operator = (const AnimationPack& other);
	};
}

#endif //ANIMATIONPACK_H
