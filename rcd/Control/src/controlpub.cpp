#include "../include/controlpub.h"
#include <qnamespace.h>

ControlPub::ControlPub(QObject *parent) : QObject(parent), pubObj(ZMQ_ADDR_CONTROL)
{
    //std::memmove(&cacheControlData, 0x00, sizeof(cacheControlData));
    cacheControlData.brakePedal = 10000;
    cacheControlData.hydraulicBrake = 10000;
    cacheControlData.parkCtrl = 1;
    //std::memmove(&status, 0x00, sizeof(status));

    pubDataTimer.setInterval(CONTROLDATA_PUB_TIME_MS);
    connect(&pubDataTimer, SIGNAL(timeout()), this, SLOT(HandlePubDataTimerOut()));
    pubDataTimer.start();

    pubStatusTimer.setInterval(LINKSTATUS_PUB_TIME_MS);
    connect(&pubStatusTimer, SIGNAL(timeout()), SLOT(HandlePubStatusTimerOut()));
    pubStatusTimer.start();

    pubLightTimer.setInterval(CONTROLLIGHT_PUB_TIME_MS);
    connect(&pubLightTimer, SIGNAL(timeout()), this, SLOT(HandlePubLightTimerOut()));
    pubLightTimer.start();
}

void ControlPub::HandleTr300rsControlDataSlot(const QByteArray &data)
{
    ControlData tmp;
    std::memset(&tmp, 0x00, sizeof(ControlData));
    std::memmove(&tmp, data.data(), data.size());
    while (controlDataFlag.test_and_set(std::memory_order::memory_order_acquire))
        ;
    cacheControlData.steeringWheelAngle = tmp.steeringWheelAngle; //方向盘
    cacheControlData.throttlePedal = tmp.throttlePedal;           //油门
    cacheControlData.brakePedal = tmp.brakePedal;                 //电制动
    cacheControlData.hydraulicBrake = tmp.hydraulicBrake;         //机械制动
    controlDataFlag.clear(std::memory_order::memory_order_release);
}

void ControlPub::HandleTr300rsStatusSlot(quint8 _status)
{
    while (linkStatusFlag.test_and_set(std::memory_order::memory_order_acquire))
        ;
    status.wheel = _status;
    linkStatusFlag.clear(std::memory_order::memory_order_release);
}

void ControlPub::HandleTr300rsButtonSlot(quint32 key)
{
    KeyButton tmp = {Qt::Key_Any};
    tmp.key = key;
    pubObj.Publish(ZMQ_TOPIC_BUTTON, &tmp, sizeof(tmp));
}

void ControlPub::HandleGearShiftSlot(quint8 gear)
{
    while (controlDataFlag.test_and_set(std::memory_order::memory_order_acquire))
        ;
    cacheControlData.gearShiftCtrl = gear;
    controlDataFlag.clear(std::memory_order::memory_order_release);
}

void ControlPub::HandleGearShiftStatusSlot(quint8 _status)
{
    while (linkStatusFlag.test_and_set(std::memory_order::memory_order_acquire))
        ;
    status.gearShift = _status;
    linkStatusFlag.clear(std::memory_order::memory_order_release);
}

void ControlPub::HandleIoDevEstopCtrlSlot(quint8 ctrl)
{
    while (controlDataFlag.test_and_set(std::memory_order::memory_order_acquire))
        ;
    cacheControlData.eStop = ctrl;
    controlDataFlag.clear(std::memory_order::memory_order_release);
}

void ControlPub::HandleIoDevBucketCtrlSlot(quint8 ctrl)
{
    while (controlDataFlag.test_and_set(std::memory_order::memory_order_acquire))
        ;
    cacheControlData.bucketCtrl = ctrl;
    controlDataFlag.clear(std::memory_order::memory_order_release);
}

void ControlPub::HandleIodevStatusSlot(quint8 _status)
{
    while (linkStatusFlag.test_and_set(std::memory_order::memory_order_acquire))
        ;
    status.bucket = _status;
    linkStatusFlag.clear(std::memory_order::memory_order_release);
}

void ControlPub::HandleZhongZhiControlDataSlot(const QByteArray &data)
{
    while (controlDataFlag.test_and_set(std::memory_order::memory_order_acquire))
        ;
    std::memmove(&cacheControlData, data.data(), data.size());
    controlDataFlag.clear(std::memory_order::memory_order_release);
}

void ControlPub::HandleZhongZhiControlLightSlot(const QByteArray& data) {
    while (controlLightFlag.test_and_set(std::memory_order::memory_order_acquire))
        ;
    std::memmove(&lightData, data.data(), data.size());
    controlLightFlag.clear(std::memory_order::memory_order_release);
}

void ControlPub::HandleZhongZhiStatusSlot(quint8 _status)
{
    while (linkStatusFlag.test_and_set(std::memory_order::memory_order_acquire))
        ;
    status.bucket = _status;
    status.wheel = _status;
    status.gearShift = _status;
    linkStatusFlag.clear(std::memory_order::memory_order_release);
}

void ControlPub::HandlePubDataTimerOut()
{
    ControlData tmp;
    std::memset(&tmp, 0x00, sizeof(ControlData));
    while (controlDataFlag.test_and_set(std::memory_order::memory_order_acquire))
        ;
    std::memmove(&tmp, &cacheControlData, sizeof(cacheControlData));
    controlDataFlag.clear(std::memory_order::memory_order_release);
    auto curTimePoint = std::chrono::steady_clock::now();
    auto curMs = std::chrono::time_point_cast<std::chrono::milliseconds>(curTimePoint);
    tmp.utcTime = curMs.time_since_epoch().count();
    pubObj.Publish(ZMQ_TOPIC_CONTROL_DATA, &tmp, sizeof(tmp));
}

void ControlPub::HandlePubLightTimerOut() {
    uint8_t tmp[4] = {0x00};
    while (controlLightFlag.test_and_set(std::memory_order::memory_order_acquire))
        ;
    std::memmove(&tmp, &lightData, sizeof(tmp));
    controlLightFlag.clear(std::memory_order::memory_order_release);
    pubObj.Publish(ZMQ_TOPIC_CONTROL_LIGHT, &tmp, sizeof(tmp));
}

void ControlPub::HandlePubStatusTimerOut()
{
    ControlDevLinkStatus tmp;
    std::memset(&tmp, 0x00, sizeof(ControlDevLinkStatus));
    while (linkStatusFlag.test_and_set(std::memory_order::memory_order_acquire))
        ;
    std::memmove(&tmp, &status, sizeof(tmp));
    linkStatusFlag.clear(std::memory_order::memory_order_release);
    pubObj.Publish(ZMQ_TOPIC_CONTROL_DEVLINKSTATUS, &tmp, sizeof(tmp));
}
