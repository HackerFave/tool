#include "mainForm.h"
#include <QApplication>
#include <QDir>
#include <QMetaType>
#include "dds_wrapper/include/node.hpp"

#ifdef Q_OS_WIN

#endif

#ifdef Q_OS_LINUX
#include "QBreakpadHandler.h"
#endif

#ifdef Q_OS_MAC

#endif
void initRegisterMetaType()
{
    qRegisterMetaType<std::string>("std::string");
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString libPath = QApplication::applicationDirPath();
    QDir::setCurrent(libPath);
    initRegisterMetaType();
#ifdef Q_OS_LINUX
    QBreakpadInstance.setDumpPath("crashes"); // 设置生成dump文件路径
#endif
    mainForm w;
    w.showMaximized();
    return a.exec();
}
