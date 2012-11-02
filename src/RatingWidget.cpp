#include "RatingWidget.h"
#include "ImageWidget.h"

RatingWidget::RatingWidget() {
	//setMouseTracking(true);
	setFixedSize(120, 20);
	image = QImage(":/images/ratingImage.png");
	starOn = QImage(":/images/star_on.png");
	starOff = QImage(":/images/star_off.png");
	setRating(0);
}

void RatingWidget::setRating(int i) {
	rating = i;
	if (rating < 0) rating = 0;
	update();
}

int RatingWidget::getRating() {
	return rating;
}

/*
void RatingWidget::mouseMoveEvent(QMouseEvent *event) {
	QPoint point = event->pos();
	printf("Mouse moved to x = %d and y = %d\n", point.x(), point.y());
}
*/

void RatingWidget::mousePressEvent(QMouseEvent *event) {
	QPoint point = event->pos();
	if (point.x() < 20) {
		setRating(0);
	} else if (point.x() < 40) {
		setRating(1);
	} else if (point.x() < 60) {
		setRating(2);
	} else if (point.x() < 80) {
		setRating(3);
	} else if (point.x() < 100) {
		setRating(4);
	} else if (point.x() < 120) {
		setRating(5);
	}
printf("vor imageWidget updateRating\n");
	((ImageWidget*) parent())->updateRating(getRating());
}

void RatingWidget::paintEvent(QPaintEvent *event)
{
	QPainter imagePainter(&image);
	QPainter widgetPainter(this);

	for (int i = 0; i < 5; i++) {
		if (i < rating) {
			QRect target((i +1) * 20 +2, 2, 16, 16);
			QRect source(0, 0, 16, 16);
			imagePainter.drawImage(target, starOn, source);
		} else {
			QRect target((i +1) * 20 +2, 2, 16, 16);
			QRect source(0, 0, 16, 16);
			imagePainter.drawImage(target, starOff, source);
		}
	}

	QRect dirtyRect = event->rect();
	widgetPainter.drawImage(dirtyRect, image, dirtyRect);
}
