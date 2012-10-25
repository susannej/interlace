#ifndef IMAGEBROWSERMODEL_H

#define IMAGEBROWSERMODEL_H

#include <QtGui>

class ImageBrowserView;

class ImageBrowserModel
{
	//Q_OBJECT

	public:
		ImageBrowserModel(ImageBrowserView *view);
		
		void dirSelected(QString directoryName);
		int readdir();
		int getNoOfFiles();
		QString getFileName(int i);

	private:
		ImageBrowserView *view;

		QDir *currentDirectory;
		QStringList files;
};

#endif
