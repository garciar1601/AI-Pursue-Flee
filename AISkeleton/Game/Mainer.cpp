#include <qtgui\qapplication.h>
#include <MeGame.h>

void main(int argumentCount, char* arguments[]){
	QApplication app(argumentCount, arguments);

	MeGame meGame;

	if(! meGame.initialize()){
		return;
	}

	meGame.runGameLoop();

	app.exec();

	meGame.shutdown();
}