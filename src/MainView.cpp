#include "MainView.h"

MainView::MainView(QWidget *parent) {
	this->parent = parent;

	tabview = new SourceTabView(this);
	browser = new ImageBrowserView(this);

	addWidget(tabview);
	addWidget(browser);
}

