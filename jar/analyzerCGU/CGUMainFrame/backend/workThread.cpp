#include "workThread.h"
#include <QtDebug>
workThread::workThread(const void *node):_node(node)
{
}

//void callBackData(const MsgShellCMDResult &tmp)
//{
//    qDebug()<<tmp.cmd().c_str()<<":"<<tmp.moudle()<<":"<<tmp.result().c_str()<<"********************************************";
//}
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
//    void * node = const_cast<void*>(_node);

//    auto treader = static_cast<Node*>(node)->CreateReader<MsgShellCMDResultPubSubType>(Topic_ShellCMDResult, QOS_PROFILE_EVENT_LOCAL);
//    treader->SetCallBack(Callback);
//    while(1){
//        if(treader){
//            sleep(3);
//        }
//    }
}
