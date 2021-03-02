#include "EngineTimer.h"

using namespace std::chrono;

EngineTimer::EngineTimer()
{
	last = steady_clock::now();
}

// Returns the time between the last time you called Mark() and now
float EngineTimer::Mark()
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;

	return frameTime.count();
}

// Returns the time between the last time you called Mark() without resetting last
float EngineTimer::Peek() const
{
	return duration<float>(steady_clock::now() - last).count();
}