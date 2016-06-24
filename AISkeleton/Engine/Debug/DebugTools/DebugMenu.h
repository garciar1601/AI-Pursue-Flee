#ifndef DEBUG_MENU_H
#define DEBUG_MENU_H
#include <Debug\DebugTools\WatchInfo.h>
#include <Debug\DebugTools\SlideInfo.h>
#include <Debug\DebugTools\ToggleInfo.h>
#include <Debug\DebugTools\ButtonInfo.h>
#include <Qt\qlist.h>
#include <Qt\qwidget.h>
#include <ExportHeader.h>
class QVBoxLayout;
class DebugMenu : public QWidget
{
	QVBoxLayout* theLayout;
	QList<WatchInfo> watchInfos;
	QList<SliderInfo> sliderInfos;
	QList<ToggleInfo> toggleInfos;
	static const int MAX_BUTTON_INFOS = 50;
	ButtonInfo buttonInfos[MAX_BUTTON_INFOS];
	uint numButtonInfos;

	static DebugMenu* instance;
	DebugMenu() {}
	DebugMenu(const DebugMenu&);
	DebugMenu& operator=(const DebugMenu&);
public:
	ENGINE_SHARED static DebugMenu& getInstance() { return *instance; }
	ENGINE_SHARED void watchFloat(const char* text, const float& value);
	ENGINE_SHARED void slideFloat(const char* text, float& value, float min = -10.0f, float max = 10.0f, float granularity = 40.0f);
	ENGINE_SHARED void toggleBool(const char* text, bool& boolean);
	ENGINE_SHARED void button(const char* text, fastdelegate::FastDelegate0<> callback);
	ENGINE_SHARED bool initialize(QVBoxLayout* layout);
	ENGINE_SHARED void update();
	ENGINE_SHARED void display();
};

#define debugMenu DebugMenu::getInstance()

#endif