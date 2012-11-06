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
		~ImageWidget();
		
		void setImage(QImage image);
		void setName(QString name);
		void setAbsoluteName(QString absoluteName);
		void setRating(int rating);
		void setSize(int width, int height);
		void updateRating(int rating);
		void setSelected(bool sel);
		bool isSelected();

		void mousePressEvent(QMouseEvent *event);
		void mouseDoubleClickEvent(QMouseEvent *event);
		void paintEvent(QPaintEvent *event);

	private:
		bool selected;

		QString imageAbsoluteName;
		QString imageName;
		QGridLayout *widgetLayout;

		RatingWidget *ratingWidget;
		QLabel *imageLabel;
		QLabel *nameLabel;
};

#endif
