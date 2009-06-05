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

			void setFrameTime(unsigned int frameTime);
			unsigned int getFrameTime();

			void setLooped(bool looped);
			bool isLooped();

			int getCurrentFrame();
			void setCurrentFrame(int frame);
			Surface *getFrame(int framenum);

			void start();
			void stop();
			void pause();
			void resume();

			void draw(Drawer *target);
	};
}

#endif //ANIMATION_H
