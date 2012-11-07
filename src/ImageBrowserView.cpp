#include "ImageBrowserView.h"

#include "ImageBrowserModel.h"
#include "ImageWidget.h"

#define DIA_SIZE 200

ImageBrowserView::ImageBrowserView() {
	rows = 0;
	columns = 0;
	ctrlButtonPressed = false;

	gridLayout = new QGridLayout();
	setLayout(gridLayout);

	model = new ImageBrowserModel(this);

	progressValueChanged(0);

	connect(this, SIGNAL(showWidget(QWidget*, int)), this, SLOT(addWidget2View(QWidget*, int)));
}

void ImageBrowserView::dirSelected(QString directoryName) {
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	removeWidgets();
	cleanupWidgets();
	model->dirSelected(directoryName);
	createWidgets();
	updateView();
	QApplication::restoreOverrideCursor();
}

void ImageBrowserView::dirUpdate() {
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	removeWidgets();
	cleanupWidgets();
	model->dirUpdate();
	createWidgets();
	updateView();
	QApplication::restoreOverrideCursor();
}

void ImageBrowserView::resizeUpdate() {
	removeWidgets();
	updateView();
}

void ImageBrowserView::removeWidgets() {
	for (int i = 0; i < vector.size(); i++) {
		gridLayout->removeWidget(vector[i]);
	}
}

void ImageBrowserView::cleanupWidgets() {
	int count = vector.size();
	for (int i = 0; i < count; i++) {
		int last = vector.size() -1;
		delete vector[last];
		vector.remove(last);
	}
		
}

void ImageBrowserView::createWidgets() {
	int max = model->getNoOfFiles();
	int oldProgress = 0;
	int currentProgress = 0;
	progressValueChanged(0);

	for(int i = 0; i < max; i++) {
		QWidget *w = createImage(i);
		vector.append(w);

		// informieren der ProgressBar
		currentProgress = i * 100 / max;
		if (currentProgress != oldProgress) {
			progressValueChanged(currentProgress);
			oldProgress = currentProgress;
		}
	}
	progressValueChanged(0);
}

void ImageBrowserView::updateView() {

	if (vector.size() > 0) {
		// re-setting the previously set sizes of the spacer-columns and rows
		for (int r = 0; r < rows; r++) {
			gridLayout->setRowMinimumHeight(r * 2 +1, 0);
		}
		for (int c = 0; c < columns; c++) {
			gridLayout->setColumnMinimumWidth(c * 2 +1, 0);
		}		
		
		// determine the new count of rows and columns
		int saWidth = ((QScrollArea*) parent())->width();
		columns = saWidth / (DIA_SIZE + 10);
		if (columns == 0) columns = 1;
		rows = model->getNoOfFiles() / columns;
		if ((model->getNoOfFiles() % columns) > 0) {
			rows++;
		}

		// setting the new sizes of the spacer columns and rows
		for (int r = 0; r < rows; r++) {
			gridLayout->setRowMinimumHeight(r * 2 +1, 10);
		}
		for (int c = 0; c < columns; c++) {
			gridLayout->setColumnMinimumWidth(c * 2 +1, 10);
		}		
		setFixedSize(columns * (DIA_SIZE + 10), rows * (DIA_SIZE + 10));
		for (int i = 0; i < vector.size(); i++) {
			//gridLayout->addWidget(vector[i], (int) i / columns, (int) i % columns);
			showWidget(vector[i], i);
		}
	}

}

void ImageBrowserView::addWidget2View(QWidget *image, int i) {
	gridLayout->addWidget(image, (int) (i / columns) * 2, (int) (i % columns) * 2);
}

QWidget* ImageBrowserView::createImage(int i) {
	ImageWidget *imageWidget = new ImageWidget;

	imageWidget->setAbsoluteName(model->getAbsoluteFileName(i));
	imageWidget->setRating(model->getRating(i));
	imageWidget->setImage(model->getImage(i, DIA_SIZE - 50));
	imageWidget->setName(model->getFileName(i));
	imageWidget->setSize(DIA_SIZE, DIA_SIZE);

	return imageWidget;
}

void ImageBrowserView::updateRating(QString name, int rating) {
	model->updateRating(name, rating);
}

void ImageBrowserView::rotateSelectionLeft() {
	rotateSelection(ImageBrowserModel::LEFT);
}

void ImageBrowserView::rotateSelectionRight() {
	rotateSelection(ImageBrowserModel::RIGHT);
}

void ImageBrowserView::rotateSelection(ImageBrowserModel::Rotation direction) {
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	int max = getNoOfSelectedImages();
	int oldProgress = 0;
	int currentProgress = 0;
	int j = 0;
	progressValueChanged(0);
	for (int i = 0; i < vector.size(); i++) {
		if (((ImageWidget*) vector[i])->isSelected()) {
			model->rotateImage(i, direction);
			delete vector[i];
			vector[i] = createImage(i);
			showWidget(vector[i], i);
			((ImageWidget*) vector[i])->setSelected(true);

			// informieren der ProgressBar
			currentProgress = j * 100 / max;
			if (currentProgress != oldProgress) {
				progressValueChanged(currentProgress);
				oldProgress = currentProgress;
			}
			j++;
		}
	}
	progressValueChanged(0);
	QApplication::restoreOverrideCursor();
}

void ImageBrowserView::toggleSelectionMode(bool mode) {
	ctrlButtonPressed = mode;
}

bool ImageBrowserView::isCtrlButtonPressed() {
	return ctrlButtonPressed;
}

void ImageBrowserView::clearSelection() {
	for (int i = 0; i < vector.size(); i++) {
		if (((ImageWidget*) vector[i])->isSelected()) {
			((ImageWidget*) vector[i])->setSelected(false);
		}
	}
}

int ImageBrowserView::getNoOfSelectedImages() {
	int no = 0;
	for (int i = 0; i < vector.size(); i++) {
		if (((ImageWidget*) vector[i])->isSelected()) {
			no++;
		}
	}
	return no;
}
