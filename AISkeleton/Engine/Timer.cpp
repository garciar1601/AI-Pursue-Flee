#ifndef DEBUG_OFF
#include "Timer.h"

void Timer::Start(){
	QueryPerformanceCounter(&start);
}

float Timer::Stop(){
	LARGE_INTEGER stop;
	QueryPerformanceCounter(&stop);
	LARGE_INTEGER elapsed;
	elapsed.QuadPart = stop.QuadPart - start.QuadPart;
	float seconds = ((float)elapsed.QuadPart) / timeFrequency.QuadPart;
	return seconds;
}

float Timer::Interval(){
	LARGE_INTEGER current;
	QueryPerformanceCounter(&current);
	LARGE_INTEGER delta;
	delta.QuadPart = current.QuadPart - timeLastFrame.QuadPart;
	timeLastFrame = current;
	float seconds = ((float)delta.QuadPart) / timeFrequency.QuadPart;
	return seconds;
}
#endif