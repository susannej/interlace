#include "Interlace.h"

int main(int argc, char **argv) {
	QApplication app(argc, argv);

	Interlace interlace;
	interlace.show();

	return app.exec();
}

Interlace::Interlace()
{
	exitAction = new QAction(tr("E&xit"), this);

	connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(exitAction);

	dirtree = new QWidget;
	browser = new QWidget;

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(dirtree);
	layout->addWidget(browser);

	setLayout(layout);

	setWindowTitle(tr("Interlace"));
}
