#ifndef IMAGEBROWSERVIEW_H

#define IMAGEBROWSERVIEW_H

#include <QtWidgets>
#include <exiv2/exiv2.hpp>

#include "ImageBrowserModel.h"
#include "ToolFunctions.h"

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
		void updateLabel(QString name, QString label);
		void clearSelection();
		bool isCtrlButtonPressed();
		int getNoOfSelectedImages();
		void updateProgress(int value);
		void exifDataUpdate();
		void writeExifData(QString exifKey, QString text);

	public slots:
		void addWidget2View(QWidget *image, int i);
		void dirUpdate();
		void setStarFilter(int);
		void setColorFilter(int);
		void magnifier();
		void rotateSelectionLeft();
		void rotateSelectionRight();
		void deleteSelection();
		void toggleSelectionMode(bool mode);

	signals:
		void progressValueChanged(int newValue);
		void showWidget(QWidget *image, int i);
		void setExifData(QStringList imageList);

	private:
		ImageBrowserModel *model;
		int rows, columns;
		QGridLayout *gridLayout;
		QVector<QWidget*> vector;
		bool ctrlButtonPressed;
        	QFuture<void> readImagesProcess;

		void updateView();
		void rotateSelection(ImageBrowserModel::Rotation direction);
};

#endif
