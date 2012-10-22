#include <QtGui>

class SourceTabView : public QTabWidget
{
	Q_OBJECT

	public:
		SourceTabView(QWidget *parent);

	private slots:
		void dirtreeItemActivated(const QModelIndex &);
	//	void preferences();

	private:
		QWidget *parent;

		QFileSystemModel *dirmodel;
		QTreeView *dirtree;

		QWidget *databasetree;

	/*
	 * QTreeView hat ein Signal void activated(const QModelIndex & index)
	 */

};
