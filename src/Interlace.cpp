#include "Interlace.h"

#include "MainView.h"

#include "DarkStyleSheet.h"

int main(int argc, char **argv) {
	QApplication app(argc, argv);
	app.setStyle("plastique");

	Interlace interlace;
	interlace.setStyleSheet(DARK_STYLESHEET);
	interlace.resize(800, 600);
	interlace.show();

	return app.exec();
}

Interlace::Interlace() {
	exitAction = new QAction(tr("E&xit"), this);
	preferencesAction = new QAction(tr("Preferences..."), this);

	connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(preferencesAction, SIGNAL(triggered()), this, SLOT(preferences()));

	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(exitAction);

	editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(preferencesAction);

	mainview = new MainView(this);
	this->setCentralWidget(mainview);

	setWindowTitle(tr("Interlace"));
}

void Interlace::preferences() {
	printf("preferences Action\n");
}
