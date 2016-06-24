#ifndef PROFILER_H
#define PROFILER_H
#include "ExportHeader.h"
#include "Assert.h"
#include "Logger.h"

class Profiler{
public:
	ENGINE_SHARED static Profiler& getInstance();
private:
	Profiler() {}
	Profiler(const Profiler&);
	Profiler& operator=(const Profiler&);
	static Profiler theInstance;
#if PROFILING_ON
	const char* fileName;
	static const unsigned int MAX_FRAME_SAMPLES = 100;
	static const unsigned int MAX_PROFILE_CATEGORIES = 20;
	int frameIndex;
	unsigned int categoryIndex;
	unsigned int numUsedCategories;
	struct ProfileCategory{
		const char* name;
		float samples[MAX_FRAME_SAMPLES];
	} categories[MAX_PROFILE_CATEGORIES];
	bool currentFrameComplete() const;
	bool wrapped() const;
	void writeFrame(unsigned int frameNumber) const;
	void writeData() const;
#endif
public:
#if PROFILING_ON
	ENGINE_SHARED void initialize(const char* fileName);
	ENGINE_SHARED void shutdown();
	ENGINE_SHARED void newFrame();
	ENGINE_SHARED void addEntry(const char* category, float time);
#else
	ENGINE_SHARED void initialize(const char* fileName){}
	ENGINE_SHARED void shutdown(){}
	ENGINE_SHARED void newFrame(){}
	ENGINE_SHARED void addEntry(const char* category, float time){}
#endif
};

#define profiler Profiler::getInstance()

#endif