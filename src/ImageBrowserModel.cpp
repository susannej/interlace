#include "ImageBrowserModel.h"

#include "ImageBrowserView.h"

#include <cassert>

ImageBrowserModel::ImageBrowserModel(ImageBrowserView *view) {
	this->view = view;
	photoNinjaExists = false;
	starFilter = 0;
}

void ImageBrowserModel::dirSelected(QString directoryName) {
	currentDirectory = QDir(directoryName);
	dirUpdate();
}

void ImageBrowserModel::dirUpdate() {
	if (currentDirectory.exists("_PhotoNinja")) {
		photoNinjaExists = true;
	} else {
		photoNinjaExists = false;
	}

	/*
	QByteArray barray = currentDirectory.toLatin1();
	char* s = barray.data();
	printf("dirtree activated %s\n", s);
	*/
	printf("diretree activated\n");

	readdir();
}

void ImageBrowserModel::setStarFilter(int value) {
	starFilter = value;
}

void ImageBrowserModel::setColorFilter(QString label) {
	colorFilter = label;
}

int ImageBrowserModel::readdir() {
	QStringList fileExt;
	fileExt << "*.cr2" << "*.crw" << "*.mrw" << "*.dng" << "*.nef" << "*.pef" << "*.arw" << "*.rw2" << "*.sr2" << "*.srw" << "*.orf" << "*.pgf" << "*.raf";
	fileExt << "*.jpg" << "*.jpeg" << "*.tiff" << "*.tif" << "*.png" << "*.gif" << "*.psd" << "*.jp2" << "*.eps" << "*.exv";
	files.clear();
	files = currentDirectory.entryInfoList(fileExt, QDir::Files | QDir::NoDotAndDotDot, QDir::Name);

	printf("Directory enthaelt %d images\n", files.size());

	int max = getNoOfFiles();
	int oldProgress = 0;
	int currentProgress = 0;
	view->updateProgress(0);

	fileRatings.clear();
	fileRatings.reserve(files.size());
	fileLabels.clear();
	fileLabels.reserve(files.size());
	fileOrientations.clear();
	fileOrientations.reserve(files.size());

	for (int i = 0; i < files.size(); i++) {
		int rating = 0;
		QString label = "";
		int orientation = 1;
		try {
			QString absoluteFilename = files.at(i).absoluteFilePath();

			// Read the Metadata to get later at least the orientation...
			Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(absoluteFilename.toLatin1().data());
			image->readMetadata();
			
			Exiv2::XmpData xmpData = image->xmpData();
			Exiv2::Xmpdatum xmpDatum = xmpData["Xmp.xmp.Rating"];
			int tmpRating = (int) xmpDatum.toLong();
			if (tmpRating != -1) {
				rating = tmpRating;
			}
			xmpDatum = xmpData["Xmp.xmp.Label"];
			label = QString::fromStdString(xmpDatum.toString());

			Exiv2::ExifData exifData = image->exifData();
			Exiv2::Exifdatum exifDatum = exifData["Exif.Image.Orientation"];
			int tmpOrientation = (int) exifDatum.toLong();
			if (tmpOrientation != -1) {
				orientation = tmpOrientation;
			}

			// XMP's are only created for RAW-files, other files do have their information embedded in the image!
			if (!absoluteFilename.endsWith(".jpg", Qt::CaseInsensitive)
				&& !absoluteFilename.endsWith(".tiff", Qt::CaseInsensitive)
				&& !absoluteFilename.endsWith(".png", Qt::CaseInsensitive)
				&& !absoluteFilename.endsWith(".tif", Qt::CaseInsensitive)
				&& !absoluteFilename.endsWith(".jpeg", Qt::CaseInsensitive)) {
			
				QString xmpFile = absoluteFilename.replace(absoluteFilename.size() -3, 3, "xmp");
				if (currentDirectory.exists(xmpFile)) {
					Exiv2::Image::AutoPtr xmpImage = Exiv2::ImageFactory::open(xmpFile.toLatin1().data());
					xmpImage->readMetadata();

					xmpData = xmpImage->xmpData();
					xmpDatum = xmpData["Xmp.xmp.Rating"];
					int xmpRating = (int) xmpDatum.toLong();
					if (xmpRating != -1) {
						rating = xmpRating;
					}
					xmpDatum = xmpData["Xmp.xmp.Label"];
					label = QString::fromStdString(xmpDatum.toString());

					exifData = xmpImage->exifData();
					exifDatum = exifData["Exif.Image.Orientation"];
					int exifOrientation = (int) exifDatum.toLong();
					if (exifOrientation != -1) {
						orientation = exifOrientation;
					}
				}
			}

		} catch (Exiv2::AnyError& e) {
			std::cout << "Caught Exiv2 exception '" << e << "'\n";
		}

		if (rating < 0) rating = 0;

		fileRatings.append(rating);
		fileLabels.append(label);
		fileOrientations.append(orientation);

		// informieren der ProgressBar
		currentProgress = i * 100 / max;
		if (currentProgress != oldProgress) {
			view->updateProgress(currentProgress);
			oldProgress = currentProgress;
		}

	}

	view->updateProgress(0);

	if (starFilter > 0) {
		for (int i = files.length() -1; i >= 0; i--) {
			if (getRating(i) < starFilter) {
				files.removeAt(i);
				fileRatings.removeAt(i);
				fileLabels.removeAt(i);
				fileOrientations.removeAt(i);
			}
		}
	}
	if (colorFilter.size() > 0) {
		for (int i = files.length() -1; i >= 0; i--) {
			if (getLabel(i) != colorFilter) {
				files.removeAt(i);
				fileRatings.removeAt(i);
				fileLabels.removeAt(i);
				fileOrientations.removeAt(i);
			}
		}
	}

	return files.size();
}

