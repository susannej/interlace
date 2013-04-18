#include "ImageBrowserModel.h"

#include "ImageBrowserView.h"

#include <exiv2/exiv2.hpp>
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
	QByteArray barray = currentDirectory.toAscii();
	char* s = barray.data();
	printf("dirtree activated %s\n", s);
	*/
	printf("diretree activated\n");

	readdir();
}

void ImageBrowserModel::setStarFilter(int value) {
	starFilter = value;
}

int ImageBrowserModel::readdir() {
	QStringList fileExt;
	fileExt << "*.cr2" << "*.crw" << "*.mrw" << "*.dng" << "*.nef" << "*.pef" << "*.arw" << "*.rw2" << "*.sr2" << "*.srw" << "*.orf" << "*.pgf" << "*.raf";
	fileExt << "*.jpg" << "*.jpeg" << "*.tiff" << "*.tif" << "*.png" << "*.gif" << "*.psd" << "*.jp2" << "*.eps" << "*.exv";
	files = currentDirectory.entryInfoList(fileExt, QDir::Files | QDir::NoDotAndDotDot, QDir::Name);

printf("Directory enthaelt %d images\n", files.size());

	if (starFilter > 0) {
		for (int i = files.length() -1; i >= 0; i--) {
			if (getRating(i) < starFilter) {
				files.removeAt(i);
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
	int orientation = 1;
	try {
		QString absoluteFilename = files.at(index).absoluteFilePath();

		// Read the Metadata to get later at least the orientation...
		Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(absoluteFilename.toAscii().data());
		image->readMetadata();
		Exiv2::ExifData exifData = image->exifData();
		Exiv2::Exifdatum exifDatum = exifData["Exif.Image.Orientation"];
		orientation = (int) exifDatum.toLong();

		// XMP's are only created for RAW-files, other files do have their information embedded in the image!
		if (!absoluteFilename.endsWith(".jpg", Qt::CaseInsensitive)
			&& !absoluteFilename.endsWith(".tiff", Qt::CaseInsensitive)
			&& !absoluteFilename.endsWith(".png", Qt::CaseInsensitive)
			&& !absoluteFilename.endsWith(".tif", Qt::CaseInsensitive)
			&& !absoluteFilename.endsWith(".jpeg", Qt::CaseInsensitive)) {
			
			QString xmpFile = absoluteFilename.replace(absoluteFilename.size() -3, 3, "xmp");
			if (currentDirectory.exists(xmpFile)) {
				Exiv2::Image::AutoPtr xmpImage = Exiv2::ImageFactory::open(xmpFile.toAscii().data());
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
				Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(absoluteFilename.toAscii().data());
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

	if (qimage.width() > qimage.height()) {
		qimage = qimage.scaledToWidth(maxsize, Qt::SmoothTransformation); // Qt::SmoothTransformation oder Qt::FastTransformation
	} else {
		qimage = qimage.scaledToHeight(maxsize, Qt::SmoothTransformation);
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

	return qimage;
}

int ImageBrowserModel::getRating(int index) {
	int rating = 0;
	try {
		QString absoluteFilename = files.at(index).absoluteFilePath();

		// Read the Metadata to get later at least the orientation...
		Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(absoluteFilename.toAscii().data());
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
				Exiv2::Image::AutoPtr xmpImage = Exiv2::ImageFactory::open(xmpFile.toAscii().data());
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
			Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(absoluteFilename.toAscii().data());
			image->readMetadata();
			Exiv2::XmpData xmpData = image->xmpData();
			xmpData["Xmp.xmp.Rating"] = rating;
			image->setXmpData(xmpData);
			image->writeMetadata();

		} else {

			QString xmpFile = absoluteFilename.replace(absoluteFilename.size() -3, 3, "xmp");
			if (currentDirectory.exists(xmpFile)) {

				Exiv2::Image::AutoPtr xmpImage = Exiv2::ImageFactory::open(xmpFile.toAscii().data());
				xmpImage->readMetadata();
				Exiv2::XmpData xmpXmpData = xmpImage->xmpData();
				xmpXmpData["Xmp.xmp.Rating"] = rating;

				xmpImage->setXmpData(xmpXmpData);
				xmpImage->writeMetadata();
			} else {
				Exiv2::Image::AutoPtr xmpImage = Exiv2::ImageFactory::create(Exiv2::ImageType::xmp, xmpFile.toAscii().data());
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
}

void ImageBrowserModel::updateRating(QString name, int rating) {
	int i = 0;
	while (i < files.size() && name != files.at(i).fileName()) {
		i++;
	}
	updateRating(i, rating);
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

			Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(absoluteFilename.toAscii().data());
			image->readMetadata();
			Exiv2::ExifData exifData = image->exifData();
			exifData["Exif.Image.Orientation"] = orientation;
			image->setExifData(exifData);
			image->writeMetadata();

		} else {

			QString xmpFile = absoluteFilename.replace(absoluteFilename.size() -3, 3, "xmp");
			if (currentDirectory.exists(xmpFile)) {

				Exiv2::Image::AutoPtr xmpImage = Exiv2::ImageFactory::open(xmpFile.toAscii().data());
				xmpImage->readMetadata();
				Exiv2::ExifData exifData = xmpImage->exifData();
				Exiv2::XmpProperties::registerNs("http://ns.adobe.com/tiff/1.0/", "tiff");
				exifData["Exif.Image.Orientation"] = orientation;
				xmpImage->setExifData(exifData);
				xmpImage->writeMetadata();
			} else {
				Exiv2::Image::AutoPtr xmpImage = Exiv2::ImageFactory::create(Exiv2::ImageType::xmp, xmpFile.toAscii().data());
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
}

void ImageBrowserModel::deleteImage(int i) {
	try {
		QString absoluteFilename = files.at(i).absoluteFilePath();

		qDebug() << "Deleting " + absoluteFilename;

		QFile::remove(absoluteFilename);
		files.removeAt(i);
		QString xmpFile = absoluteFilename.replace(absoluteFilename.size() -3, 3, "xmp");
		if (currentDirectory.exists(xmpFile)) {
			QFile::remove(xmpFile);
		}
	} catch (Exiv2::AnyError& e) {
		std::cout << "Caught Exiv2 exception '" << e << "'\n";
	}
}