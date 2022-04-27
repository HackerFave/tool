#ifndef IODEV_H
#define IODEV_H

#include <QModbusRtuSerialMaster>
#include <QObject>
#include <QTimer>

#define IODEV_DEFAULT_PATH "/dev/ttyUSB0"

#define READ_IOSTATE_TIMER_OUT_MS 30

#define IODEV_MODBUS_SERVER_ADDR 1

//获取IO状态的寄存器起始地址和长度
#define IODEV_IOSTATE_START_ADDR 100
#define IODEV_IOSTATE_LENGTH 16

//手刹IO索引
#define PARK_CTRL_INDEX 10

//货箱控制-上升索引
#define BUCKET_CTRL_UP_INDEX 13
//货箱控制-保持/不动作
//#define BUCKET_CTRL_HOLD_INDEX 13
//货箱控制-下降索引
#define BUCKET_CTRL_DOWN_INDEX 15

//左转向
#define TRUMPET_LEFT_INDEX 12
//右转向
#define TRUMPET_RIGHT_INDEX 11

//急停索引
#define EMERGENCY_STOP_INDEX 4

class IODev : public QObject
{
    Q_OBJECT
public:
    explicit IODev(QObject *parent = nullptr);
    ~IODev();
    void StartRecvRawData();

private:
    void Init();
    void StartTimer(int ms);
    inline bool DevExist();
    QModbusDataUnit ReadIOStateRequest() const;
    void SetIODevConfig();
    void ReadIOState();
    void DisConnectIoDev();

    inline void HandleEstopCtrl(const QModbusDataUnit &unit);
    inline void HandleBucketCtrl(const QModbusDataUnit &unit);
    inline void HandleTrumpet(const QModbusDataUnit &unit);

private slots:
    void HandleReadIOStateReady();
    void HandleTimeOut();

signals:
    void EmitEstopCtrl(quint8 ctrl);
    void EmitBucketCtrl(quint8 ctrl);
    void EmitIoDevStatus(quint8 status);

private:
    QModbusRtuSerialMaster *iodev = nullptr;
    QString iodevPath = IODEV_DEFAULT_PATH;
    QTimer readTimer;
};

#endif // IODEV_H
