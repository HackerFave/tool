#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include "baseThread.h"
#include <QMap>
#include <QTimer>
#include <QVariantList>
#include <QDebug>
//#include "DynamicDataApi.h"

class workThread : public baseThread
{
public:
    workThread();
    Q_INVOKABLE void startWork() override;
    Q_INVOKABLE void stopWork() override;
    Q_INVOKABLE void run(){};

public slots:
    Q_INVOKABLE void slotPostWork() override;
    Q_INVOKABLE void slotPostAppendData(const void *,int,int) override;
    Q_INVOKABLE void slotSubAckAppendData(const void*,int,const uint32_t &) override;//ack包，是否收取到信息
    Q_INVOKABLE void slotTest() override;
private slots:
private:
    bool _isStop = false;
    bool _isRun = false;
//    DynamicDataApi *_dataApi = nullptr;
};

#endif // WORKTHREAD_H
