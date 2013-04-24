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
		void setLabel(QString label);
		void setSize(int width, int height);
		void updateRating(int rating);
		void updateLabel(QString label);
		void setSelected(bool sel);
		bool isSelected();

		void mousePressEvent(QMouseEvent *event);
		void mouseDoubleClickEvent(QMouseEvent *event);
		void paintEvent(QPaintEvent *event);

	protected:
		void contextMenuEvent(QContextMenuEvent *event);

	private slots:
		void execPrg(QAction*);

	private:
		bool selected;
		QString m_label;

		QString imageAbsoluteName;
		QString imageName;
		QGridLayout *widgetLayout;

		RatingWidget *ratingWidget;
		QWidget *colorLabel;
		QLabel *imageLabel;
		QLabel *nameLabel;
};

#endif
