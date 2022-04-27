#include "IPC_Thread.h"
#include <QDebug>
#include <QFile>

#include "zmq_addon.hpp"
#include <QDebug>
#define ZMQ_PROXY_SUB_CHANNEL "ipc://@message_pool/sub_channel"
#define ZMQ_PROXY_PUB_CHANNEL "ipc://@message_pool/pub_channel"

#define ZMQ_ADDR_UI "ipc://@UIPub"
#define ZMQ_ADDR_CONTROL "ipc://@ControlPub"
#define ZMQ_ADDR_Gateway "ipc://@GatewayPub"

#define ZMQ_ADDR_UI_L "ipc://@UILPub"
#define ZMQ_ADDR_UI_R "ipc://@UIRPub"
#define ZMQ_ADDR_UI_M "ipc://@UIMPub"

IPC_Thread::IPC_Thread()
{

}
Q_INVOKABLE void IPC_Thread::startWork()
{
    _isStop = false;
}
Q_INVOKABLE void IPC_Thread::stopWork()
{
    _isStop = true;
}
Q_INVOKABLE void IPC_Thread::slotPostWork()
{
    QProcess process;
    while (1) {
        if(!_isStop){
            if(process.state()==QProcess::NotRunning){
                QString path = "/root/IPC_pro";
                QFile file(path);
                if(!file.exists()){
                    qDebug()<<"启动ipc绑定--------------------------------";
                    zmq::context_t ctx(1);
                    zmq::socket_t frontend(ctx, /*ZMQ_ROUTER*/ ZMQ_XSUB);
                    zmq::socket_t backend(ctx, /*ZMQ_DEALER*/ ZMQ_XPUB);
                    frontend.bind(ZMQ_PROXY_PUB_CHANNEL);//绑定发布者
                    backend.bind(ZMQ_PROXY_SUB_CHANNEL);//绑定订阅者
                    zmq::proxy(frontend, backend);
                    return;
                }
                process.start(path);
                if(process.waitForStarted()){
                    qDebug() << "IPC_Pro 启动成功"
                             <<process.readAllStandardOutput();
                }
                else {
                    qDebug() << "IPC_Pro 启动失败"<<process.readAllStandardError();
                }
            }

            else {
                qDebug() << "IPC_Pro running..."<<QThread::currentThreadId();
            }

        }
        else {
            process.close();
            process.kill();
        }
        QThread::sleep(30);
    }
}
