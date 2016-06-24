#include <Debug\DebugTools\DebugMenu.h>
#include <QtGui\qtabwidget.h>
#include <Qt\qlabel.h>
#include <QtGui\qvboxlayout>
#include <QtGui\qhboxlayout>
#include <Qt\qstring.h>
#include <Qt\qdebug.h>
#include <Qt\qcheckbox.h>
#include <Debug\DebugTools\DebugSlider.h>

DebugMenu* DebugMenu::instance = 0;
bool DebugMenu::initialize(QVBoxLayout* layout){
	if(instance != 0){
		return false;
	}
	instance = new DebugMenu;
	instance->numButtonInfos = 0;
	instance->theLayout = layout;
	instance->setLayout(layout);
	return true;
}

void DebugMenu::display(){
	instance->setLayout(theLayout);
	instance->show();
}

void DebugMenu::watchFloat(const char* text, const float& value){
	QHBoxLayout* thisRow;
	theLayout->addLayout(thisRow = new QHBoxLayout);

	thisRow->addWidget(new QLabel(text));

	WatchInfo w;
	thisRow->addWidget(w.theLabel = new QLabel());
	w.theFloat = &value;
	watchInfos.append(w);
}

void DebugMenu::slideFloat(const char* text, float& value, float min, float max, float granularity){
	QHBoxLayout* thisRow;
	theLayout->addLayout(thisRow = new QHBoxLayout);

	thisRow->addWidget(new QLabel(text));
	
	SliderInfo s;
	thisRow->addWidget(s.slider = new DebugSlider(min, max, false, granularity));
	s.slider->setValue(value);
	s.value = &value;
	sliderInfos.append(s);
}

void DebugMenu::toggleBool(const char* text, bool& boolean){
	QHBoxLayout* thisRow;
	theLayout->addLayout(thisRow = new QHBoxLayout);

	thisRow->addWidget(new QLabel(text));

	ToggleInfo t;
	thisRow->addWidget(t.checkBox = new QCheckBox());
	t.boolean = &boolean;
	t.checkBox->setChecked(*t.boolean);
	toggleInfos.append(t);
}

void DebugMenu::update(){
	for (int i = 0; i < watchInfos.size(); i++){
		WatchInfo& w = watchInfos[i];
		w.theLabel->setText(QString::number(*w.theFloat));
	}
	for (int i = 0; i < sliderInfos.size(); i++){
		SliderInfo& s = sliderInfos[i];
		*s.value = s.slider->value();
	}
	for (int i = 0; i < toggleInfos.size(); i++){
		ToggleInfo& t = toggleInfos[i];
		*t.boolean = t.checkBox->isChecked();
	}
}

void DebugMenu::button(const char* text, fastdelegate::FastDelegate0<> callback)
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