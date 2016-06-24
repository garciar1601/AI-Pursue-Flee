#ifndef TIMER_H
#define TIMER_H
#include <Windows.h>
#include <ExportHeader.h>

class Timer{
public:
#ifndef DEBUG_OFF
	LARGE_INTEGER start;
	LARGE_INTEGER timeFrequency;
	LARGE_INTEGER timeLastFrame;
	Timer(){timeLastFrame.QuadPart = 0; QueryPerformanceFrequency(&timeFrequency);};
	void ENGINE_SHARED Start();
	float ENGINE_SHARED Stop();
	float ENGINE_SHARED Interval();
#else
	ENGINE_SHARED void Start(){}
	ENGINE_SHARED float Stop(){}
	ENGINE_SHARED float Interval(){}
#endif
};

#endif