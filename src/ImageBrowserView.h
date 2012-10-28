#ifndef IMAGEBROWSERVIEW_H

#define IMAGEBROWSERVIEW_H

#include <QtGui>

class ImageBrowserModel;

class ImageBrowserView : public QWidget
{
	Q_OBJECT

	public:
		ImageBrowserView();
		
		void dirSelected(QString directoryName);
		void resizeUpdate();
		void removeWidgets();
		void cleanupWidgets();
		void createWidgets();
		QWidget* createImage(int i);

	private:
		ImageBrowserModel *model;
		int rows, columns;
		QGridLayout *gridLayout;
		QVector<QWidget*> vector;

		void updateView();
};

#endif
