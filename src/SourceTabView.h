#ifndef SOURCETABVIEW_H

#define SOURCETABVIEW_H 1

#include <QtGui>

#include "MainView.h"

class SourceTabView : public QTabWidget
{
	Q_OBJECT

	public:
		SourceTabView(MainView *parent);

	private slots:
		void dirtreeItemClicked(const QModelIndex &);
	//	void preferences();

	private:
		MainView *parent;

		QFileSystemModel *dirmodel;
		QTreeView *dirtree;

		QWidget *databasetree;

	/*
	 * QTreeView hat ein Signal void activated(const QModelIndex & index)
	 */

};

#endif