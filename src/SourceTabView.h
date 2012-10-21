#include <QtGui>

class SourceTabView : public QTabWidget
{
	Q_OBJECT

	public:
		SourceTabView(QWidget *parent);

	//private slots:
	//	void quit();
	//	void preferences();

	private:
		QWidget *parent;
		QWidget *dirtree;
		QWidget *databasetree;
};
