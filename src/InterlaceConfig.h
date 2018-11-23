#ifndef INTERLACECONFIG_H

#define INTERLACECONFIG_H

#include <QtWidgets>

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
		QString getLabelColor(int no);
		int getLabelIndex(QString label);
		QString getLabelDesc(int no);
		QIcon getIcon4Color(QString label);
		QString getStartDirectory();
		
	private:
		static InterlaceConfig* instance;

		int imageSize;
		QString *startDirectory;
		QStringList *prgDesc;
		QStringList *prgCommand;
		QStringList *labelDesc;
		QStringList *labelColor;
		QVector<QIcon> *labelIcons;

		InterlaceConfig();
		void read();
		void setPrgDesc(QString desc);
		void setPrgCommand(QString command);
		void setLabelDesc(QString desc);
		void setLabelColor(QString color);
};


#endif
