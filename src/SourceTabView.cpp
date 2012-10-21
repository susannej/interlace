#include "SourceTabView.h"

SourceTabView::SourceTabView(QWidget *parent) {
	this->parent = parent;

	dirtree = new QWidget;
	databasetree = new QWidget;

	addTab(dirtree, tr("Filesystem"));
	addTab(databasetree, tr("Database"));
}

