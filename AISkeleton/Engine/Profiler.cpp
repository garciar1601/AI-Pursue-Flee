#include "Profiler.h"
Profiler Profiler::theInstance;
Profiler& Profiler::getInstance(){
	return theInstance;
}
#ifdef PROFILING_ON
#include <fstream>
static std::ofstream outStream;

void Profiler::initialize(const char* fileName){
	this->fileName = fileName;
	frameIndex = -1;
	categoryIndex = 0;
	numUsedCategories = 0;
	LOG(Info, "Profiler Initialized");
}

void Profiler::shutdown(){
	writeData();
	LOG(Info, "Profiler shut down");
}

void Profiler::addEntry(const char* category, float time){
	
	ASSERT(categoryIndex < MAX_PROFILE_CATEGORIES, "Number of categories exceeded max capacity.");
	ProfileCategory& pc = categories[categoryIndex];
	if (frameIndex == 0){
		pc.name = category;
		numUsedCategories++;
	}
	else{
		ASSERT(pc.name == category && category != NULL, "Category added did not match category added previous frame.");
		ASSERT(categoryIndex < numUsedCategories, "An extra category was attempted to be added.");
	}
	categoryIndex++;
	pc.samples[frameIndex % MAX_FRAME_SAMPLES] = time;
}

void Profiler::newFrame(){
	if(frameIndex != -1){
		ASSERT(categoryIndex == numUsedCategories, "New frame was called before all of the categories was filled for the current frame.");
	}
	frameIndex++;
	categoryIndex = 0;
}

bool Profiler::wrapped() const{
	return frameIndex >= MAX_FRAME_SAMPLES && frameIndex != -1;
}

void Profiler::writeData() const{
	outStream.open(fileName, std::ios::trunc);

	for(unsigned int i = 0; i < numUsedCategories; i++){
		outStream << categories[i].name;
		if(((i + 1) < numUsedCategories)){
			outStream << ',';
		}
		else{
			outStream << '\n';
		}
	}

	unsigned int endIndex;
	unsigned int startIndex; 
	if(wrapped()){
		endIndex = frameIndex % MAX_FRAME_SAMPLES;
		startIndex = (endIndex + 1) % MAX_FRAME_SAMPLES; 
		while(startIndex != endIndex){
			writeFrame(startIndex);
			startIndex = (startIndex + 1) % MAX_FRAME_SAMPLES;
		}
		if(currentFrameComplete()){
			writeFrame(startIndex);
		}
	}
	else{
		unsigned int numActualFrames = frameIndex;
		if(currentFrameComplete()){
			numActualFrames++;
		}
		startIndex = 0;
		endIndex = numActualFrames;
		while(startIndex < endIndex){
			writeFrame(startIndex);
			startIndex++;
		}
	}
	outStream.close();
}

void Profiler::writeFrame(unsigned int frameNumber) const{
		for(unsigned int cat = 0; cat < numUsedCategories; cat++){
			outStream << categories[cat].samples[frameNumber];
			if(((cat + 1) < numUsedCategories)){
				outStream << ',';
			}
			else{
				outStream << '\n';
			}
		}
}

bool Profiler::currentFrameComplete() const{
	return categoryIndex == numUsedCategories;
}
#endif