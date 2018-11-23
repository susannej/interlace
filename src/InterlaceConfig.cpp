#include "InterlaceConfig.h"

InterlaceConfig* InterlaceConfig::instance = NULL;

InterlaceConfig::InterlaceConfig() {
	imageSize = 200;
	prgDesc = new QStringList();
	prgCommand = new QStringList();
	labelDesc = new QStringList();
	labelColor = new QStringList();
	labelIcons = new QVector<QIcon>();
	QPixmap pixmap(16, 16);
	pixmap.fill(Qt::transparent);
	labelIcons->append(pixmap);
	startDirectory = new QString("");
}

InterlaceConfig::~InterlaceConfig() {
}

InterlaceConfig* InterlaceConfig::getInstance() {
	if (instance == NULL) {
		instance = new InterlaceConfig();
		instance->read();
	}
	return instance;
}

QString InterlaceConfig::getPrgDesc(int no) {
	if (no < prgDesc->size())
		return prgDesc->at(no);
	return NULL;
}

QString InterlaceConfig::getPrgCommand(int no) {
	if (no < prgCommand->size())
		return prgCommand->at(no);
	return NULL;
}

int InterlaceConfig::getConfProgramms() {
	return prgDesc->size();
}

void InterlaceConfig::read() {
	QFile file(QDir::homePath() + "/Interlace.cfg");
	if (file.open(QIODevice::ReadOnly)) {
		QString line;
		while( !file.atEnd() ) {
			line = (file.readLine()).trimmed();

			if (line.startsWith("Prg")) {

				QStringList list1 = line.split("=");
				QStringList list2 = list1.at(1).split(";");

				setPrgDesc(list2.at(0));
				setPrgCommand(list2.at(1));
			} else if (line.startsWith("ImageSize")) {
				QStringList list1 = line.split("=");
				imageSize = list1.at(1).toInt();
			} else if (line.startsWith("Label")) {
				QStringList list1 = line.split("=");
				QStringList list2 = list1.at(1).split(";");

				setLabelDesc(list2.at(0));
				setLabelColor(list2.at(1));

				QPixmap pixmap(16, 16);
				pixmap.fill(QColor(list2.at(1)));
				labelIcons->append(pixmap);
			} else if (line.startsWith("StartDirectory")) {
				QStringList list1 = line.split("=");
				*startDirectory = list1.at(1);
			}
			
			qDebug()<<line;
		}
		file.close();
	}	
}

void InterlaceConfig::setPrgDesc(QString desc) {
	prgDesc->append(desc);
}

void InterlaceConfig::setPrgCommand(QString command) {
	prgCommand->append(command);
}

int InterlaceConfig::getImageSize() {
	return imageSize;
}

int InterlaceConfig::getConfLabels() {
	return labelDesc->size();
}

QString InterlaceConfig::getLabelColor(QString label) {
	for (int i = 0; i < labelDesc->size(); i++) {
		if (labelDesc->at(i) == label) {
			return labelColor->at(i);
		}
	}
	return "";
}

QString InterlaceConfig::getLabelColor(int no) {
	return labelColor->at(no);
}

int InterlaceConfig::getLabelIndex(QString label) {
	for (int i = 0; i < labelDesc->size(); i++) {
		if (labelDesc->at(i) == label) {
			return i;
		}
	}
	return -1;
}

void InterlaceConfig::setLabelDesc(QString desc) {
	labelDesc->append(desc);
}

void InterlaceConfig::setLabelColor(QString color) {
	labelColor->append(color);
}

QString InterlaceConfig::getLabelDesc(int no) {
	return labelDesc->at(no);
}

QIcon InterlaceConfig::getIcon4Color(QString label) {
	return labelIcons->at(getLabelIndex(label) +1);
}

QString InterlaceConfig::getStartDirectory() {
	return *startDirectory;
}
