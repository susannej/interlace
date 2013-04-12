#ifndef INTERLACECONFIG_H

#define INTERLACECONFIG_H

#include <QtGui>

class InterlaceConfig
{
	public:
		static InterlaceConfig* getInstance();
		~InterlaceConfig();
		
		int getConfProgramms();
		QString getPrgDesc(int no);
		QString getPrgCommand(int no);
		int getImageSize();
		
	private:
		static InterlaceConfig* instance;

		int imageSize;
		QStringList *prgDesc;
		QStringList *prgCommand;

		InterlaceConfig();
		void read();
		void setPrgDesc(QString desc);
		void setPrgCommand(QString command);
};


#endif
