#include "qtimagedisplay.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTImageDisplay w;
	w.show();
	return a.exec();
}
