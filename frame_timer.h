#pragma once

#include <chrono>

class CFrameTimer
{
public:
	// Returns time elapsed since last time this function was called (or 0.0 is called for first time)
	double StartNewFrame();	
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> _StartNewFrameTimeStamp;
};

