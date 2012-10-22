#include "SourceTabView.h"

SourceTabView::SourceTabView(QWidget *parent) {
	this->parent = parent;

	dirmodel = new QFileSystemModel;
	dirmodel->setRootPath("");
	dirmodel->setFilter(QDir::AllDirs | QDir::Drives | QDir::Dirs | QDir::NoDotAndDotDot);
	dirmodel->removeColumn(1);
	dirmodel->removeColumn(2);
	dirmodel->removeColumn(3);

	dirtree = new QTreeView;
	dirtree->setModel(dirmodel);
	dirtree->setSortingEnabled(true);
	dirtree->hideColumn(1);
	dirtree->hideColumn(2);
	dirtree->hideColumn(3);
	dirtree->sortByColumn(0, Qt::AscendingOrder);

	connect(dirtree, SIGNAL(clicked(const QModelIndex &)), this, SLOT(dirtreeItemActivated(const QModelIndex &)));

	databasetree = new QWidget;

	addTab(dirtree, tr("Filesystem"));
	addTab(databasetree, tr("Database"));

	/*
	 * QTreeView hat ein Signal void activated(const QModelIndex & index)
	 */
}

void SourceTabView::dirtreeItemActivated(const QModelIndex &index) {
	QString qstring = dirmodel->fileName(index);
	QByteArray barray = qstring.toAscii();
	char* s = barray.data();
//	(((QString) (dirmodel->fileName(index))->toAscii())->data();
	printf("dirtree activated %s\n", s);
}
