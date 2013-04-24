#ifndef IMAGEBROWSERMODEL_H

#define IMAGEBROWSERMODEL_H

#include <QtGui>

class ImageBrowserView;

class ImageBrowserModel
{
	//Q_OBJECT
	

	public:
		enum Rotation {LEFT, RIGHT};

		ImageBrowserModel(ImageBrowserView *view);
		
		void dirSelected(QString directoryName);
		void dirUpdate();
		void setStarFilter(int);
		void setColorFilter(QString);
		int readdir();
		int getNoOfFiles();
		QString getFileName(int i);
		QString getAbsoluteFileName(int i);
		QImage getImage(int i, int maxsize);
		int getRating(int i);
		void updateRating(int i, int rating);
		void updateRating(QString name, int rating);
		QString getLabel(int i);
		void updateLabel(int i, QString label);
		void updateLabel(QString name, QString label);
		void rotateImage(int i, Rotation direction);
		void deleteImage(int i);

	private:
		ImageBrowserView *view;

		QDir currentDirectory;
		QDir photoNinjaDirectory;
		QFileInfoList files;
		bool photoNinjaExists;
		int starFilter;
		QString colorFilter;

		int getImageOrientation(int i);
};

#endif
