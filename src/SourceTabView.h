#ifndef SOURCETABVIEW_H

#define SOURCETABVIEW_H

#include <QtWidgets>

class MainView;

class SourceTabView : public QTabWidget
{
	Q_OBJECT

	public:
		SourceTabView(MainView *parent);
		void setStartDirectory(QString dir);

	private slots:
		void dirtreeItemClicked(const QModelIndex &index);
		void dirmodelDirectoryLoaded(const QString &path);

	private:
		MainView *parent;

		QFileSystemModel *dirmodel;
		QTreeView *dirtree;
		QModelIndex startPathIdx;

		QWidget *databasetree;

};

#endif
