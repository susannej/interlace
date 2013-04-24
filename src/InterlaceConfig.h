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
		int getConfLabels();
		QString getLabelColor(QString label);
		QString getLabelDesc(int no);
		QIcon getIcon4Color(QString label);
		
	private:
		static InterlaceConfig* instance;

		int imageSize;
		QStringList *prgDesc;
		QStringList *prgCommand;
		QStringList *labelDesc;
		QStringList *labelColor;

		InterlaceConfig();
		void read();
		void setPrgDesc(QString desc);
		void setPrgCommand(QString command);
		void setLabelDesc(QString desc);
		void setLabelColor(QString color);
};


#endif
