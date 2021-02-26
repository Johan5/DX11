#include "frame_timer.h"

using namespace std::chrono;

double CFrameTimer::StartNewFrame()
{
	time_point<steady_clock> CurrentTime = high_resolution_clock::now();
	long long TimeElapsedMicroSec = duration_cast<microseconds>( CurrentTime - _StartNewFrameTimeStamp ).count();
	double TimeElapsedMilliSec = static_cast<double>( TimeElapsedMicroSec  ) / 1000.0;
	_StartNewFrameTimeStamp = CurrentTime;
	return TimeElapsedMilliSec;
}
