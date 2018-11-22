#ifndef SOURCETABVIEW_H

#define SOURCETABVIEW_H

#include <QtWidgets>

class MainView;

class SourceTabView : public QTabWidget
{
	Q_OBJECT

	public:
		SourceTabView(MainView *parent);

	private slots:
		void dirtreeItemClicked(const QModelIndex &index);

	private:
		MainView *parent;

		QFileSystemModel *dirmodel;
		QTreeView *dirtree;

		QWidget *databasetree;

};

#endif
