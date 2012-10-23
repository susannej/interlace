#ifndef MAINVIEW_H

#define MAINVIEW_H

#include <QtGui>

class SourceTabView;
class ImageBrowserView;

class MainView : public QSplitter
{
	Q_OBJECT

	public:
		MainView(QWidget *parent);
		void dirSelected(QString directoryName);

	//private slots:
	//	void quit();
	//	void preferences();

	private:
		QWidget *parent;
		SourceTabView *tabview;
		ImageBrowserView *browser;
};

#endif
