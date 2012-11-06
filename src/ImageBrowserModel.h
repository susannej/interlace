#ifndef IMAGEBROWSERMODEL_H

#define IMAGEBROWSERMODEL_H

#include <QtGui>

class ImageBrowserView;

class ImageBrowserModel
{
	//Q_OBJECT
	
	enum Rotation {LEFT, RIGHT};

	public:
		ImageBrowserModel(ImageBrowserView *view);
		
		void dirSelected(QString directoryName);
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
};

#endif
