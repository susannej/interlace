#include "MainView.h"

#include "SourceTabView.h"
#include "ImageBrowserScrollArea.h"
#include "ImageBrowserView.h"
#include "InterlaceConfig.h"

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

	topBar->addSeparator();

	starFilter = new QComboBox();
	starFilter->addItem(QIcon(":/images/0star.png"), "");
	starFilter->addItem(QIcon(":/images/1star.png"), "");
	starFilter->addItem(QIcon(":/images/2star.png"), "");
	starFilter->addItem(QIcon(":/images/3star.png"), "");
	starFilter->addItem(QIcon(":/images/4star.png"), "");
	starFilter->addItem(QIcon(":/images/5star.png"), "");
	starFilter->setIconSize(QSize(106, 16));
	topBar->addWidget(starFilter);

	topBar->addSeparator();

	colorFilter = new QComboBox();
	InterlaceConfig *conf = InterlaceConfig::getInstance();
	colorFilter->addItem(conf->getIcon4Color("None"), "None label");
	for (int i = 0; i < conf->getConfLabels(); i++) {
		QString label = conf->getLabelDesc(i);
		colorFilter->addItem(conf->getIcon4Color(label), label + " label");
	}
	topBar->addWidget(colorFilter);

	topBar->addSeparator();

	QToolButton *magnifier = new QToolButton;
	magnifier->setIcon(QIcon(":/images/magnifier.png"));
	topBar->addWidget(magnifier);
	
	ImageBrowserScrollArea *scrollArea = new ImageBrowserScrollArea;
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	browser = new ImageBrowserView();
	scrollArea->setWidget(browser);

	QToolButton *turnLeft = new QToolButton;
	turnLeft->setIcon(QIcon(":/images/rotate_left.png"));
	QToolButton *turnRight = new QToolButton;
	turnRight->setIcon(QIcon(":/images/rotate_right.png"));
	QToolButton *deleteSelection = new QToolButton;
	deleteSelection->setIcon(QIcon(":/images/gnome_edit_delete.png"));
	QToolButton *ctrl = new QToolButton;
	ctrl->setIcon(QIcon(":/images/key_ctrl.png"));
	ctrl->setCheckable(true);
	QToolBar *bottomBar = new QToolBar;
	bottomBar->addWidget(turnLeft);
	bottomBar->addWidget(turnRight);
	bottomBar->addSeparator();
	bottomBar->addWidget(deleteSelection);
	bottomBar->addSeparator();
	bottomBar->addWidget(ctrl);

	mVl->addWidget(topBar, 0, 0);
	mVl->addWidget(scrollArea, 1, 0);
	mVl->addWidget(bottomBar, 2, 0);

	connect(browser, SIGNAL(progressValueChanged(int)), dirProgress, SLOT(setValue(int)));
	connect(refresh, SIGNAL(clicked()), browser, SLOT(dirUpdate()));
	connect(starFilter, SIGNAL(currentIndexChanged(int)), browser, SLOT(setStarFilter(int)));
	connect(colorFilter, SIGNAL(currentIndexChanged(int)), browser, SLOT(setColorFilter(int)));
	connect(ctrl, SIGNAL(clicked(bool)), browser, SLOT(toggleSelectionMode(bool)));
	connect(turnLeft, SIGNAL(clicked()), browser, SLOT(rotateSelectionLeft()));
	connect(turnRight, SIGNAL(clicked()), browser, SLOT(rotateSelectionRight()));
	connect(deleteSelection, SIGNAL(clicked()), browser, SLOT(deleteSelection()));
	connect(magnifier, SIGNAL(clicked()), browser, SLOT(magnifier()));

	addWidget(leftView);
	addWidget(midView);
}

void MainView::dirSelected(QString directoryName) {
	browser->dirSelected(directoryName);
}
