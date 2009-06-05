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
			int frameTime;
			bool loop;
		public:
			Animation(Surface *imgStrip, int frameTime);
			Animation(int frameTime);

			void setFrameTime(int frameTime);
			int getFrameTime();

			void setLooped(bool looped);
			bool isLooped();
	};
}

#endif //ANIMATION_H
