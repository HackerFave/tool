#include "QtSimpleMap.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtSimpleMap w;
	w.setGeometry(0, 0, 1024, 1024);
    w.show();
    return a.exec();
}
