#include "dataInfoThread.h"
#define TIME_OUT 3
#include <iostream>
#include <QDebug>
#include <stdlib.h>
#include <string>
#include <unistd.h>
dataInfoThread* dataInfoThread::P_dataInfoThread = nullptr;
dataInfoThread::dataInfoThread()
{
    initPoint();
}
void dataInfoThread::initPoint()
{
    P_dataInfoThread = this;
}
void dataInfoThread::callBackData(const std::string &str)
{
#ifdef Q_OS_LINUX
    //    char *tmp =new char[strlen(str.c_str())+1];
    //    strcpy(tmp, str.c_str());
    P_dataInfoThread->emitData(str);
    //    delete[] tmp;
#endif
}
void dataInfoThread::emitData(const std::string &str)
{

    emit this->signalBackendData(str);
}

Q_INVOKABLE void dataInfoThread::startWork()
{
    _isStop = false;
}
Q_INVOKABLE void dataInfoThread::stopWork()
{
    _isStop = true;
}
Q_INVOKABLE void dataInfoThread::slotDoWork()
{

}
Q_INVOKABLE void dataInfoThread::slotDoWork(void *monitor)
{
    qDebug()<<"[ 数据转发服务启动 ]";
#ifdef Q_OS_LINUX
    static_cast<DynamicDataApi*>(monitor)->RegisterCallBack(callBackData);
#endif
}

