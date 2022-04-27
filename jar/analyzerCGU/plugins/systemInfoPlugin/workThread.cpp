#include "workThread.h"
#include <QtDebug>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <iostream>
workThread *workThread::P_workThread = nullptr;
workThread::workThread(const void *node):_Node(node)
{
    initPoint();
}
void workThread::initPoint()
{
    P_workThread = this;
}
void workThread::callBackDataShellCMDResult(const MsgShellCMDResult &tmp)
{
    emit P_workThread->signalShellCMDResult(&tmp);

}
void workThread::callBackDataDiskMonitor(const MsgDiskMonitor &tmp)
{
    emit P_workThread->signalDataDiskMonitor(&tmp);
}
void workThread::callBackDataCPUMEMMonitor(const MsgCPUMEMMonitor &tmp)
{
    emit P_workThread->signalDataCPUMEMMonitor(&tmp);
}
void workThread::emitData(const std::string &str)
{
    emit this->signalBackendData(str);
}
Q_INVOKABLE void workThread::startWork()
{
    _isStop = false;
}
Q_INVOKABLE void workThread::stopWork()
{
    _isStop = true;
}
Q_INVOKABLE void workThread::slotDoWork()
{
    void * node = const_cast<void*>(_Node);
    //
    _MsgShellCMDResult =static_cast<Node*>(node)->CreateReader<MsgShellCMDResultPubSubType>(Topic_ShellCMDResult);
    _MsgShellCMDResult->SetCallBack(callBackDataShellCMDResult);

    //后端发送 磁盘监控
    _MsgDiskMonitor =static_cast<Node*>(node)->CreateReader<MsgDiskMonitorPubSubType>(Topic_DiskMonitor);
    _MsgDiskMonitor->SetCallBack(callBackDataDiskMonitor);

    //后端发送 CPU监控
    _MsgCPUMEMMonitor =static_cast<Node*>(node)->CreateReader<MsgCPUMEMMonitorPubSubType>(Topic_CPUMonitor);
    _MsgCPUMEMMonitor->SetCallBack(callBackDataCPUMEMMonitor);
}