int ImageBrowserModel::getNoOfFiles() {
	return files.size();
}

QString ImageBrowserModel::getFileName(int i) {
	return files.at(i).fileName();
}

QString ImageBrowserModel::getAbsoluteFileName(int i) {
	return files.at(i).absoluteFilePath();
}

int ImageBrowserModel::getImageOrientation(int index) {
	return fileOrientations.at(index);
}

int ImageBrowserModel::getImageOrientationFromFile(int index) {
	int orientation = 1;
	try {
		QString absoluteFilename = files.at(index).absoluteFilePath();

		// Read the Metadata to get later at least the orientation...
		Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(absoluteFilename.toLatin1().data());
		image->readMetadata();
		Exiv2::ExifData exifData = image->exifData();
		Exiv2::Exifdatum exifDatum = exifData["Exif.Image.Orientation"];
		int exifOrientation = (int) exifDatum.toLong();
		if (exifOrientation != -1) {
			orientation = exifOrientation;
		}

		// XMP's are only created for RAW-files, other files do have their information embedded in the image!
		if (!absoluteFilename.endsWith(".jpg", Qt::CaseInsensitive)
			&& !absoluteFilename.endsWith(".tiff", Qt::CaseInsensitive)
			&& !absoluteFilename.endsWith(".png", Qt::CaseInsensitive)
			&& !absoluteFilename.endsWith(".tif", Qt::CaseInsensitive)
			&& !absoluteFilename.endsWith(".jpeg", Qt::CaseInsensitive)) {
			
			QString xmpFile = absoluteFilename.replace(absoluteFilename.size() -3, 3, "xmp");
			if (currentDirectory.exists(xmpFile)) {
				Exiv2::Image::AutoPtr xmpImage = Exiv2::ImageFactory::open(xmpFile.toLatin1().data());
				xmpImage->readMetadata();
				Exiv2::ExifData xmpExifData = xmpImage->exifData();
				Exiv2::Exifdatum xmpExifDatum = xmpExifData["Exif.Image.Orientation"];
				int xmpOrientation = (int) xmpExifDatum.toLong();
				if (xmpOrientation != -1) {
					orientation = xmpOrientation;
				}
			}
		}
	} catch (Exiv2::AnyError& e) {
		std::cout << "Caught Exiv2 exception '" << e << "'\n";
	}

	fileOrientations.replace(index, orientation);
	return orientation;
}

