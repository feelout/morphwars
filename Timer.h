#ifndef TIMER_H
#define TIMER_H

namespace Utility
{
	class Timer
	{
		private:
		public:
			static unsigned int currentTicks();
			static void delay(unsigned int time);
	};
}

#endif //TIMER_H
