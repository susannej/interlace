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
	midView = new QWidget;
	QGridLayout *mVl = new QGridLayout;
	midView->setLayout(mVl);

	QToolButton *refresh = new QToolButton;
	refresh->setIcon(QIcon(":/images/refresh.png"));
	QToolBar *topBar = new QToolBar;
	topBar->addWidget(refresh);
	
	ImageBrowserScrollArea *scrollArea = new ImageBrowserScrollArea;
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	browser = new ImageBrowserView();
	scrollArea->setWidget(browser);

	QToolButton *turnLeft = new QToolButton;
	turnLeft->setIcon(QIcon(":/images/rotate_left.png"));
	QToolButton *turnRight = new QToolButton;
	turnRight->setIcon(QIcon(":/images/rotate_right.png"));
	QToolButton *ctrl = new QToolButton;
	ctrl->setIcon(QIcon(":/images/key_ctrl.png"));
	ctrl->setCheckable(true);
	QToolBar *bottomBar = new QToolBar;
	bottomBar->addWidget(turnLeft);
	bottomBar->addWidget(turnRight);
	bottomBar->addSeparator();
	bottomBar->addWidget(ctrl);

	mVl->addWidget(topBar, 0, 0);
	mVl->addWidget(scrollArea, 1, 0);
	mVl->addWidget(bottomBar, 2, 0);

	connect(browser, SIGNAL(progressValueChanged(int)), dirProgress, SLOT(setValue(int)));
	connect(refresh, SIGNAL(clicked()), browser, SLOT(dirUpdate()));
	connect(ctrl, SIGNAL(clicked(bool)), browser, SLOT(toggleSelectionMode(bool)));
	connect(turnLeft, SIGNAL(clicked()), browser, SLOT(rotateSelectionLeft()));
	connect(turnRight, SIGNAL(clicked()), browser, SLOT(rotateSelectionRight()));

	addWidget(leftView);
	addWidget(midView);
}

void MainView::dirSelected(QString directoryName) {
	browser->dirSelected(directoryName);
}


