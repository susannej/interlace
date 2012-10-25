#include "ImageBrowserView.h"

#include "ImageBrowserModel.h"

#define DIA_SIZE 200

ImageBrowserView::ImageBrowserView() {

	rows = 0;
	columns = 0;

	gridLayout = new QGridLayout();
	setLayout(gridLayout);

	model = new ImageBrowserModel(this);
}

void ImageBrowserView::dirSelected(QString directoryName) {
printf("dirSelected \n");
	removeWidgets();
	cleanupWidgets();
printf("VectorSize = %d\n", vector.size());
	model->dirSelected(directoryName);
	createWidgets();
printf("VectorSize = %d\n", vector.size());
	updateView();
		
}

void ImageBrowserView::removeWidgets() {
printf("removeWidgetsIn \n");

	for (int i = 0; i < vector.size(); i++) {
		gridLayout->removeWidget(vector[i]);
	}

printf("removeWidgetsOut \n");
}

void ImageBrowserView::cleanupWidgets() {
printf("cleanupWidgets \n");
	int count = vector.size();
	for (int i = 0; i < count; i++) {
		int last = vector.size() -1;
printf("lösche %d aus vector\n", last);
		delete vector[last];
		vector.remove(last);
	}
		
}

void ImageBrowserView::createWidgets() {
printf("createWidgets \n");
printf("Vectorsize = %d\n", vector.size());
	for(int i = 0; i < model->getNoOfFiles(); i++) {
		QWidget *w = createImage();
		vector.append(w);
	}
}

void ImageBrowserView::updateView() {
printf("updateView \n");
	removeWidgets();
	printf ("Hoehe = %d\nBreite = %d\n", height(), width());

	if (vector.size() > 0) {
		int saWidth = ((QScrollArea*) parent())->width();
		columns = saWidth / DIA_SIZE;
printf("columns = %d\n", columns);
		int bla = model->getNoOfFiles();
printf("nooffiles = %d\n", bla);
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

QWidget* ImageBrowserView::createImage() {
printf("createImage \n");
	QWidget *ret = new QWidget();
	QLabel *label = new QLabel;
	QImage image(":images/Dickie.jpg");
	label->setPixmap(QPixmap::fromImage(image));
	QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight);
	layout->addWidget(label);
	ret->setLayout(layout);
	ret->resize(DIA_SIZE, DIA_SIZE);
	return ret;
}
