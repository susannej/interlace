#include "MainView.h"

#include "SourceTabView.h"
#include "ImageBrowserScrollArea.h"
#include "ImageBrowserView.h"

MainView::MainView(QWidget *parent) {
	this->parent = parent;

	// Directory Area
	leftView = new QWidget;
	QGridLayout *lVl = new QGridLayout;
	leftView->setLayout(lVl);

	tabview = new SourceTabView(this);
	dirProgress = new QProgressBar;
	dirProgress->setRange(0, 99);

	lVl->addWidget(tabview, 0, 0);
	lVl->addWidget(dirProgress, 1, 0);

	// Browser Area
	ImageBrowserScrollArea *scrollArea = new ImageBrowserScrollArea;
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	browser = new ImageBrowserView();
	scrollArea->setWidget(browser);

	connect(browser, SIGNAL(progressValueChanged(int)), dirProgress, SLOT(setValue(int)));

	addWidget(leftView);
	addWidget(scrollArea);
}

void MainView::dirSelected(QString directoryName) {
	browser->dirSelected(directoryName);
}


