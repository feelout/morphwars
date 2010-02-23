#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include "Surface.h"

namespace Graphics
{
	class Animation
	{
		private:
			std::vector<Surface*> surfaces;
			int currentFrame;
			unsigned int frameTime;
			unsigned int lastFrameTime;
			bool loop;
			bool animating;
		public:
			Animation(Surface *imgStrip, int width, int height, int frameTime);
			Animation(Animation *tocopy);

			void setFrameTime(unsigned int frameTime);
			unsigned int getFrameTime() const;

			void setLooped(bool looped);
			bool isLooped() const;

			int getCurrentFrame() const;
			void setCurrentFrame(int frame);
			Surface *getFrame(int framenum) const;

			void start();
			void stop();
			void pause();
			void resume();

			void draw(Surface *target, int x, int y) const;
			void update();
	};
}

#endif //ANIMATION_H
