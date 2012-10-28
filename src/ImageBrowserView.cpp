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
	QWidget *w0 = new QWidget();
	//w0->setStyleSheet("background-color: #444");
	QGridLayout *l0 = new QGridLayout();
	w0->setLayout(l0);

	QPixmap pixmap(100, 20);
	pixmap.fill((new QColor("#323232"))->rgb()); //Qt::white);
	QPen pen1;// = new QPen;
	pen1.setColor(Qt::white);
	pen1.setWidth(6);
	QPen pen2;
	pen2.setColor(Qt::white);
	pen2.setWidth(1);
	QPainter p(&pixmap);
	p.setPen(pen1);
	p.drawArc(7, 8, 6, 6, 0, 360 * 16);
	p.drawArc(27, 8, 6, 6, 0, 360 * 16);
	p.drawArc(48, 8, 6, 6, 0, 360 * 16);
	p.setPen(pen2);
	p.drawArc(65, 6, 10, 10, 0, 360 * 16);
	p.drawArc(85, 6, 10, 10, 0, 360 * 16);
	QLabel *rateLabel = new QLabel;
	rateLabel->setPixmap(pixmap);
	l0->addWidget(rateLabel, 0, 0, Qt::AlignCenter);

	QImage image = model->getImage(i, DIA_SIZE - 50);
	QLabel *imageLabel = new QLabel;
	imageLabel->setPixmap(QPixmap::fromImage(image));
	l0->addWidget(imageLabel, 1, 0, Qt::AlignCenter);
	l0->setRowStretch(1, 100);
	l0->setColumnStretch(0, 100);

	QLabel *nameLabel = new QLabel(model->getFileName(i));
	l0->addWidget(nameLabel, 2, 0, Qt::AlignCenter);

	w0->setFixedSize(DIA_SIZE, DIA_SIZE);
	return w0;
}
