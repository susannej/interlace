#ifndef IMAGEBROWSERVIEW_H

#define IMAGEBROWSERVIEW_H 1

#include <QtGui>

class ImageBrowserView : public QWidget
{
	Q_OBJECT

	public:
		ImageBrowserView(QWidget *parent);
		
		void dirSelected(QString directoryName);

	//private slots:
	//	void quit();
	//	void preferences();

	private:
		QWidget *parent;
};

#endif