#include "ImageBrowserView.h"

#include "ImageBrowserModel.h"

ImageBrowserView::ImageBrowserView(QWidget *parent) {
	this->parent = parent;

	model = new ImageBrowserModel(this);
}

void ImageBrowserView::dirSelected(QString directoryName) {
	model->dirSelected(directoryName);
}
