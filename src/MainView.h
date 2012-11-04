#ifndef MAINVIEW_H

#define MAINVIEW_H

#include <QtGui>

class SourceTabView;
class ImageBrowserScrollArea;
class ImageBrowserView;

class MainView : public QSplitter
{
	Q_OBJECT

	public:
		MainView(QWidget *parent);
		void dirSelected(QString directoryName);

	private:
		QWidget *parent;
		QWidget *leftView;
		SourceTabView *tabview;
		QProgressBar *dirProgress;
		ImageBrowserScrollArea *scrollArea;
		ImageBrowserView *browser;
};

#endif
