#include "FPSEqualizer.h"
#include "Timer.h"

/** DELETE ME **/
#include <stdio.h>

using namespace Utility;

FPSEqualizer::FPSEqualizer(int desiredFPS)
	: fps(desiredFPS)
{
}

void FPSEqualizer::frameStarted()
{
	frameStartTime = Timer::currentTicks();
}

void FPSEqualizer::frameEnded()
{
	unsigned int neededTime = frameStartTime + (1000/fps);
	if(neededTime < Timer::currentTicks())
	{
		return;
	}
	Timer::delay(neededTime - Timer::currentTicks());
}
