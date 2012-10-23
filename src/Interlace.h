#ifndef INTERLACE_H

#define INTERLACE_H

#include <QtGui>

class MainView;

class Interlace : public QMainWindow
{
	Q_OBJECT

	public:
		Interlace();

	private slots:
	//	void quit();
		void preferences();

	private:
		MainView *mainview;

		QAction *exitAction;
		QAction *preferencesAction;

		QMenu *fileMenu;
		QMenu *editMenu;
};

#endif
