#ifndef INTERLACE_H

#define INTERLACE_H

#include <QtWidgets>

class MainView;

class Interlace : public QMainWindow
{
	Q_OBJECT

	public:
		Interlace();

	private slots:
	//	void quit();
		void preferences();
		void about();

	private:
		MainView *mainview;

		QAction *exitAction;
		QAction *preferencesAction;
		QAction *aboutAction;

		QMenu *fileMenu;
		QMenu *editMenu;
		QMenu *infoMenu;
};

#endif
