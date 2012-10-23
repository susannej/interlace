#include "MainView.h"

#include "SourceTabView.h"
#include "ImageBrowserView.h"

MainView::MainView(QWidget *parent) {
	this->parent = parent;

	tabview = new SourceTabView(this);
	browser = new ImageBrowserView(this);

	addWidget(tabview);
	addWidget(browser);
}

void MainView::dirSelected(QString directoryName) {
	browser->dirSelected(directoryName);
}
