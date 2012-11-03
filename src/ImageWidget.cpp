#include "ImageWidget.h"
#include "RatingWidget.h"
#include "ImageBrowserView.h"

ImageWidget::ImageWidget() {
	//setStyleSheet("background-color: #444");
	widgetLayout = new QGridLayout();
	setLayout(widgetLayout);

	widgetLayout->setRowStretch(1, 100);
	widgetLayout->setColumnStretch(0, 100);

	ratingWidget = new RatingWidget();
	widgetLayout->addWidget(ratingWidget, 0, 0, Qt::AlignCenter);
	imageLabel = new QLabel();
	widgetLayout->addWidget(imageLabel, 1, 0, Qt::AlignCenter);
	nameLabel = new QLabel();
	widgetLayout->addWidget(nameLabel, 2, 0, Qt::AlignCenter);

}

void ImageWidget::setImage(QImage image) {
	imageLabel->setPixmap(QPixmap::fromImage(image));
}

void ImageWidget::setName(QString name) {
	imageName = name;
	nameLabel->setText(name);
}

void ImageWidget::setAbsoluteName(QString absoluteName) {
	imageAbsoluteName = absoluteName;
}

void ImageWidget::setRating(int rating) {
	ratingWidget->setRating(rating);
}

void ImageWidget::updateRating(int rating) {
	((ImageBrowserView*) parent())->updateRating(imageName, rating);
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
