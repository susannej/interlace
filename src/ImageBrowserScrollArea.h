#ifndef IMAGEBROWSERSCROLLAREA_H

#define IMAGEBROWSERSCROLLAREA_H

#include <QtGui>

class ImageBrowserView;
class MainView;

class ImageBrowserScrollArea : public QScrollArea
{
	Q_OBJECT

	protected:
		void resizeEvent(QResizeEvent *event);

};

#endif
