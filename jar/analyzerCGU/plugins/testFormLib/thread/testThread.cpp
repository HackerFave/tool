#include "testThread.h"
#include <QDebug>
#define TIME_OUT 3

testThread::testThread()
{
//   _monitor = new DynamicDataApi;
}
Q_INVOKABLE void testThread::startWork()
{
    _isStop = false;
}
Q_INVOKABLE void testThread::stopWork()
{
    _isStop = true;
}
Q_INVOKABLE void testThread::slotPostWork()
{
    //    qDebug()<<"启动数据监控："<<QThread::currentThreadId();
    while (1)
    {
        if(_isStop)
            return;
//        INFODATA SS = _test->m_listener.getInfoMap();
//        emit signalDataInfo(SS,_test->m_listener.getParticipantInfo());
//        emit signalTest("QString");
//        _monitor->Print();
        std::string str;
//        _monitor->GetJsonString(str);
//        qDebug()<<str.c_str()<<"KKKKKKKKKKKKKKKKKKKKK";
        QThread::sleep(TIME_OUT);
    }
}
Q_INVOKABLE void testThread::slotPostAppendData(const void * f,int len,int id)
{


}
Q_INVOKABLE void testThread::slotSubAckAppendData(const void*f,int id,const uint32_t &len)
{

}
Q_INVOKABLE void testThread::slotTest()
{
    qDebug()<<"fuck fuck fuck *******************************";
}
