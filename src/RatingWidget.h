#ifndef RATINGWIDGET_H

#define RATINGWIDGET_H

#include <QtWidgets>

class RatingWidget : public QWidget
{
	Q_OBJECT

	public:
		RatingWidget();

		void setRating(int i);
		int getRating();
		
		//void mouseMoveEvent(QMouseEvent *event);
		void mousePressEvent(QMouseEvent *event);
		void paintEvent(QPaintEvent *event);

	private:
		int rating;
		QImage image;
		QImage starOn;
		QImage starOff;

};

#endif
