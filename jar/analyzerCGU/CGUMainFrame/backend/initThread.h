#ifndef INITTHREAD_H
#define INITTHREAD_H
#include "baseInterface/baseThread.h"
#ifdef Q_OS_LINUX
#include "DynamicDataApi.h"
#endif
class initThread : public baseThread
{
public:
    initThread();
    Q_INVOKABLE void startWork() override;
    Q_INVOKABLE void stopWork() override;
    Q_INVOKABLE void run(){};
public slots:
    Q_INVOKABLE void slotDoWork() override;
signals:
private:
    bool _isStop = false;
    bool _isRun = false;
 #ifdef Q_OS_LINUX
    DynamicDataApi _monitorApi;
 #endif
};

#endif // INITTHREAD_H
