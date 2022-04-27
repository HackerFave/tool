#ifndef CONTROLPUB_H
#define CONTROLPUB_H

#include "msg.h"
#include "zmq/zmqmsg.hpp"
#include "zmq/zmqwrap.hpp"
#include <QObject>
#include <QTimer>
#include <atomic>
#include <cstdint>

#define CONTROLDATA_PUB_TIME_MS 50
#define LINKSTATUS_PUB_TIME_MS 1000
#define CONTROLLIGHT_PUB_TIME_MS 50

class ControlPub : public QObject
{
    Q_OBJECT
public:
    explicit ControlPub(QObject *parent = nullptr);

signals:

public slots:
    void HandleTr300rsControlDataSlot(const QByteArray &data);
    void HandleTr300rsStatusSlot(quint8 _status);
    void HandleTr300rsButtonSlot(quint32 key);

    void HandleGearShiftSlot(quint8 gear);
    void HandleGearShiftStatusSlot(quint8 _status);

    void HandleIoDevEstopCtrlSlot(quint8 ctrl);
    void HandleIoDevBucketCtrlSlot(quint8 ctrl);
    void HandleIodevStatusSlot(quint8 _status);

    void HandleZhongZhiControlDataSlot(const QByteArray &data);
    void HandleZhongZhiStatusSlot(quint8 _status);
    void HandleZhongZhiControlLightSlot(const QByteArray &data);
private slots:
    void HandlePubDataTimerOut();
    void HandlePubStatusTimerOut();
    void HandlePubLightTimerOut();

private:
    ZmqPublisher pubObj;
    QTimer pubDataTimer;
    QTimer pubStatusTimer;
    QTimer pubLightTimer;

    ControlData cacheControlData;
    std::atomic_flag controlDataFlag = ATOMIC_FLAG_INIT;

    ControlDevLinkStatus status;
    std::atomic_flag linkStatusFlag = ATOMIC_FLAG_INIT;

    uint8_t lightData[4] = {0x00};
    std::atomic_flag controlLightFlag = ATOMIC_FLAG_INIT;

};

#endif // CONTROLPUB_H
