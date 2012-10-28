#ifndef RATINGWIDGET_H

#define RATINGWIDGET_H

#include <QtGui>

class RatingWidget : public QWidget
{
	Q_OBJECT

	public:
		RatingWidget();
		
		void mouseMoveEvent(QMouseEvent *event);
		void mousePressEvent(QMouseEvent *event);

};

#endif
