#include "ImageBrowserModel.h"

#include "ImageBrowserView.h"

ImageBrowserModel::ImageBrowserModel(ImageBrowserView *view) {
	this->view = view;
}

void ImageBrowserModel::dirSelected(QString directoryName) {
	QByteArray barray = directoryName.toAscii();
	char* s = barray.data();
	printf("dirtree activated %s\n", s);
}
