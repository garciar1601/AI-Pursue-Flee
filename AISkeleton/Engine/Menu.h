#ifndef MENU_H
#define MENU_H
#include <Debug\DebugTools\ButtonInfo.h>
#include <Qt\qlist.h>
#include <Qt\qwidget.h>
#include <ExportHeader.h>
class QVBoxLayout;
class Menu : public QWidget
{
	QVBoxLayout* theLayout;
	static const int MAX_BUTTON_INFOS = 50;
	ButtonInfo buttonInfos[MAX_BUTTON_INFOS];
	uint numButtonInfos;

	static Menu* instance;
	Menu() {}
	Menu(const Menu&);
	Menu& operator=(const Menu&);
public:
	ENGINE_SHARED static Menu& getInstance() { return *instance; }
	ENGINE_SHARED void button(const char* text, fastdelegate::FastDelegate0<> callback);
	ENGINE_SHARED void label(const char* text);
	ENGINE_SHARED bool initialize(QVBoxLayout* layout);
	ENGINE_SHARED void update();
	ENGINE_SHARED void display();
	ENGINE_SHARED void vanish();
};

#define menu Menu::getInstance()

#endif