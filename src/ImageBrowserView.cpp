#include "ImageBrowserView.h"

#include "ImageBrowserModel.h"
#include "ImageWidget.h"

#define DIA_SIZE 200

ImageBrowserView::ImageBrowserView() {
	rows = 0;
	columns = 0;

	gridLayout = new QGridLayout();
	setLayout(gridLayout);

	model = new ImageBrowserModel(this);
}

void ImageBrowserView::dirSelected(QString directoryName) {
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	removeWidgets();
	cleanupWidgets();
	model->dirSelected(directoryName);
	createWidgets();
	updateView();
	QApplication::restoreOverrideCursor();
}

void ImageBrowserView::resizeUpdate() {
	removeWidgets();
	updateView();
}

void ImageBrowserView::removeWidgets() {
	for (int i = 0; i < vector.size(); i++) {
		gridLayout->removeWidget(vector[i]);
	}
}

void ImageBrowserView::cleanupWidgets() {
	int count = vector.size();
	for (int i = 0; i < count; i++) {
		int last = vector.size() -1;
		delete vector[last];
		vector.remove(last);
	}
		
}

void ImageBrowserView::createWidgets() {
	for(int i = 0; i < model->getNoOfFiles(); i++) {
		QWidget *w = createImage(i);
		vector.append(w);
	}
}

void ImageBrowserView::updateView() {
	if (vector.size() > 0) {
		int saWidth = ((QScrollArea*) parent())->width();
		columns = saWidth / DIA_SIZE;
		if (columns == 0) columns = 1;
		rows = model->getNoOfFiles() / columns;
		if ((model->getNoOfFiles() % columns) > 0) {
			rows++;
		}
		setFixedSize(columns * DIA_SIZE, rows * DIA_SIZE);
		for (int i = 0; i < vector.size(); i++) {
			gridLayout->addWidget(vector[i], (int) i / columns, (int) i % columns);
		}
	}
}

QWidget* ImageBrowserView::createImage(int i) {
	ImageWidget *imageWidget = new ImageWidget;

	imageWidget->setAbsoluteName(model->getAbsoluteFileName(i));
	imageWidget->setRating(model->getRating(i));
	imageWidget->setImage(model->getImage(i, DIA_SIZE - 50));
	imageWidget->setName(model->getFileName(i));
	imageWidget->setSize(DIA_SIZE, DIA_SIZE);

	return imageWidget;
}

