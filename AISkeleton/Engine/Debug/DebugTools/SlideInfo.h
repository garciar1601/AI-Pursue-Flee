#ifndef SLIDER_INFO_H
#define SLIDER_INFO_H
#include <Debug\DebugTools\DebugSlider.h>

class SliderInfo{
	friend class DebugMenu;
	DebugSlider* slider;
	float* value;
};

#endif