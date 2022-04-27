#include "dataPost.h"
#include "define_zmqmodule.h"
#include <stdio.h>
#include <unistd.h>
#include <QtDebug>
dataPost *dataPost::P_dataPost = nullptr;

dataPost::dataPost()
{
    qRegisterMetaType<size_t>("size_t&");
    qRegisterMetaType<size_t>("size_t");
    initPoint();

}
Q_INVOKABLE void dataPost::initPoint()
{
    if(P_dataPost){
        delete P_dataPost;
        P_dataPost = this;
    }
    P_dataPost = this;
}

Q_INVOKABLE void dataPost::slotPostTopicDataChild( const void *payload, size_t payload_len,int id)
{

    Topic_ID flag = static_cast<Topic_ID>(id);
    _sender.Post(enumToOtherString(flag), payload, payload_len);
    emit this->signalPostStartThread();//启动post轮寻线程
    emit this->signalPostAppendData(payload, payload_len,id);
}
