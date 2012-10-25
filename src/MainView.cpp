#include "MainView.h"

#include "SourceTabView.h"
#include "ImageBrowserScrollArea.h"
#include "ImageBrowserView.h"

MainView::MainView(QWidget *parent) {
	this->parent = parent;

	tabview = new SourceTabView(this);
	ImageBrowserScrollArea *scrollArea = new ImageBrowserScrollArea; //(this);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	browser = new ImageBrowserView();
	scrollArea->setWidget(browser);

	addWidget(tabview);
	addWidget(scrollArea);
}

void MainView::dirSelected(QString directoryName) {
	browser->dirSelected(directoryName);
}