QImage ImageBrowserModel::getImage(int index, int maxsize) {
	QImage qimage;
	int orientation = getImageOrientation(index);
	try {
		QString absoluteFilename = files.at(index).absoluteFilePath();

		if (currentDirectory.exists("_PhotoNinja/" + files.at(index).fileName().replace(".", "_") + ".preview")) {
			qimage = QImage(currentDirectory.absolutePath() +"/_PhotoNinja/" + files.at(index).fileName().replace(".", "_") + ".preview");
		} else {
			if (absoluteFilename.endsWith(".jpg", Qt::CaseInsensitive)
				|| absoluteFilename.endsWith(".tiff", Qt::CaseInsensitive)
				|| absoluteFilename.endsWith(".png", Qt::CaseInsensitive)
				|| absoluteFilename.endsWith(".tif", Qt::CaseInsensitive)
				|| absoluteFilename.endsWith(".jpeg", Qt::CaseInsensitive)) { // GIF, PSD, JP2, EPS auch noch ausprobieren!!!
				qimage = QImage(absoluteFilename);
			} else {
				Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(absoluteFilename.toLatin1().data());
				image->readMetadata();
				Exiv2::PreviewManager manager(*image);
				Exiv2::PreviewPropertiesList props = manager.getPreviewProperties();
				int i = 0;
				int id = -1;
				while (id == -1 && i != (int) props.size()) {
					Exiv2::PreviewProperties p = props.at(i);
					if (maxsize <= (int) p.width_ || maxsize <= (int) p.height_) {
						id = i;
					}
					i++;
				}
				if (id != -1) {
					Exiv2::PreviewImage pimage = manager.getPreviewImage(props.at(id));
					qimage.loadFromData(QByteArray((const char*) pimage.pData(), pimage.size()));
				} else {
					qimage = QImage(":images/Dickie.jpg");	
				}
			}
		}
	} catch (Exiv2::AnyError& e) {
		std::cout << "Caught Exiv2 exception '" << e << "'\n";
	}

	if (orientation != 1) {
		QTransform tf;
		switch (orientation) {
			case 3:
				tf.rotate(180);
				break;
			case 6:
				tf.rotate(90);
				break;
			case 8:
				tf.rotate(270);
				break;
		}	
		qimage = qimage.transformed(tf, Qt::SmoothTransformation);
	}

	int maxwidth = maxsize;
	int maxheight = maxsize - 40;
	qimage = qimage.scaled(maxwidth, maxheight, Qt::KeepAspectRatio, Qt::SmoothTransformation);

	/*
	if (qimage.width() > qimage.height()) {
		qimage = qimage.scaledToWidth(maxsize, Qt::SmoothTransformation); // Qt::SmoothTransformation oder Qt::FastTransformation
	} else {
		qimage = qimage.scaledToHeight(maxheight, Qt::SmoothTransformation);
	}
	*/

	return qimage;
}

int ImageBrowserModel::getRating(int index) {
	return fileRatings.at(index);
}

int ImageBrowserModel::getRatingFromFile(int index) {
	int rating = 0;
	try {
		QString absoluteFilename = files.at(index).absoluteFilePath();

		// Read the Metadata to get later at least the orientation...
		Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(absoluteFilename.toLatin1().data());
		image->readMetadata();
		Exiv2::XmpData xmpData = image->xmpData();
		Exiv2::Xmpdatum xmpDatum = xmpData["Xmp.xmp.Rating"];
		rating = (int) xmpDatum.toLong();

		// XMP's are only created for RAW-files, other files do have their information embedded in the image!
		if (!absoluteFilename.endsWith(".jpg", Qt::CaseInsensitive)
			&& !absoluteFilename.endsWith(".tiff", Qt::CaseInsensitive)
			&& !absoluteFilename.endsWith(".png", Qt::CaseInsensitive)
			&& !absoluteFilename.endsWith(".tif", Qt::CaseInsensitive)
			&& !absoluteFilename.endsWith(".jpeg", Qt::CaseInsensitive)) {
			
			QString xmpFile = absoluteFilename.replace(absoluteFilename.size() -3, 3, "xmp");
			if (currentDirectory.exists(xmpFile)) {
				Exiv2::Image::AutoPtr xmpImage = Exiv2::ImageFactory::open(xmpFile.toLatin1().data());
				xmpImage->readMetadata();
				Exiv2::XmpData xmpXmpData = xmpImage->xmpData();
				Exiv2::Xmpdatum xmpXmpDatum = xmpXmpData["Xmp.xmp.Rating"];
				int xmpRating = (int) xmpXmpDatum.toLong();
				if (xmpRating != -1) {
					rating = xmpRating;
				}
			}
		}

	} catch (Exiv2::AnyError& e) {
		std::cout << "Caught Exiv2 exception '" << e << "'\n";
	}

	if (rating < 0) rating = 0;

	fileRatings.replace(index, rating);
	return rating;
}

