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
		void updateRating(QString name, int rating);
		void clearSelection();
		bool isCtrlButtonPressed();

	public slots:
		void addWidget2View(QWidget *image, int i);
		void dirUpdate();
		void rotateSelectionLeft();
		void rotateSelectionRight();
		void toggleSelectionMode(bool mode);

	signals:
		void progressValueChanged(int newValue);
		void showWidget(QWidget *image, int i);

	private:
		ImageBrowserModel *model;
		int rows, columns;
		QGridLayout *gridLayout;
		QVector<QWidget*> vector;
		bool ctrlButtonPressed;

		void updateView();
};

#endif
