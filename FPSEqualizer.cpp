#include "FPSEqualizer.h"
#include "Timer.h"

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
	Timer::delay((frameStartTime+(1000/fps))-Timer::currentTicks());
}
