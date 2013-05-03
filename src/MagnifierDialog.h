#ifndef MAGNIFIERDIALOG_H

#define MAGNIFIERDIALOG_H

#include <QtGui>

class InterlaceConfig;
class ImageBrowserView;
class ImageBrowserModel;
class RatingWidget;

class MagnifierDialog : public QDialog
{
	Q_OBJECT

	public:
		MagnifierDialog(QWidget *parent = 0);
		~MagnifierDialog();

		void setImages(QVector<int> *images);
		void setModel(ImageBrowserModel *model);
		void setStartImage(int no);


	protected:
		virtual void keyPressEvent(QKeyEvent *event);
		virtual void resizeEvent(QResizeEvent *event);
		virtual void mousePressEvent(QMouseEvent *event);
		virtual void wheelEvent (QWheelEvent * event);
		
	private:
		InterlaceConfig *conf;
		ImageBrowserModel *m_model;
		QVector<int> *imageNos;
		int currentImage;
		QString m_label;
		int imageSize;

		QGridLayout *widgetLayout;
		RatingWidget *ratingWidget;
		QWidget *colorLabel;
		QLabel *imageLabel;
		QLabel *nameLabel;

		void setImage();
		void updateRating(int rating);
};

#endif
