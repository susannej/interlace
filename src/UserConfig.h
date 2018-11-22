#ifndef USERCONFIG_H

#define USERCONFIG_H

#include <QtWidgets>

class UserConfig
{
	public:
		static UserConfig* getInstance();
		~UserConfig();
		
	private:
		static UserConfig* instance;

		/*
		int imageSize;
		QStringList *prgDesc;
		QStringList *prgCommand;
		QStringList *labelDesc;
		QStringList *labelColor;
		QVector<QIcon> *labelIcons;
		*/

		UserConfig();
		void read();
		int write();
};


#endif
