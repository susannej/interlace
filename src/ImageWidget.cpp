#include "ImageWidget.h"
#include "RatingWidget.h"
#include "ImageBrowserView.h"
#include "InterlaceConfig.h"

ImageWidget::ImageWidget() {
	setStyleSheet("background-color: #555");
	widgetLayout = new QGridLayout();
	setLayout(widgetLayout);

	widgetLayout->setRowStretch(1, 100);
	widgetLayout->setColumnStretch(0, 100);

	ratingWidget = new RatingWidget();
	widgetLayout->addWidget(ratingWidget, 0, 0, Qt::AlignCenter);
	imageLabel = new QLabel();
	widgetLayout->addWidget(imageLabel, 1, 0, Qt::AlignCenter);
	nameLabel = new QLabel();
	widgetLayout->addWidget(nameLabel, 2, 0, Qt::AlignCenter);

	selected = false;
}

ImageWidget::~ImageWidget() {
	delete nameLabel;
	delete imageLabel;
	delete ratingWidget;
	delete widgetLayout;
}

void ImageWidget::setImage(QImage image) {
	imageLabel->setPixmap(QPixmap::fromImage(image));
}

void ImageWidget::setName(QString name) {
	imageName = name;
	nameLabel->setText(name);
}

void ImageWidget::setAbsoluteName(QString absoluteName) {
	imageAbsoluteName = absoluteName;
}

void ImageWidget::setRating(int rating) {
	ratingWidget->setRating(rating);
}

void ImageWidget::updateRating(int rating) {
	((ImageBrowserView*) parent())->updateRating(imageName, rating);
}

void ImageWidget::setSize(int width, int height) {
	setFixedSize(width, height);
}

void ImageWidget::setSelected(bool sel) {
	if (sel) {
		setStyleSheet("background-color: #FFA02F; color: #000000");
		selected = true;
	} else {
		setStyleSheet("background-color: #555");
		selected = false;
	}
	update();
}

bool ImageWidget::isSelected() {
	return selected;
}

void ImageWidget::mousePressEvent(QMouseEvent *event) {
	if ((event->modifiers() & Qt::ControlModifier) || ((ImageBrowserView*) parent())->isCtrlButtonPressed()) {
		if (event->button() == Qt::LeftButton) {
			if (isSelected())
				setSelected(false);
			else
				setSelected(true);
		}
	} else {
		((ImageBrowserView*) parent())->clearSelection();
		setSelected(true);
	}
}

void ImageWidget::mouseDoubleClickEvent(QMouseEvent *event) {
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

	InterlaceConfig* conf = InterlaceConfig::getInstance();
	QString command = conf->getPrgCommand(0);

	qDebug() << "Aufruf von: " + command;

	QProcess *proc = new QProcess();
	connect(proc, SIGNAL( finished ( int , QProcess::ExitStatus)), proc, SLOT(deleteLater()));
	proc->start(conf->getPrgCommand(0), QStringList() << QDir::toNativeSeparators(imageAbsoluteName));
	proc->waitForStarted(-1);

	//system(("wine /home/joe/.wine/drive_c/windows/command/start.exe /Unix /home/joe/.wine/drive_c/Programme/PictureCode/PhotoNinja/PhotoNinja32.exe z:\"" + imageAbsoluteName + "\"").toAscii().data());
	QApplication::restoreOverrideCursor();
}

void ImageWidget::paintEvent(QPaintEvent *event) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

void ImageWidget::contextMenuEvent(QContextMenuEvent *event) {
	InterlaceConfig* conf = InterlaceConfig::getInstance();
    QMenu menu(this);
	connect(&menu, SIGNAL(triggered(QAction*)), this, SLOT(execPrg(QAction*)));
	for (int i = 0; i < conf->getConfProgramms(); i++) {
		QAction *prg = new QAction(QString::number(i+1) + " Öffnen in " + conf->getPrgDesc(i), this);
		menu.addAction(prg);
	}
     menu.exec(event->globalPos());
 }

void ImageWidget::execPrg(QAction *action) {
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

	QString actionText = action->text();
	int space = actionText.indexOf(" ");
	int no = actionText.left(space).toInt() -1;

	InterlaceConfig* conf = InterlaceConfig::getInstance();

	QProcess *proc = new QProcess();
	connect(proc, SIGNAL( finished ( int , QProcess::ExitStatus)), proc, SLOT(deleteLater()));
	proc->start(conf->getPrgCommand(no), QStringList() << QDir::toNativeSeparators(imageAbsoluteName));
	proc->waitForStarted(-1);

	QApplication::restoreOverrideCursor();
}