void ImageBrowserModel::updateRating(int i, int rating) {
	try {
		QString absoluteFilename = files.at(i).absoluteFilePath();


		// XMP's are only created for RAW-files, other files do have their information embedded in the image!
		if (absoluteFilename.endsWith(".jpg", Qt::CaseInsensitive)
			|| absoluteFilename.endsWith(".tiff", Qt::CaseInsensitive)
			|| absoluteFilename.endsWith(".png", Qt::CaseInsensitive)
			|| absoluteFilename.endsWith(".tif", Qt::CaseInsensitive)
			|| absoluteFilename.endsWith(".jpeg", Qt::CaseInsensitive)) {

			// Read the Metadata to get later at least the orientation...
			Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(absoluteFilename.toLatin1().data());
			image->readMetadata();
			Exiv2::XmpData xmpData = image->xmpData();
			xmpData["Xmp.xmp.Rating"] = rating;
			image->setXmpData(xmpData);
			image->writeMetadata();

		} else {

			QString xmpFile = absoluteFilename.replace(absoluteFilename.size() -3, 3, "xmp");
			if (currentDirectory.exists(xmpFile)) {

				Exiv2::Image::AutoPtr xmpImage = Exiv2::ImageFactory::open(xmpFile.toLatin1().data());
				xmpImage->readMetadata();
				Exiv2::XmpData xmpXmpData = xmpImage->xmpData();
				xmpXmpData["Xmp.xmp.Rating"] = rating;

				xmpImage->setXmpData(xmpXmpData);
				xmpImage->writeMetadata();
			} else {
				Exiv2::Image::AutoPtr xmpImage = Exiv2::ImageFactory::create(Exiv2::ImageType::xmp, xmpFile.toLatin1().data());
				Exiv2::XmpData xmpData;
				Exiv2::XmpProperties::registerNs("http://ns.adobe.com/xap/1.0/", "xmp");
				xmpData["Xmp.xmp.Rating"] = rating;
				
				xmpImage->setXmpData(xmpData);
				xmpImage->writeMetadata();

			}
		}

	} catch (Exiv2::AnyError& e) {
		std::cout << "Caught Exiv2 exception '" << e << "'\n";
	}

	getRatingFromFile(i);
}

void ImageBrowserModel::updateRating(QString name, int rating) {
	int i = 0;
	while (i < files.size() && name != files.at(i).fileName()) {
		i++;
	}
	updateRating(i, rating);
}

QString ImageBrowserModel::getLabel(int index) {
	return fileLabels.at(index);
}

