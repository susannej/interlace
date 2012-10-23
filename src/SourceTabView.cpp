#include "SourceTabView.h"

SourceTabView::SourceTabView(MainView *parent) {
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

	connect(dirtree, SIGNAL(clicked(const QModelIndex &)), this, SLOT(dirtreeItemClicked(const QModelIndex &)));

	databasetree = new QWidget;

	addTab(dirtree, tr("Filesystem"));
	addTab(databasetree, tr("Database"));

	/*
	 * QTreeView hat ein Signal void activated(const QModelIndex & index)
	 */
}

void SourceTabView::dirtreeItemClicked(const QModelIndex &index) {
	//parent->dirSelected(dirmodel->fileName(index);
	QString qstring = dirmodel->fileName(index);
	QByteArray barray = qstring.toAscii();
	char* s = barray.data();
//	(((QString) (dirmodel->fileName(index))->toAscii())->data();
	printf("dirtree activated %s\n", s);

		QString qstring2 = dirmodel->filePath(index);
	QByteArray barray2 = qstring2.toAscii();
	char* s2 = barray2.data();
//	(((QString) (dirmodel->fileName(index))->toAscii())->data();
	printf("dirtree activated %s\n", s2);
}
