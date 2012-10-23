#include "SourceTabView.h"

#include "MainView.h"

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
}

void SourceTabView::dirtreeItemClicked(const QModelIndex &index) {
	parent->dirSelected(dirmodel->filePath(index));
}
