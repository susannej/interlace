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
	
	m_label = QString();
	colorLabel = new QWidget();
	QGridLayout *l = new QGridLayout();
	colorLabel->setLayout(l);
	//colorLabel->setStyleSheet("background-color: #F00");
	//colorLabel->setStyleSheet("background-color: Red");
	colorLabel->setStyleSheet("background-color: #555");
	widgetLayout->addWidget(colorLabel, 1, 0, Qt::AlignCenter);
	imageLabel = new QLabel();
	l->addWidget(imageLabel, 1, 1, Qt::AlignCenter);
	
	nameLabel = new QLabel();
	widgetLayout->addWidget(nameLabel, 2, 0, Qt::AlignCenter);

	selected = false;
}

ImageWidget::~ImageWidget() {
	qDebug() << "ImageWidget Destructor called!";
	delete nameLabel;
	delete imageLabel;
	delete ratingWidget;
	delete widgetLayout;
	delete colorLabel;
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

QString ImageWidget::getAbsoluteName() {
	return imageAbsoluteName;
}

void ImageWidget::setRating(int rating) {
	ratingWidget->setRating(rating);
}

void ImageWidget::setLabel(QString label) {
	m_label = label;
	InterlaceConfig *conf = InterlaceConfig::getInstance();
	QString color = conf->getLabelColor(label);
	if (color.size() > 0) {
		colorLabel->setStyleSheet("background-color: " + color);
	} else {
		if (selected)
			colorLabel->setStyleSheet(SELECTED_COLOR);
		else
			colorLabel->setStyleSheet("background-color: #555");
	}
}

void ImageWidget::updateLabel(QString label) {
	setLabel(label);
	((ImageBrowserView*) parent())->updateLabel(imageName, label);
}

void ImageWidget::updateRating(int rating) {
	((ImageBrowserView*) parent())->updateRating(imageName, rating);
}

void ImageWidget::setSize(int width, int height) {
	setFixedSize(width, height);
}

void ImageWidget::setSelected(bool sel) {
	if (sel) {
		//setStyleSheet("background-color: #FFA02F; color: #000000");
		setStyleSheet(SELECTED_COLOR);
		selected = true;
		if (m_label.size() == 0) {
			colorLabel->setStyleSheet(SELECTED_COLOR);
		}
	} else {
		setStyleSheet("background-color: #555");
		selected = false;
		if (m_label.length() == 0) {
			colorLabel->setStyleSheet("background-color: #555");
		}
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
	QString confCommand = conf->getPrgCommand(0);
	QString file = QDir::toNativeSeparators(imageAbsoluteName);
	QString command = confCommand.replace("${file}", file);

	qDebug() << "Aufruf von: " + command;

	QProcess *proc = new QProcess();
	connect(proc, SIGNAL( finished ( int , QProcess::ExitStatus)), proc, SLOT(deleteLater()));
qDebug() << QDir::toNativeSeparators(imageAbsoluteName);
	proc->start(command);
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
		QAction *prg = new QAction(QString::number(i+1) + " Open with " + conf->getPrgDesc(i), this);
		menu.addAction(prg);
	}

	menu.addSeparator();
	
	QAction *labelNone = new QAction(conf->getIcon4Color("None"), "Set label to None", this);
	menu.addAction(labelNone);
	for (int i = 0; i < conf->getConfLabels(); i++) {
		QAction *label = new QAction(conf->getIcon4Color(conf->getLabelDesc(i)), "Set label to " + conf->getLabelDesc(i), this);
		menu.addAction(label);
	}
	menu.exec(event->globalPos());
 }

void ImageWidget::execPrg(QAction *action) {
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

	QString actionText = action->text();

	if (actionText.startsWith("Set label")) {
		int no = sizeof("Set label to ");
		QString labelText = actionText.mid(no -1);
		if (labelText == "None")
			updateLabel("");
		else 
			updateLabel(labelText);
qDebug() << "Setting label to " + labelText;
	} else {
		int space = actionText.indexOf(" ");
		int no = actionText.left(space).toInt() -1;

		InterlaceConfig* conf = InterlaceConfig::getInstance();

		QString confCommand = conf->getPrgCommand(no);
		QString file = QDir::toNativeSeparators(imageAbsoluteName);
		QString command = confCommand.replace("${file}", file);

		QProcess *proc = new QProcess();
		connect(proc, SIGNAL( finished ( int , QProcess::ExitStatus)), proc, SLOT(deleteLater()));
		proc->start(command);
		proc->waitForStarted(-1);
	}

	QApplication::restoreOverrideCursor();
}
