#include "SourceTabView.h"

#include "MainView.h"

#include <iostream>

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
	//connect(dirmodel, SIGNAL(directoryLoaded(QString)), this, SLOT(dirmodelDirectoryLoaded(QString)));

	databasetree = new QWidget;

	addTab(dirtree, tr("Filesystem"));
	addTab(databasetree, tr("Database"));
}

void SourceTabView::dirtreeItemClicked(const QModelIndex &index) {
	parent->dirSelected(dirmodel->filePath(index));
}

void SourceTabView::dirmodelDirectoryLoaded(const QString &path) {
	//std::cout << path.toLatin1();
	/*
	if (path == "/home/susanne/Bilder") {
		//startPathIdx = dirmodel->setRootPath("/home/susanne/Bilder");
		dirtree->setCurrentIndex(startPathIdx);
	}
	*/
}

void SourceTabView::setStartDirectory(QString dir) {
	QModelIndex rootIndex = dirmodel->index(QDir::cleanPath(dir));
        if (rootIndex.isValid())
            dirtree->setCurrentIndex(rootIndex);
}
