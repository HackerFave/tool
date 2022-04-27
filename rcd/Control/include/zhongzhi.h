#ifndef ZHONGZHI_H
#define ZHONGZHI_H

#include "zmq/zmqmsg.hpp"
#include "zmq/zmqwrap.hpp"
#include "can/canraw.hpp"
#include <QByteArray>
#include <QMap>
#include <QObject>
#include <QSerialPort>
#include <QString>
#include <QThread>
#include <QTimer>

//离合行程
#define CLUTCH_MIN 0x00af
#define CLUTCH_MAX 0x022a

//刹车踏板行程
#define BRAKE_MIN 0x01cf
#define BRAKE_MAX 0x0330

//油门踏板行程
#define ACC_MIN 0x0095
#define ACC_MAX 0x031F

//手刹行程
#define HANDBRAKE_MIN 0x01d1
#define HANDBRAKE_MAX 0x0250

//方向盘行程 steeringwheel
#define STEERINGWHEEL_LEFT_MAX 0x218F
#define STEERINGWHEEL_MEDIAN 0x0000
#define STEERINGWHEEL_RIGHT_MIN 0xde00
#define STEERINGWHEEL_RIGHT_OFFSET 0xde70
#define STEERINGWHEEL_RIGHT_MAX 0xFFFF

//挡位定义
#define GEAR_MASK 0x0F
#define GEAR_N 0x01
#define GEAR_D 0x02
#define GEAR_R 0x03
#define GEAR_P 0x04

//后视镜按键定义
#define MIRROR_DIR_MASK 0xF0
#define MIRROR_DIR_NO 0x00
#define MIRROR_DIR_UP 0x10
#define MIRROR_DIR_DOWN 0x20
#define MIRROR_DIR_LEFT 0x30
#define MIRROR_DIR_RIGHT 0x40
#define MIRROR_SELECT_MASK 0x0F
#define MIRROR_SELECT_NO 0x03
#define MIRROR_SELECT_L 0x01
#define MIRROR_SELECT_R 0x02

//key_1
#define KEY_1_BIT0_ENGINE 0        //点火
#define KEY_1_BIT1_ON 1            //ON
#define KEY_1_BIT2_ACC 2           //ACC
#define KEY_1_BIT3_SPEAKER 3       //喇叭
#define KEY_1_BIT4_SEAT_BELT 4     //安全带
#define KEY_1_BIT5_WARN_LIGHT 5    //双闪
#define KEY_1_BIT6_LOOP_INTERNAL 6 //内循环
#define KEY_1_BIT7_TRUNK 7         //后备箱

//key_2
#define KEY_2_BIT0_HIGH_BEAM 0   //远光灯
#define KEY_2_BIT1_LEFT_BEAM 1   //左转灯
#define KEY_2_BIT2_RIGHT_BEAM 2  //右转灯
#define KEY_2_BIT3_AC 3          //空调
#define KEY_2_BIT4_MIRROR_HEAT 4 //后视镜加热按键
#define KEY_2_BIT5_WIDTH_LIGHT 5 //示宽灯
#define KEY_2_BIT6_LOW_BEAM 6    //近光灯
#define KEY_2_BIT7_FOG_LIGHT 7   //雾灯

#pragma pack(1)
typedef struct
{
    uint16_t clutch;        //离合
    uint16_t brake;         //刹车
    uint16_t accelerator;   //油门
    uint16_t handbrake;     //手刹
    uint16_t steeringwheel; //方向盘
    uint8_t gear;           //挡位
    uint8_t mirror;         //后视镜按键
    uint8_t key_1;          //
    uint8_t key_2;          //
    uint8_t unused[5];
} MainCtrlMsg;
#pragma pack()

#define HEART_BEAT_TIME_MS 1000

class zhongzhi : public QObject
{
    Q_OBJECT
public:
    explicit zhongzhi(QObject *parent = nullptr);
    virtual ~zhongzhi();
    void StartRun();

signals:
    void EmitControlData(const QByteArray &data);
    void EmitStatus(quint8 status);
    void EmitButton(quint32 dirKey);
    void EmitControlLight(const QByteArray& data);

public slots:
    void HandleMeterHeartBeatTimerSlot();
    void HandleMeterDataTimerSlot();

private:
    void InitUart();
    void InitMeterUart();
    void HandleUartData();
    void StartLoop();
    void SetContronData(const MainCtrlMsg &msg, ControlData &data);
    void HandleButton(const MainCtrlMsg &msg);
    void HandleZmqVehicleRawData(std::string &topic, const void *data, int len);

private:
    QSerialPort uart;
    QByteArray  uartBuf;
    QThread *   readT;
    ControlData cacheControlData;
    uint8_t cacheLightData[4] = {0x00};

    QSerialPort uartMeter;
    ZmqSubscriber subObj;
    QTimer meterHeartBeatTimer;
    QTimer meterDataTimer;
    QTimer controlDataTImer;
    float speed = 0.0;       //km/h
    float engineSpeed = 0.0; //1rpm

    // tl can data struct
    TL0x0C300021Struct tl0x0c300021St;
    TL0x0C303021Struct tl0x0c303021St;
    TL0x1830A021Struct tl0x1830a021St;
    TL0x1830A321Struct tl0x1830a321St;
    TL0x1830B021Struct tl0x1830b021St;

    const char heartBeat[5] = {static_cast<char>(0xaa),
                               static_cast<char>(0xff),
                               static_cast<char>(0xff),
                               0x00,
                               0x55};
    const char meterHeartBeat[5] = {static_cast<char>(0xaa),
                                    static_cast<char>(0xff),
                                    static_cast<char>(0xff),
                                    0x02,
                                    0x55};
    const char controlHeartBeat[5] = {static_cast<char>(0xaa),
                                      static_cast<char>(0xff),
                                      static_cast<char>(0xff),
                                      0x01,
                                      0x55};


    const int steeringwheelRightTotal = static_cast<uint16_t>(STEERINGWHEEL_RIGHT_MAX)
                                        - static_cast<uint16_t>(STEERINGWHEEL_RIGHT_OFFSET);
    const int steeringwheelLeftTotal = static_cast<uint16_t>(STEERINGWHEEL_LEFT_MAX);
    const int steeringwheelTotal     = steeringwheelLeftTotal + steeringwheelRightTotal;

    const int clutchTotal = static_cast<uint16_t>(CLUTCH_MAX) - static_cast<uint16_t>(CLUTCH_MIN);
    const int brakeTotal  = static_cast<uint16_t>(BRAKE_MAX) - static_cast<uint16_t>(BRAKE_MIN);
    const int accTotal    = static_cast<uint16_t>(ACC_MAX) - static_cast<uint16_t>(ACC_MIN);
    const int handbrakeTotal = static_cast<uint16_t>(HANDBRAKE_MAX)
                               - static_cast<uint16_t>(HANDBRAKE_MIN);
    const QMap<uint8_t, uint8_t> zhongzhiGearMap
        = {{GEAR_D, GEARSHIFT_CTRL_D},
           {GEAR_N, GEARSHIFT_CTRL_N},
           {GEAR_P, GEARSHIFT_CTRL_N},
           {GEAR_R, GEARSHIFT_CTRL_R}};
};

#endif // ZHONGZHI_H
