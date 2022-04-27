#include "initThread.h"
#include <stdlib.h>
#include <iostream>
#include <QDebug>
#define TIME_OUT 3
using namespace std;
#include <iostream>

initThread::initThread()
{

}
Q_INVOKABLE void initThread::startWork()
{
    _isStop = false;
}
Q_INVOKABLE void initThread::stopWork()
{
    _isStop = true;
}
Q_INVOKABLE void initThread::slotDoWork()
{
#ifdef Q_OS_LINUX
    qDebug()<<"[ 启动数据监控线程 ]";
    _monitorApi.Init();
    _monitorApi.StartRun();
    emit signalNoticeThread(&_monitorApi);
#endif
}
