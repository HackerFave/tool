#include "mainwindow.h"

#include <QApplication>
#include <QDir>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QDir::setCurrent(QApplication::applicationDirPath());
    MainWindow w;
    w.showMaximized();
    return a.exec();
}
