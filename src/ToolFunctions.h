#ifndef TOOLFUNCTIONS_H

#define TOOLFUNCTIONS_H

#include <QFileInfo>
#include <QtWidgets>

#include "InterlaceConfig.h"
#include "ImageWidget.h"

int getImageOrientationFromFile(QString absoluteFilename);
void popImages(QWidget*);

#endif