QString ImageBrowserModel::getLabelFromFile(int index) {
	QString label = "";
	QString absoluteFilename = files.at(index).absoluteFilePath();
qDebug() << "Bearbeitung für Image: " + absoluteFilename;
	try {
		// Read the Metadata to get later at least the orientation...
		Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(absoluteFilename.toLatin1().data());
		image->readMetadata();
		Exiv2::XmpData xmpData = image->xmpData();
		Exiv2::Xmpdatum xmpDatum = xmpData["Xmp.xmp.Label"];
		label = QString::fromStdString(xmpDatum.toString());

		// XMP's are only created for RAW-files, other files do have their information embedded in the image!
		if (!absoluteFilename.endsWith(".jpg", Qt::CaseInsensitive)
			&& !absoluteFilename.endsWith(".tiff", Qt::CaseInsensitive)
			&& !absoluteFilename.endsWith(".png", Qt::CaseInsensitive)
			&& !absoluteFilename.endsWith(".tif", Qt::CaseInsensitive)
			&& !absoluteFilename.endsWith(".jpeg", Qt::CaseInsensitive)) {
			
			QString xmpFile = absoluteFilename.replace(absoluteFilename.size() -3, 3, "xmp");
			if (currentDirectory.exists(xmpFile)) {
				Exiv2::Image::AutoPtr xmpImage = Exiv2::ImageFactory::open(xmpFile.toLatin1().data());
				xmpImage->readMetadata();
				Exiv2::XmpData xmpXmpData = xmpImage->xmpData();
				Exiv2::Xmpdatum xmpXmpDatum = xmpXmpData["Xmp.xmp.Label"];
				label = QString::fromStdString(xmpXmpDatum.toString());
				/*
				int xmpRating = (int) xmpXmpDatum.toLong();
				if (xmpRating != -1) {
					rating = xmpRating;
				}
				*/
			}
		}

	} catch (Exiv2::AnyError& e) {
		std::cout << "Caught Exiv2 exception '" << e << "'\n";
	}

qDebug() << "xmp.Label = " + label + " für Image: " + absoluteFilename;

	/*
	if (rating < 0) rating = 0;
	return rating;
	*/

	fileLabels.replace(index, label);
	return label;
}

void ImageBrowserModel::updateLabel(int i, QString label) {
	try {
		QString absoluteFilename = files.at(i).absoluteFilePath();


		// XMP's are only created for RAW-files, other files do have their information embedded in the image!
		if (absoluteFilename.endsWith(".jpg", Qt::CaseInsensitive)
			|| absoluteFilename.endsWith(".tiff", Qt::CaseInsensitive)
			|| absoluteFilename.endsWith(".png", Qt::CaseInsensitive)
			|| absoluteFilename.endsWith(".tif", Qt::CaseInsensitive)
			|| absoluteFilename.endsWith(".jpeg", Qt::CaseInsensitive)) {

			// Read the Metadata to get later at least the orientation...
			Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(absoluteFilename.toLatin1().data());
			image->readMetadata();
			Exiv2::XmpData xmpData = image->xmpData();
			xmpData["Xmp.xmp.Label"] = label.toStdString();
			image->setXmpData(xmpData);
			image->writeMetadata();

		} else {

			QString xmpFile = absoluteFilename.replace(absoluteFilename.size() -3, 3, "xmp");
			if (currentDirectory.exists(xmpFile)) {

				Exiv2::Image::AutoPtr xmpImage = Exiv2::ImageFactory::open(xmpFile.toLatin1().data());
				xmpImage->readMetadata();
				Exiv2::XmpData xmpXmpData = xmpImage->xmpData();
				xmpXmpData["Xmp.xmp.Label"] = label.toStdString();

				xmpImage->setXmpData(xmpXmpData);
				xmpImage->writeMetadata();
			} else {
				Exiv2::Image::AutoPtr xmpImage = Exiv2::ImageFactory::create(Exiv2::ImageType::xmp, xmpFile.toLatin1().data());
				Exiv2::XmpData xmpData;
				Exiv2::XmpProperties::registerNs("http://ns.adobe.com/xap/1.0/", "xmp");
				xmpData["Xmp.xmp.Label"] = label.toStdString();
				
				xmpImage->setXmpData(xmpData);
				xmpImage->writeMetadata();

			}
		}

	} catch (Exiv2::AnyError& e) {
		std::cout << "Caught Exiv2 exception '" << e << "'\n";
	}

	getLabelFromFile(i);
}

void ImageBrowserModel::updateLabel(QString name, QString label) {
	int i = 0;
	while (i < files.size() && name != files.at(i).fileName()) {
		i++;
	}
	updateLabel(i, label);
}

