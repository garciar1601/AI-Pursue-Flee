#ifndef TOGGLE_INFO_H
#define TOGGLE_INFO_H
#include <Qt\qcheckbox.h>

class ToggleInfo{
	friend class DebugMenu;
	QCheckBox* checkBox;
	bool* boolean;
};

#endif