#include "Timer.h"
#include <SDL/SDL.h>

using namespace Utility;

unsigned int Timer::currentTicks()
{
	return SDL_GetTicks();
}

void Timer::delay(unsigned int time)
{
	SDL_Delay(time);
}