void ImageBrowserModel::rotateImage(int i, Rotation direction) {
	int orientation = getImageOrientation(i);

	if (direction == RIGHT) {
		switch (orientation) {
			case 1:
				orientation = 6;
				break;
			case 3:
				orientation = 8;
				break;
			case 6:
				orientation = 3;
				break;
			case 8:
				orientation = 1;
				break;
		}
	} else if (direction == LEFT) {
		switch (orientation) {
			case 1:
				orientation = 8;
				break;
			case 3:
				orientation = 6;
				break;
			case 6:
				orientation = 1;
				break;
			case 8:
				orientation = 3;
				break;
		}
	}

	try {
		QString absoluteFilename = files.at(i).absoluteFilePath();


		// XMP's are only created for RAW-files, other files do have their information embedded in the image!
		if (absoluteFilename.endsWith(".jpg", Qt::CaseInsensitive)
			|| absoluteFilename.endsWith(".tiff", Qt::CaseInsensitive)
			|| absoluteFilename.endsWith(".png", Qt::CaseInsensitive)
			|| absoluteFilename.endsWith(".tif", Qt::CaseInsensitive)
			|| absoluteFilename.endsWith(".jpeg", Qt::CaseInsensitive)) {

			Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(absoluteFilename.toLatin1().data());
			image->readMetadata();
			Exiv2::ExifData exifData = image->exifData();
			exifData["Exif.Image.Orientation"] = orientation;
			image->setExifData(exifData);
			image->writeMetadata();

		} else {

			QString xmpFile = absoluteFilename.replace(absoluteFilename.size() -3, 3, "xmp");
			if (currentDirectory.exists(xmpFile)) {

				Exiv2::Image::AutoPtr xmpImage = Exiv2::ImageFactory::open(xmpFile.toLatin1().data());
				xmpImage->readMetadata();
				Exiv2::ExifData exifData = xmpImage->exifData();
				Exiv2::XmpProperties::registerNs("http://ns.adobe.com/tiff/1.0/", "tiff");
				exifData["Exif.Image.Orientation"] = orientation;
				xmpImage->setExifData(exifData);
				xmpImage->writeMetadata();
			} else {
				Exiv2::Image::AutoPtr xmpImage = Exiv2::ImageFactory::create(Exiv2::ImageType::xmp, xmpFile.toLatin1().data());
				Exiv2::ExifData exifData;
				Exiv2::XmpProperties::registerNs("http://ns.adobe.com/tiff/1.0/", "tiff");
				exifData["Exif.Image.Orientation"] = orientation;
				
				xmpImage->setExifData(exifData);
				xmpImage->writeMetadata();

			}
		}

	} catch (Exiv2::AnyError& e) {
		std::cout << "Caught Exiv2 exception '" << e << "'\n";
	}

	getImageOrientationFromFile(i);
}

void ImageBrowserModel::deleteImage(int i) {
	try {
		QString absoluteFilename = files.at(i).absoluteFilePath();

		qDebug() << "Deleting " + absoluteFilename;

		QFile::remove(absoluteFilename);
		files.removeAt(i);
		fileRatings.removeAt(i);
		fileLabels.removeAt(i);
		fileOrientations.removeAt(i);
		QString xmpFile = absoluteFilename.replace(absoluteFilename.size() -3, 3, "xmp");
		if (currentDirectory.exists(xmpFile)) {
			QFile::remove(xmpFile);
		}
	} catch (Exiv2::AnyError& e) {
		std::cout << "Caught Exiv2 exception '" << e << "'\n";
	}
}

