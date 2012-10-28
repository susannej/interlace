#include "RatingWidget.h"

RatingWidget::RatingWidget() {
	setMouseTracking(true);
	setFixedSize(100, 20);
}

void RatingWidget::mouseMoveEvent(QMouseEvent *event) {
	QPoint point = event->pos();
	printf("Mouse moved to x = %d and y = %d\n", point.x(), point.y());
}

void RatingWidget::mousePressEvent(QMouseEvent *event) {
	QPoint point = event->pos();
	printf("==> Mouse pressed at x = %d and y = %d!!!!!\n", point.x(), point.y());
}
