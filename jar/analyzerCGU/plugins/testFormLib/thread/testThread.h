#ifndef TESTTHREAD_H
#define TESTTHREAD_H
#include "baseThread.h"
#include <QtDebug>
//#include "DynamicDataApi.h"
#include <QMetaType>

class testThread : public baseThread
{
public:
    testThread();
    Q_INVOKABLE void startWork() override;
    Q_INVOKABLE void stopWork() override;
    Q_INVOKABLE void run(){};
public slots:
    Q_INVOKABLE void slotPostWork() override;
    Q_INVOKABLE void slotPostAppendData(const void *,int,int) override;
    Q_INVOKABLE void slotSubAckAppendData(const void*,int,const uint32_t &) override;//ack包，是否收取到信息
    Q_INVOKABLE void slotTest() override;

private:
    bool _isStop = false;
    bool _isRun = false;
    int _numCount = 0;
//    DynamicDataApi * _monitor = nullptr;//数据监控
    std::map<std::string,std::list<std::string>> _data;
};

#endif // TESTTHREAD_H
