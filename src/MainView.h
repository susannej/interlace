#ifndef MAINVIEW_H

#define MAINVIEW_H

#include <QtWidgets>
#include <exiv2/exiv2.hpp>

class SourceTabView;
class ImageBrowserScrollArea;
class ImageBrowserView;

class MainView : public QSplitter
{
	Q_OBJECT

	public:
		MainView(QWidget *parent = 0);
		void dirSelected(QString directoryName);

	public slots:
		void setExifData(QStringList exifList);
		void exifCellChanged(int row, int column);

	private:
		QWidget *parent;

		QWidget *leftView;
		SourceTabView *tabview;
		QProgressBar *dirProgress;

		QWidget *midView;
		ImageBrowserScrollArea *scrollArea;
		ImageBrowserView *browser;

		QWidget *rightView;
		QTableWidget *infoTable;

		QComboBox *starFilter;
		QComboBox *colorFilter;

		bool observeCellChange = true;
};

#endif
