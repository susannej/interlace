#include "MagnifierDialog.h"
#include "RatingWidget.h"
#include "ImageBrowserView.h"
#include "InterlaceConfig.h"

MagnifierDialog::MagnifierDialog(QWidget *parent) : QDialog(parent) {
	//setStyleSheet("background-color: #555");
	setStyleSheet("background-color: #000");
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
	colorLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//colorLabel->setStyleSheet("background-color: #700");
	//colorLabel->setStyleSheet("background-color: #555");
	colorLabel->setStyleSheet("background-color: #000");
	widgetLayout->addWidget(colorLabel, 1, 0, Qt::AlignCenter);
	imageLabel = new QLabel();
	imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	l->addWidget(imageLabel, 1, 1, Qt::AlignCenter);
	
	nameLabel = new QLabel();
	widgetLayout->addWidget(nameLabel, 2, 0, Qt::AlignCenter);
	currentImage = 0;
	imageNos = NULL;
	m_model = NULL;
	imageSize = 250;
	conf = InterlaceConfig::getInstance();

	setWindowState(Qt::WindowMaximized);
}

MagnifierDialog::~MagnifierDialog() {
	qDebug() << "ImageWidget Destructor called!";
	delete nameLabel;
	delete imageLabel;
	delete ratingWidget;
	delete widgetLayout;
}

void MagnifierDialog::setImages(QVector<int> *images) {
	qDebug() << "Höhe = " << this->height() << " asdf";
	imageNos = images;
	currentImage = 0;
	setImage();
}

void MagnifierDialog::setModel(ImageBrowserModel *model) {
	m_model = model;
}

void MagnifierDialog::setStartImage(int no) {
	qDebug() << "Höhe = " << this->height() << " asdf";
	currentImage = no;
	setImage();
}

void MagnifierDialog::keyPressEvent(QKeyEvent *event) {
	int i = 0;
	switch (event->key()) {
	case Qt::Key_Escape:
		close();
		break;
	case Qt::Key_Up:
		if (imageNos != NULL) {
			i = conf->getLabelIndex(m_model->getLabel(imageNos->at(currentImage)));
		} else {
			i = conf->getLabelIndex(m_model->getLabel(currentImage));
		}
		i = i -1;
		if (i <= -1) {
			i = -1;
			colorLabel->setStyleSheet("background-color: #000");
			if (imageNos != NULL) 
				m_model->updateLabel(imageNos->at(currentImage), "");
			else 
				m_model->updateLabel(currentImage, "");
		} else {
			colorLabel->setStyleSheet("background-color: " + conf->getLabelColor(i));
			if (imageNos != NULL) 
				m_model->updateLabel(imageNos->at(currentImage), conf->getLabelDesc(i));
			else 
				m_model->updateLabel(currentImage, conf->getLabelDesc(i));
		}
		break;
	case Qt::Key_Down:
		if (imageNos != NULL) {
			i = conf->getLabelIndex(m_model->getLabel(imageNos->at(currentImage)));
		} else {
			i = conf->getLabelIndex(m_model->getLabel(currentImage));
		}
		i = i +1;
		if (i >= conf->getConfLabels()) i = conf->getConfLabels() -1;
		colorLabel->setStyleSheet("background-color: " + conf->getLabelColor(i));
		if (imageNos != NULL) 
			m_model->updateLabel(imageNos->at(currentImage), conf->getLabelDesc(i));
		else 
			m_model->updateLabel(currentImage, conf->getLabelDesc(i));
		break;
	case Qt::Key_Left:
		currentImage = currentImage -1;
		setImage();
		break;
	case Qt::Key_Right:
		currentImage = currentImage +1;
		setImage();
		break;
	case Qt::Key_0:
		updateRating(0);
		ratingWidget->setRating(0);
		break;
	case Qt::Key_1:
		updateRating(1);
		ratingWidget->setRating(1);
		break;
	case Qt::Key_2:
		updateRating(2);
		ratingWidget->setRating(2);
		break;
	case Qt::Key_3:
		updateRating(3);
		ratingWidget->setRating(3);
		break;
	case Qt::Key_4:
		updateRating(4);
		ratingWidget->setRating(4);
		break;
	case Qt::Key_5:
		updateRating(5);
		ratingWidget->setRating(5);
		break;
	}
}

void MagnifierDialog::resizeEvent(QResizeEvent *event) {
	qDebug() << "Resize event, height = " << (event->size()).height();
	imageSize = (event->size()).height() -100;
	setImage();
}

void MagnifierDialog::setImage() {
	if (currentImage < 0) currentImage = 0;
	if (imageNos != NULL) {
		if (currentImage >= imageNos->size()) currentImage = imageNos->size() -1;
		imageLabel->setPixmap(QPixmap::fromImage(m_model->getImage(imageNos->at(currentImage), imageSize)));
		ratingWidget->setRating(m_model->getRating(imageNos->at(currentImage)));
		nameLabel->setText(m_model->getFileName(imageNos->at(currentImage)) + " - Bild " + QString::number(currentImage +1) + " von " + QString::number(imageNos->size()));
		setWindowTitle("Interlace - " + m_model->getFileName(imageNos->at(currentImage)));
		colorLabel->setStyleSheet("background-color: " + conf->getLabelColor(m_model->getLabel(imageNos->at(currentImage))));
	} else {
		if (currentImage >= m_model->getNoOfFiles()) currentImage = m_model->getNoOfFiles() -1;
		imageLabel->setPixmap(QPixmap::fromImage(m_model->getImage(currentImage, imageSize)));
		ratingWidget->setRating(m_model->getRating(currentImage));
		nameLabel->setText(m_model->getFileName(currentImage) + " - Bild " + QString::number(currentImage +1) + " von " + QString::number(m_model->getNoOfFiles()));
		setWindowTitle("Interlace - " + m_model->getFileName(currentImage));
		colorLabel->setStyleSheet("background-color: " + conf->getLabelColor(m_model->getLabel(currentImage)));
	}
}

void MagnifierDialog::updateRating(int rating) {
	if (imageNos != NULL) {
		m_model->updateRating(imageNos->at(currentImage), rating);
	} else {
		m_model->updateRating(currentImage, rating);
	}
}