QStringList ImageBrowserModel::readImageData(QStringList images) {
	InterlaceConfig *conf = InterlaceConfig::getInstance();
	QStringList exifKeys = conf->getExifKeys();
	QStringList exifValues;

	// Initialize the exifValues List
	for (int i = 0; i < exifKeys.size(); i++) {
		exifValues << "";
	}

	// Schleife über alle selektierten Bilder
	for (int i = 0; i < images.size(); i++) {
		Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(images.at(i).toLatin1().data());

		image->readMetadata();
		Exiv2::XmpData xmpData = image->xmpData();
		Exiv2::ExifData exifData = image->exifData();
		Exiv2::IptcData iptcData = image->iptcData();

		// empty Xmp-File data
		Exiv2::Image::AutoPtr xmpImage;
		Exiv2::XmpData xmpXmpData;
		Exiv2::ExifData xmpExifData;
		Exiv2::IptcData xmpIptcData;
		bool xmpExists = false;

		// XMP's are only created for RAW-files, other files do have their information embedded in the image!
		if (!images.at(i).endsWith(".jpg", Qt::CaseInsensitive)
			&& !images.at(i).endsWith(".tiff", Qt::CaseInsensitive)
			&& !images.at(i).endsWith(".png", Qt::CaseInsensitive)
			&& !images.at(i).endsWith(".tif", Qt::CaseInsensitive)
			&& !images.at(i).endsWith(".jpeg", Qt::CaseInsensitive)) {
			
			QString xmpFilename = images.at(i);
			QString xmpFile = xmpFilename.replace(images.at(i).size() -3, 3, "xmp");
			if (currentDirectory.exists(xmpFile)) {
				//try {
					xmpImage = Exiv2::ImageFactory::open(xmpFile.toLatin1().data());
					xmpImage->readMetadata();
					xmpXmpData = xmpImage->xmpData();
					xmpExifData = xmpImage->exifData();
					xmpIptcData = xmpImage->iptcData();
					xmpExists = true;
				//} catch (Exiv2::AnyError& e) {
				//}
			}
		}

		for (int j = 0; j < exifKeys.size(); j++) {
			QString key = exifKeys.at(j);
			QString value;
			if (key.startsWith("Exif")) {
				try {
					Exiv2::Exifdatum tmpValue = exifData[key.toLatin1().data()];
					value = QString::fromStdString(tmpValue.print());
				} catch (Exiv2::AnyError& e) {
					value = "-";
				}
				if (xmpExists) {
					try {
						Exiv2::Exifdatum tmpValue = xmpExifData[key.toLatin1().data()];
						value = QString::fromStdString(tmpValue.print());
					} catch (Exiv2::AnyError& e) {
					}
				}
			} else if (key.startsWith("Xmp")) {
				try {
					Exiv2::Xmpdatum tmpValue = xmpData[key.toLatin1().data()];
					value = QString::fromStdString(tmpValue.print());
				} catch (Exiv2::AnyError& e) {
					value = "-";
				}
				if (xmpExists) {
					try {
						Exiv2::Xmpdatum tmpValue = xmpXmpData[key.toLatin1().data()];
						value = QString::fromStdString(tmpValue.print());
					} catch (Exiv2::AnyError& e) {
					}
				}
			} else if (key.startsWith("Iptc")) {
				try {
					Exiv2::Iptcdatum tmpValue = iptcData[key.toLatin1().data()];
					value = QString::fromStdString(tmpValue.print());
				} catch (Exiv2::AnyError& e) {
					value = "-";
				}
				if (xmpExists) {
					try {
						Exiv2::Iptcdatum tmpValue = xmpIptcData[key.toLatin1().data()];
						value = QString::fromStdString(tmpValue.print());
					} catch (Exiv2::AnyError& e) {
					}
				}
			}

			if (i == 0) {
				exifValues.replace(j, value);
			} else if (exifValues.at(j) != "<diff>" && exifValues.at(j) != value) {
				exifValues.replace(j, "<diff>");
			}
		}
	}

	return exifValues;
}

