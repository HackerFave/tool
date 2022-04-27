#include "../include/iodev.h"
#include "mydebug.h"
#include "zmq/zmqmsg.hpp"
#include <QDebug>
#include <QFileInfo>
#include <QSerialPort>
#include <QThread>
#include <QVariant>

IODev::IODev(QObject *parent) : QObject(parent)
{
    StartTimer(READ_IOSTATE_TIMER_OUT_MS);
    connect(&readTimer, &QTimer::timeout, this, &IODev::HandleTimeOut);
}

IODev::~IODev()
{
    DisConnectIoDev();
}

void IODev::Init()
{
    iodev = new QModbusRtuSerialMaster(this);
    SetIODevConfig();
    bool ret = iodev->connectDevice();
    MY_DEBUG() << "Connect to ioDev result:" << ret << " , " << iodev->errorString();
    StartTimer(10);
}

void IODev::StartTimer(int ms)
{
    readTimer.setSingleShot(true);
    readTimer.start(ms);
}

bool IODev::DevExist()
{
    return QFileInfo::exists(iodevPath);
}

void IODev::StartRecvRawData()
{
    if (DevExist())
    {
        if (iodev == nullptr)
        {
            Init();
        }
        else
        {
            ReadIOState();
        }
    }
    else
    {
        emit EmitIoDevStatus(DEV_LINKSTATUS_DISCONNECTED);
        DisConnectIoDev();
        StartTimer(3000);
        MY_DEBUG() << "Wait for IO Device";
    }
}

QModbusDataUnit IODev::ReadIOStateRequest() const
{
    return QModbusDataUnit(QModbusDataUnit::RegisterType::DiscreteInputs,
                           IODEV_IOSTATE_START_ADDR, IODEV_IOSTATE_LENGTH);
}

void IODev::SetIODevConfig()
{
    iodev->setConnectionParameter(QModbusDevice::SerialPortNameParameter, iodevPath);
    iodev->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, QSerialPort::Baud9600);
    iodev->setConnectionParameter(QModbusDevice::SerialParityParameter, QSerialPort::NoParity);
    iodev->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, QSerialPort::Data8);
    iodev->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, QSerialPort::OneStop);
    iodev->setTimeout(100);
}

void IODev::ReadIOState()
{
    MY_DEBUG() << "sendReadRequest";
    if (iodev->state() == QModbusDevice::ConnectedState)
    {
        if (auto *reply = iodev->sendReadRequest(ReadIOStateRequest(), IODEV_MODBUS_SERVER_ADDR))
        {
            if (!reply->isFinished())
            {
                connect(reply, SIGNAL(finished()), this, SLOT(HandleReadIOStateReady()));
            }
            else
            {
                delete reply; // broadcast replies return immediately
            }
        }
        else
        {
            MY_DEBUG() << " sendReadRequest error:" << iodev->errorString();
        }
    }
}

void IODev::DisConnectIoDev()
{
    if (iodev != nullptr)
    {
        iodev->disconnectDevice();
    }
    delete iodev;
    iodev = nullptr;
}

void IODev::HandleEstopCtrl(const QModbusDataUnit &unit)
{
    quint8 eStop = static_cast<quint8>(unit.value(EMERGENCY_STOP_INDEX));
    emit EmitEstopCtrl(eStop);
    MY_DEBUG() << "Get eStop:" << eStop;
}

void IODev::HandleBucketCtrl(const QModbusDataUnit &unit)
{
    quint8 buckCtrl = BUCKET_CTRL_HOLDON;
    if (unit.value(BUCKET_CTRL_UP_INDEX) == 1)
    {
        buckCtrl = BUCKET_CTRL_UP;
    }
    else if (unit.value(BUCKET_CTRL_DOWN_INDEX) == 1)
    {
        buckCtrl = BUCKET_CTRL_DOWN;
    }
    //    else if (unit.value(BUCKET_CTRL_HOLD_INDEX) == 1)
    //    {
    //        buckCtrl = BUCKET_CTRL_HOLDON;
    //    }
    else
    {
        buckCtrl = BUCKET_CTRL_HOLDON;
    }
    emit EmitBucketCtrl(buckCtrl);
    MY_DEBUG() << "Get BucketCtrl:" << buckCtrl;
}

void IODev::HandleTrumpet(const QModbusDataUnit &unit)
{
    if (unit.value(TRUMPET_LEFT_INDEX) == 1)
    {
        MY_DEBUG() << "Get Trumpet left ON";
    }
    else
    {
        MY_DEBUG() << "Get Trumpet left OFF";
    }
    if (unit.value(TRUMPET_RIGHT_INDEX) == 1)
    {
        MY_DEBUG() << "Get Trumpet Right ON";
    }
    else
    {
        MY_DEBUG() << "Get Trumpet Right OFF";
    }
}

void IODev::HandleReadIOStateReady()
{
    qDebug() << "HandleReadIOStateReady";
    StartTimer(10);
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError)
    {
        emit EmitIoDevStatus(DEV_LINKSTATUS_CONNECTED_DATA);

        const QModbusDataUnit unit = reply->result();
        HandleEstopCtrl(unit);
        HandleBucketCtrl(unit);
        HandleTrumpet(unit);
    }
    else
    {
        MY_DEBUG() << "Read IOState Error:" << reply->errorString();
        emit EmitIoDevStatus(DEV_LINKSTATUS_CONNECTED_NO_DATA);
    }
    reply->deleteLater();
}

void IODev::HandleTimeOut()
{
    StartRecvRawData();
}
