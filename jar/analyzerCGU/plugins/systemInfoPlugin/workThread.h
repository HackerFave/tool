#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include "thread/baseThread.h"
#include "dds_wrapper/include/node.hpp"
#include "idl/systemInfo.h"
#include "idl/systemInfoPubSubTypes.h"
#include "idl/systemInfoTypeObject.h"
using namespace tg_msgs::dds;
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
    static void callBackDataShellCMDResult(const MsgShellCMDResult &tmp);
    static void callBackDataDiskMonitor(const MsgDiskMonitor &tmp);
    static void callBackDataCPUMEMMonitor(const MsgCPUMEMMonitor &tmp);
    void emitData(const std::string &str);
    void initPoint();
private:
    bool _isStop = false;
    bool _isRun = false;
    const void * _Node =nullptr;
    //    Node * _nodetest = nullptr;
    static workThread *P_workThread;
    std::shared_ptr<TGSub<MsgShellCMDResult>> _MsgShellCMDResult;
    std::shared_ptr<TGSub<MsgDiskMonitor>> _MsgDiskMonitor;
    std::shared_ptr<TGSub<MsgCPUMEMMonitor>> _MsgCPUMEMMonitor;
};

#endif // WORKTHREAD_H