void ImageBrowserModel::writeImageData(QStringList images, QString exifKey, QString text)  {

	// Schleife über alle selektierten Bilder
	for (int i = 0; i < images.size(); i++) {
		try {
			QString absoluteFilename = images.at(i);


			// XMP's are only created for RAW-files, other files do have their information embedded in the image!
			if (absoluteFilename.endsWith(".jpg", Qt::CaseInsensitive)
				|| absoluteFilename.endsWith(".tiff", Qt::CaseInsensitive)
				|| absoluteFilename.endsWith(".png", Qt::CaseInsensitive)
				|| absoluteFilename.endsWith(".tif", Qt::CaseInsensitive)
				|| absoluteFilename.endsWith(".jpeg", Qt::CaseInsensitive)) {
	
				Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(absoluteFilename.toLatin1().data());
				image->readMetadata();
				if (exifKey.startsWith("Exif")) {
					Exiv2::ExifData exifData = image->exifData();
					exifData[exifKey.toLatin1().data()] = text.toLatin1().data();
					image->setExifData(exifData);
				} else if (exifKey.startsWith("Xmp")) {
					Exiv2::XmpData xmpData = image->xmpData();
					xmpData[exifKey.toLatin1().data()] = text.toLatin1().data();
					image->setXmpData(xmpData);
				} else if (exifKey.startsWith("Iptc")) {
					Exiv2::IptcData iptcData = image->iptcData();
					iptcData[exifKey.toLatin1().data()] = text.toLatin1().data();
					image->setIptcData(iptcData);
				}
				image->writeMetadata();

			} else {
	
				QString xmpFile = absoluteFilename.replace(absoluteFilename.size() -3, 3, "xmp");
				if (currentDirectory.exists(xmpFile)) {
	
					Exiv2::Image::AutoPtr xmpImage = Exiv2::ImageFactory::open(xmpFile.toLatin1().data());
					xmpImage->readMetadata();
					if (exifKey.startsWith("Exif")) {
qDebug() << "Xmp-File update";
						Exiv2::ExifData exifData = xmpImage->exifData();
						Exiv2::XmpProperties::registerNs("http://ns.adobe.com/tiff/1.0/", "tiff");
						exifData[exifKey.toLatin1().data()] = text.toLatin1().data();
						xmpImage->setExifData(exifData);
qDebug() << QString::fromStdString(exifData[exifKey.toLatin1().data()].print());
qDebug() << "Xmp File update ende";
					}  else if (exifKey.startsWith("Xmp")) {
						Exiv2::XmpData xmpData = xmpImage->xmpData();
						Exiv2::XmpProperties::registerNs("http://ns.adobe.com/tiff/1.0/", "tiff");
						xmpData[exifKey.toLatin1().data()] = text.toLatin1().data();
						xmpImage->setXmpData(xmpData);
					}  else if (exifKey.startsWith("Iptc")) {
						Exiv2::IptcData iptcData = xmpImage->iptcData();
						Exiv2::XmpProperties::registerNs("http://ns.adobe.com/tiff/1.0/", "tiff");
						iptcData[exifKey.toLatin1().data()] = text.toLatin1().data();
						xmpImage->setIptcData(iptcData);
					}
					xmpImage->writeMetadata();
Exiv2::ExifData exifData = xmpImage->exifData();
qDebug() << QString::fromStdString(exifData[exifKey.toLatin1().data()].print());
qDebug() << "Xmp write meta";
				} else {
					Exiv2::Image::AutoPtr xmpImage = Exiv2::ImageFactory::create(Exiv2::ImageType::xmp, xmpFile.toLatin1().data());
					if (exifKey.startsWith("Exif")) {
						Exiv2::ExifData exifData;
						Exiv2::XmpProperties::registerNs("http://ns.adobe.com/tiff/1.0/", "tiff");
						exifData[exifKey.toLatin1().data()] = text.toLatin1().data();
						xmpImage->setExifData(exifData);
					}  else if (exifKey.startsWith("Xmp")) {
						Exiv2::XmpData xmpData;
						Exiv2::XmpProperties::registerNs("http://ns.adobe.com/tiff/1.0/", "tiff");
						xmpData[exifKey.toLatin1().data()] = text.toLatin1().data();
						xmpImage->setXmpData(xmpData);
					}  else if (exifKey.startsWith("Iptc")) {
						Exiv2::IptcData iptcData;
						Exiv2::XmpProperties::registerNs("http://ns.adobe.com/tiff/1.0/", "tiff");
						iptcData[exifKey.toLatin1().data()] = text.toLatin1().data();
						xmpImage->setIptcData(iptcData);
					}
					xmpImage->writeMetadata();
	
				}
			}
	
		} catch (Exiv2::AnyError& e) {
			std::cout << "Caught Exiv2 exception '" << e << "'\n";
		}
	}
}
