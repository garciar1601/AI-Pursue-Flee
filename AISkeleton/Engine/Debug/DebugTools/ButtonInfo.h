#pragma once
#include <QtGui\qwidget>
#include "Qt\qpushbutton.h"
#include "FastDelegate.h"

struct ButtonInfo : public QObject
{
	Q_OBJECT
public:
	QPushButton* button;
	fastdelegate::FastDelegate0<> delegate;
	private slots:
	void buttonClicked();
};
