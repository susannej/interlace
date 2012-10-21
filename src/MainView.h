#include <QtGui>

#include "SourceTabView.h"
#include "ImageBrowserView.h"

class MainView : public QSplitter
{
	Q_OBJECT

	public:
		MainView(QWidget *parent);

	//private slots:
	//	void quit();
	//	void preferences();

	private:
		QWidget *parent;
		SourceTabView *tabview;
		ImageBrowserView *browser;
};
