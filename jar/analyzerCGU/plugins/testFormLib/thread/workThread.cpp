#include "workThread.h"


#define TIME_OUT 3

workThread::workThread()
{
//    _dataApi = new DynamicDataApi;
}
Q_INVOKABLE void workThread::startWork()
{
    _isStop = false;
}
Q_INVOKABLE void workThread::stopWork()
{
    _isStop = true;
}
Q_INVOKABLE void workThread::slotPostWork()
{
    qDebug()<<"启动数据监控："<<QThread::currentThreadId();
    while (1)
    {
        if(_isStop)
            return;
        else {
            if(!_isRun){
                _isRun = true;
//                _dataApi->Init();
//                _dataApi->StartRun();
            }
        }
        QThread::sleep(TIME_OUT);
    }
}
Q_INVOKABLE void workThread::slotPostAppendData(const void * f,int len,int id)
{


}
Q_INVOKABLE void workThread::slotSubAckAppendData(const void*f,int id,const uint32_t &len)
{

}
Q_INVOKABLE void workThread::slotTest()
{

}
