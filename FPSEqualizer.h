#ifndef FPSEQUALIZER_H
#define FPSEQUALIZER_H

namespace Utility
{
	class FPSEqualizer
	{
		private:
			unsigned int fps;
			unsigned int frameStartTime;
		public:
			FPSEqualizer(int desiredFPS);
			void frameStarted();
			void frameEnded();
	};
}

#endif //FPSEQUALIZER_H
