#include "ImageBrowserScrollArea.h"

#include "MainView.h"
#include "ImageBrowserView.h"

void ImageBrowserScrollArea::resizeEvent(QResizeEvent *event) {
	QWidget *child = widget();
	if (child != 0) {
		((ImageBrowserView*) child)->resizeUpdate();
	}
}
