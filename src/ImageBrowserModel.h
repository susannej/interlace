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
		int readdir();
		int getNoOfFiles();
		QString getFileName(int i);
		QString getAbsoluteFileName(int i);
		QImage getImage(int i, int maxsize);
		int getRating(int i);
		void updateRating(int i, int rating);
		void updateRating(QString name, int rating);
		void rotateImage(int i, Rotation direction);

	private:
		ImageBrowserView *view;

		QDir currentDirectory;
		QDir photoNinjaDirectory;
		QFileInfoList files;
		bool photoNinjaExists;

		int getImageOrientation(int i);
};

#endif
