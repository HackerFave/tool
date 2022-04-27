#include "dataInterface.h"
#include <QDebug>
#include "message_generated.h"
#include "cgu_protocol_generated.h"
#include "gps_info_generated.h"
#include "can_info_generated.h"
#include "factory/Factory.h"
dataInterface* dataInterface::P_dataInterface = nullptr;
std::mutex dataInterface::_mutex ;
using namespace gps;
using namespace topic;
dataInterface::dataInterface(QObject *parent) : baseObject(parent)
{
    _topicRecver.Subscribe("tcp/gnss/gps_info", CallbackFunc);
    _topicRecver.Subscribe("tcp/gnss/gps_status", CallbackFunc);
    _topicRecver.Subscribe("ipc/message/test_topic", CallbackFunc);
    initProduct();
}
dataInterface::~dataInterface()
{
    delInstance();
}
dataInterface* dataInterface::getInstance(){
    if (P_dataInterface == nullptr)
    {
        std::lock_guard<std::mutex> lgd(_mutex);
        if (P_dataInterface == nullptr)
        {
            P_dataInterface = new dataInterface;
        }
    }
    return P_dataInterface;
}
void dataInterface::delInstance()
{
    std::lock_guard<std::mutex> lgd(_mutex);
    if (P_dataInterface)
    {
        delete P_dataInterface;
        P_dataInterface = nullptr;
    }
}
void dataInterface::emitSignal(const void *f,int id)
{
    emit this->signalTopicData(f,id);
}

void dataInterface::CallbackFunc(const char* topic1, const void*payload, uint32_t len)//数据接受函数、、
{
    int id = enumFromString(std::string(topic1));
    if(id==0){
        return;
    }
    switch (id) {
    case Topic_ID::TOPIC_TCP_GNSS_GPS_INFO :
    {
        auto msg = GetMessage(payload);
        P_dataInterface->emitSignal(flatbuffers::GetRoot<GpsImuInfo>(msg->data()->Data()),id);
    }
        break;
    case Topic_ID::TOPIC_TCP_GNSS_GPS_STATUS :
    {
        auto msg = GetMessage(payload);
        P_dataInterface->emitSignal(flatbuffers::GetRoot<GpsStatus>(msg->data()->Data()),id);

    }
        break;
    case Topic_ID::TOPIC_IPC_MESSAGE_TEST_TOPIC :
    {
        auto msg = GetMessage(payload);
        P_dataInterface->emitSignal(flatbuffers::GetRoot<Msg8105>(msg->data()->Data()),id);

    }
        break;
    }

}
void dataInterface::initProduct()
{
//    shared_ptr<Factory> p1 (new Factory);
    Factory *p1 = new Factory;
    p1->createProductClass("dataSend");
    p1->initMethods();

}
