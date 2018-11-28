#include "ToolFunctions.h"

#include <exiv2/exiv2.hpp>

#include <iostream>

int getImageOrientationFromFile(QString absoluteFilename) {
	int orientation = 1;
    QFileInfo fileInfo(absoluteFilename);
    QDir currentDirectory(fileInfo.absolutePath());
	try {

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

	return orientation;
}


void popImages(QWidget* w) {
	//ImageWidget *imageWidget;
	//imageWidget = (ImageWidget) w;
	//std::cout << imageWidget->getAbsoluteName().toStdString() << "\n";

	InterlaceConfig *conf = InterlaceConfig::getInstance();
    int maxsize = conf->getImageSize() - 50;
	QString absoluteFilename = ((ImageWidget*) w)->getAbsoluteName();
    std::cout << absoluteFilename.toStdString() << "\n";
    /*
	int index = 0;
	for (int i = 0; i < vector.size(); i++) {
		if (((ImageWidget*) vector[i])->getAbsoluteName() == name) {
			index = i;
		}
	}
	((ImageWidget*) w)->setImage(model->getImage(index, conf->getImageSize() - 50));
    */

	QImage qimage;
    QFileInfo fileInfo(absoluteFilename);
    QDir currentDirectory(fileInfo.absolutePath());
	int orientation = getImageOrientationFromFile(absoluteFilename);
	try {
		//QString absoluteFilename = files.at(index).absoluteFilePath();

		if (currentDirectory.exists("_PhotoNinja/" + fileInfo.fileName().replace(".", "_") + ".preview")) {
			qimage = QImage(fileInfo.absolutePath() +"/_PhotoNinja/" + fileInfo.fileName().replace(".", "_") + ".preview");
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

	//return qimage;
    ((ImageWidget*) w)->setImage(qimage);

}


