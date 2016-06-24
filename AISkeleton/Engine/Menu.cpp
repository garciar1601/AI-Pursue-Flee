#include "Menu.h"
#include <QtGui\qtabwidget.h>
#include <Qt\qlabel.h>
#include <QtGui\qvboxlayout>
#include <QtGui\qhboxlayout>
#include <Qt\qstring.h>
#include <Qt\qdebug.h>
#include <Qt\qcheckbox.h>

Menu* Menu::instance = 0;
bool Menu::initialize(QVBoxLayout* layout){
	if(instance != 0){
		return false;
	}
	instance = new Menu;
	instance->numButtonInfos = 0;
	instance->theLayout = layout;
	instance->setLayout(layout);
	return true;
}

void Menu::display(){
	instance->setLayout(theLayout);
	instance->show();
}

void Menu::vanish(){
	instance->hide();
}

void Menu::label(const char* text){
	QHBoxLayout* thisRow;
	theLayout->addLayout(thisRow = new QHBoxLayout);

	thisRow->addWidget(new QLabel(text));
}

void Menu::button(const char* text, fastdelegate::FastDelegate0<> callback)
{
	QHBoxLayout* thisRow;
	theLayout->addLayout(thisRow = new QHBoxLayout);


	ButtonInfo& buttonInfo = buttonInfos[numButtonInfos++];
	theLayout->addLayout(thisRow = new QHBoxLayout);
	thisRow->addWidget(buttonInfo.button = new QPushButton(text));
	QObject::connect(buttonInfo.button, SIGNAL(clicked()), &buttonInfo, SLOT(buttonClicked()));
	buttonInfo.delegate = callback;
	thisRow->addStretch();
}
