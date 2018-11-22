#include "UserConfig.h"

UserConfig* UserConfig::instance = NULL;

UserConfig::UserConfig() {
	/*
	imageSize = 200;
	prgDesc = new QStringList();
	prgCommand = new QStringList();
	labelDesc = new QStringList();
	labelColor = new QStringList();
	labelIcons = new QVector<QIcon>();
	QPixmap pixmap(16, 16);
	pixmap.fill(Qt::transparent);
	labelIcons->append(pixmap);
	*/
}

UserConfig::~UserConfig() {
}

UserConfig* UserConfig::getInstance() {
	if (instance == NULL) {
		instance = new UserConfig();
		instance->read();
	}
	return instance;
}

void UserConfig::read() {
	QFile file(QDir::homePath() + "/InterlaceUser.cfg");
	if (file.open(QIODevice::ReadOnly)) {
		QString line;
		while( !file.atEnd() ) {
			line = (file.readLine()).trimmed();

			if (line.startsWith("Prg")) {

				QStringList list1 = line.split("=");
				QStringList list2 = list1.at(1).split(";");

				//setPrgDesc(list2.at(0));
				//setPrgCommand(list2.at(1));
			} else if (line.startsWith("ImageSize")) {
				QStringList list1 = line.split("=");
				//imageSize = list1.at(1).toInt();
			} else if (line.startsWith("Label")) {
				QStringList list1 = line.split("=");
				QStringList list2 = list1.at(1).split(";");

				//setLabelDesc(list2.at(0));
				//setLabelColor(list2.at(1));

				//QPixmap pixmap(16, 16);
				//pixmap.fill(QColor(list2.at(1)));
				//labelIcons->append(pixmap);
			}
			
			qDebug()<<line;
		}
		file.close();
	}	
}

int UserConfig::write() {
}
