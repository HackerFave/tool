#ifndef DATAINTERFACE_H
#define DATAINTERFACE_H

#include "baseObject.h"
#include "node.h"
#include <mutex>
#include <QVariantMap>
#include "topicCommon.h"
class dataInterface : public baseObject
{
    Q_OBJECT
public:
private:
    explicit dataInterface(QObject *parent = nullptr);
    dataInterface & operator=(const dataInterface & ) = delete;
    dataInterface(const dataInterface &) = delete;
    void initProduct();
public:
    ~dataInterface();
    static dataInterface* getInstance();
    static void delInstance();
    // 设置回调函数的函数
    static void CallbackFunc(const char* topic, const void*payload, uint32_t len);//数据接受函数、、
    void emitSignal(const void *,int id);
signals:
    void signalTopicData(const void*,int id);//数据发送信号
private slots:
private:
    static dataInterface *P_dataInterface;
    static std::mutex _mutex;
    datax::Node _topicRecver;

};

#endif // DATAINTERFACE_H
