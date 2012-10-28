#include "ImageBrowserModel.h"

#include "ImageBrowserView.h"

#include <exiv2/exiv2.hpp>
#include <cassert>

ImageBrowserModel::ImageBrowserModel(ImageBrowserView *view) {
	this->view = view;
	photoNinjaExists = false;
}

void ImageBrowserModel::dirSelected(QString directoryName) {
	currentDirectory = QDir(directoryName);

	if (currentDirectory.exists("_PhotoNinja")) {
		photoNinjaExists = true;
	} else {
		photoNinjaExists = false;
	}

	QByteArray barray = directoryName.toAscii();
	char* s = barray.data();
	printf("dirtree activated %s\n", s);

	readdir();
}

int ImageBrowserModel::readdir() {
	QStringList fileExt;
	fileExt << "*.cr2" << "*.crw" << "*.mrw" << "*.dng" << "*.nef" << "*.pef" << "*.arw" << "*.rw2" << "*.sr2" << "*.srw" << "*.orf" << "*.pgf" << "*.raf";
	fileExt << "*.jpg" << "*.jpeg" << "*.tiff" << "*.tif" << "*.png" << "*.gif" << "*.psd" << "*.jp2" << "*.eps" << "*.exv";
	files = currentDirectory.entryInfoList(fileExt, QDir::Files | QDir::NoDotAndDotDot, QDir::Name);

printf("Directory enhaelt %d images\n", files.size());

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

QImage ImageBrowserModel::getImage(int index, int maxsize) {
	QImage qimage;
	try {
		QString absoluteFilename = files.at(index).absoluteFilePath();
		if (absoluteFilename.endsWith(".jpg", Qt::CaseInsensitive)
			|| absoluteFilename.endsWith(".tiff", Qt::CaseInsensitive)
			|| absoluteFilename.endsWith(".png", Qt::CaseInsensitive)
			|| absoluteFilename.endsWith(".tif", Qt::CaseInsensitive)
			|| absoluteFilename.endsWith(".jpeg", Qt::CaseInsensitive)) { // GIF, PSD, JP2, EPS auch noch ausprobieren!!!
			qimage = QImage(absoluteFilename);
		} else {
			if (currentDirectory.exists("_PhotoNinja/" + files.at(index).fileName().replace(".", "_") + ".preview")) {
				qimage = QImage(currentDirectory.absolutePath() +"/_PhotoNinja/" + files.at(index).fileName().replace(".", "_") + ".preview");
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
		return qimage.scaledToWidth(maxsize, Qt::SmoothTransformation); // Qt::SmoothTransformation oder Qt::FastTransformation
	} else {
		return qimage.scaledToHeight(maxsize, Qt::SmoothTransformation);
	}
}
