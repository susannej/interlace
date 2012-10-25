#include "ImageBrowserModel.h"

#include "ImageBrowserView.h"

ImageBrowserModel::ImageBrowserModel(ImageBrowserView *view) {
	this->view = view;
}

void ImageBrowserModel::dirSelected(QString directoryName) {
	currentDirectory = new QDir(directoryName);

	QByteArray barray = directoryName.toAscii();
	char* s = barray.data();
	printf("dirtree activated %s\n", s);

	readdir();
}

int ImageBrowserModel::readdir() {
	QStringList fileExt;
	fileExt << "*.nef" << "*.jpg" << "*.jpeg";
	files = currentDirectory->entryList(fileExt, QDir::Files | QDir::NoDotAndDotDot, QDir::Name);

printf("Directory enhaelt %d images\n", files.size());

	return files.size();
}

int ImageBrowserModel::getNoOfFiles() {
	return files.size();
}

QString ImageBrowserModel::getFileName(int i) {
	return files.at(i);
}
