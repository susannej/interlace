#include "ImageWidget.h"
#include "RatingWidget.h"

ImageWidget::ImageWidget() {
	//setStyleSheet("background-color: #444");
	widgetLayout = new QGridLayout();
	setLayout(widgetLayout);

	widgetLayout->setRowStretch(1, 100);
	widgetLayout->setColumnStretch(0, 100);

	ratingLabel = new QLabel();
	widgetLayout->addWidget(ratingLabel, 0, 0, Qt::AlignCenter);
	//ratingWidget = new RatingWidget();
	//widgetLayout->addWidget(ratingWidget, 0, 0, Qt::AlignCenter);
	imageLabel = new QLabel();
	widgetLayout->addWidget(imageLabel, 1, 0, Qt::AlignCenter);
	nameLabel = new QLabel();
	widgetLayout->addWidget(nameLabel, 2, 0, Qt::AlignCenter);

}

void ImageWidget::setImage(QImage image) {
	imageLabel->setPixmap(QPixmap::fromImage(image));
}

void ImageWidget::setName(QString name) {
	nameLabel->setText(name);
}

void ImageWidget::setAbsoluteName(QString absoluteName) {
	imageAbsoluteName = absoluteName;
}

void ImageWidget::setRating(int rating) {
/* */
	QPixmap ratingPixmap(100, 20);
	ratingPixmap.fill((new QColor("#323232"))->rgb()); //Qt::white);
	QPen pen1;// = new QPen;
	pen1.setColor(Qt::white);
	pen1.setWidth(6);
	QPen pen2;
	pen2.setColor(Qt::white);
	pen2.setWidth(1);
	QPainter p(&ratingPixmap);
	p.setPen(pen1);
	p.drawArc(7, 8, 6, 6, 0, 360 * 16);
	p.drawArc(27, 8, 6, 6, 0, 360 * 16);
	p.drawArc(48, 8, 6, 6, 0, 360 * 16);
	p.setPen(pen2);
	p.drawArc(65, 6, 10, 10, 0, 360 * 16);
	p.drawArc(85, 6, 10, 10, 0, 360 * 16);
	ratingLabel->setPixmap(ratingPixmap);
/* */
}

void ImageWidget::setSize(int width, int height) {
	setFixedSize(width, height);
}

void ImageWidget::mouseDoubleClickEvent(QMouseEvent *event) {
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

	printf("Mouse DoubleClicked\n");

	system(("wine /home/joe/.wine/drive_c/windows/command/start.exe /Unix /home/joe/.wine/drive_c/Programme/PictureCode/PhotoNinja/PhotoNinja32.exe z:\"" + imageAbsoluteName + "\"").toAscii().data());
	QApplication::restoreOverrideCursor();
}
