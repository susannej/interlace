#include <QtGui>

class ImageBrowserView : public QWidget
{
	Q_OBJECT

	public:
		ImageBrowserView(QWidget *parent);

	//private slots:
	//	void quit();
	//	void preferences();

	private:
		QWidget *parent;
};
