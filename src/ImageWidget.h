#ifndef IMAGEWIDGET_H

#define IMAGEWIDGET_H

#include <QtGui>

class ImageBrowserView;
class RatingWidget;

class ImageWidget : public QWidget
{
	Q_OBJECT

	public:
		ImageWidget();
		
		void setImage(QImage image);
		void setName(QString name);
		void setAbsoluteName(QString absoluteName);
		void setRating(int rating);
		void setSize(int width, int height);

		void mouseDoubleClickEvent(QMouseEvent *event);

	private:
		QString imageAbsoluteName;
		QGridLayout *widgetLayout;

		RatingWidget *ratingWidget;
		QLabel *imageLabel;
		QLabel *nameLabel;
};

#endif
