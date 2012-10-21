#include <QtGui>

class Interlace : public QMainWindow
{
	Q_OBJECT

	public:
		Interlace();

	//private slots:
	//	void quit();

	private:
		QWidget *dirtree;
		QWidget *browser;

		QAction *exitAction;

		QMenu *fileMenu;
};
