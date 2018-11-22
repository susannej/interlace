#include "Interlace.h"

#include "MainView.h"
#include "InterlaceConfig.h"

#include "DarkStyleSheet.h"

int main(int argc, char **argv) {
	QApplication app(argc, argv);
	//app.setStyle("plastique");
	
	InterlaceConfig *config = InterlaceConfig::getInstance();

	Interlace interlace;

	//interlace.setStyleSheet(DARK_STYLESHEET);
	QFile f(":qdarkstyle/style.qss");
	if (!f.exists())
	{
    		printf("Unable to set stylesheet, file not found\n");
	}
	else
	{
		f.open(QFile::ReadOnly | QFile::Text);
		QTextStream ts(&f);
		interlace.setStyleSheet(ts.readAll());
	}

	//interlace.resize(1000, 800);
	interlace.setWindowState(Qt::WindowMaximized);
	interlace.show();

	return app.exec();
}

Interlace::Interlace() {
	exitAction = new QAction(tr("E&xit"), this);
	preferencesAction = new QAction(tr("Preferences..."), this);
	aboutAction = new QAction(tr("About..."), this);

	connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(preferencesAction, SIGNAL(triggered()), this, SLOT(preferences()));
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(exitAction);

	editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(preferencesAction);

	infoMenu = menuBar()->addMenu(tr("&Info"));
	infoMenu->addAction(aboutAction);

	mainview = new MainView(this);
	this->setCentralWidget(mainview);

	QList<int> sizes = QList<int>();
	sizes.append(280);
	sizes.append(700);
	sizes.append(280);
	mainview->setSizes(sizes);

	setWindowTitle(tr("Interlace"));
}

void Interlace::preferences() {
	printf("preferences Action\n");
}

void Interlace::about() {
	QMessageBox::about(this, tr("About Interlace"),
		tr("<h2>Interlace 0.1</h2>"
		"<p>Copyright &copy; 2014-2019 Susanne Jaeckel</p>"
		"<p>A small application for your photo-management.</p>"
	));
}
