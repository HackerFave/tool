#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include "baseInterface/baseThread.h"
#include "./dds_wrapper/include/node.hpp"

class workThread : public baseThread
{
public:
    workThread( const void *node =nullptr);
    Q_INVOKABLE void startWork() override;
    Q_INVOKABLE void stopWork() override;
    Q_INVOKABLE void run(){};
public slots:
    Q_INVOKABLE void slotDoWork() override;
    //    Q_INVOKABLE void slotDoWork(void *monitor) override;
private:
    //    static void callBackData(const MsgShellCMDResult &tmp);
    void emitData(const std::string &str);
    void initPoint();
private:
    bool _isStop = false;
    bool _isRun = false;
    const void *_node = nullptr;
};

#endif // WORKTHREAD_H
