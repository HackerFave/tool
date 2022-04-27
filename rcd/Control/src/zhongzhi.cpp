#include "zhongzhi.h"
#include "mydebug.h"
#include <cstring>
#include <QMap>
#include <QThread>
#include <QTime>
#include <QtMath>

#define UART_PATH "/dev/ttyUSB_18"
#define UART_METER_PATH "/dev/ttyUSB_28"

zhongzhi::zhongzhi(QObject *parent)
    : QObject(parent)
{
    std::memset(&cacheControlData, 0x00, sizeof(cacheControlData));
    std::memset(&tl0x0c300021St, 0x00, sizeof(TL0x0C300021Struct));
    std::memset(&tl0x0c303021St, 0x00, sizeof(TL0x0C303021Struct));
    std::memset(&tl0x1830a021St, 0x00, sizeof(TL0x1830A021Struct));
    std::memset(&tl0x1830a321St, 0x00, sizeof(TL0x1830A321Struct));
    std::memset(&tl0x1830b021St, 0x00, sizeof(TL0x1830B021Struct));

    subObj.ConnectTo(ZMQ_ADDR_Gateway);
    subObj.Subscribe(ZMQ_TOPIC_VEHICLE_RAWDATA,
                     std::bind(&zhongzhi::HandleZmqVehicleRawData,
                               this,
                               std::placeholders::_1,
                               std::placeholders::_2,
                               std::placeholders::_3));

    meterDataTimer.setInterval(50);
    connect(&meterDataTimer, SIGNAL(timeout()), this, SLOT(HandleMeterDataTimerSlot()));
    meterDataTimer.start();

    meterHeartBeatTimer.setInterval(500);
    connect(&meterHeartBeatTimer, SIGNAL(timeout()), this, SLOT(HandleMeterHeartBeatTimerSlot()));
    meterHeartBeatTimer.start();

}

zhongzhi::~zhongzhi()
{
    uart.close();
    uartMeter.close();
}

void zhongzhi::StartLoop() {
    while (true) {
        if (!uart.isOpen()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            InitUart();
        } else {
            HandleUartData();
        }
    }
}

void zhongzhi::SetContronData(const MainCtrlMsg &msg, ControlData &data)
{
    //steeringwheel leftMax-rightMax:0%-100%
    if (msg.steeringwheel >= static_cast<uint16_t>(STEERINGWHEEL_RIGHT_MIN))
    {
        //right;
        int steerwheel = msg.steeringwheel<=STEERINGWHEEL_RIGHT_OFFSET?STEERINGWHEEL_RIGHT_OFFSET:msg.steeringwheel;
        int rightAngle = static_cast<uint16_t>(STEERINGWHEEL_RIGHT_MAX - steerwheel) + steeringwheelLeftTotal;
        data.steeringWheelAngle = static_cast<uint16_t>(
            (static_cast<float>(rightAngle) / steeringwheelTotal) * 10000);
    }
    else
    {
        //left
        int steerwheel = msg.steeringwheel>=STEERINGWHEEL_LEFT_MAX?STEERINGWHEEL_LEFT_MAX:msg.steeringwheel;
        data.steeringWheelAngle = 5000-(static_cast<float>(steerwheel) / steeringwheelTotal* 10000);
    }
    MY_DEBUG() << "steeringWheelAngle-%:" << data.steeringWheelAngle;
    int accelerator = msg.accelerator;
    if(msg.accelerator <= ACC_MIN)
    {
        accelerator = ACC_MIN;
    }
    else if(msg.accelerator >= ACC_MAX)
    {
        accelerator = ACC_MAX;
    }
    accelerator -= ACC_MIN;
    data.throttlePedal = static_cast<float>(accelerator) / accTotal * 10000;
    MY_DEBUG() << "throttlePedal-%:" << data.throttlePedal;

    int brake = msg.brake;
    if(brake <= BRAKE_MIN)
    {
        brake = BRAKE_MIN;
    }
    else if(brake >= BRAKE_MAX)
    {
        brake = BRAKE_MAX;
    }
    brake -= BRAKE_MIN;
    data.brakePedal = static_cast<float>(brake) / brakeTotal * 10000;
    MY_DEBUG() << "brakePedal-%:" << data.brakePedal;

    if ((msg.gear & GEAR_MASK) == GEAR_P) //P档激活手刹
    {
        data.parkCtrl = 1;
    }
    else
    {
        data.parkCtrl = 0;
    }
    MY_DEBUG() << "parkCtrl:" << data.parkCtrl;

    data.gearShiftCtrl = zhongzhiGearMap.value(msg.gear & GEAR_MASK);
    MY_DEBUG() << "gearShift:" << data.gearShiftCtrl;

    // 内循环 改成 排气制动
    if(((msg.key_1 >> KEY_1_BIT6_LOOP_INTERNAL) & 0x01) == 0x00) {
        data.exhuastBrake = 1;
    } else {
        data.exhuastBrake = 0;
    }
    MY_DEBUG() << "exhuastBrake:" << data.exhuastBrake;
}

const static QMap<uint8_t, uint32_t> dirButtonMap = {
    {MIRROR_DIR_NO,Qt::Key_Any},
    {MIRROR_DIR_UP, Qt::Key_Escape},
    {MIRROR_DIR_DOWN, Qt::Key_Enter},
    {MIRROR_DIR_LEFT, Qt::Key_Left},
    {MIRROR_DIR_RIGHT, Qt::Key_Right}};

const static QMap<uint32_t,QString> dirKeyNameMap = {
    {  Qt::Key_Escape,QString("Key_Up")},
    {  Qt::Key_Enter,QString("Key_Down")},
    {  Qt::Key_Left,QString("Key_Left")},
    {  Qt::Key_Right,QString("Key_Right")}
};

void zhongzhi::HandleButton(const MainCtrlMsg &msg)
{
    //方向键 key_left/key_right 代表左右移动 key_up 代表返回 key_down 代表确认
    static KeyButton dir = {Qt::Key_Any};
    uint8_t tmpKey = msg.mirror & MIRROR_DIR_MASK;
    if ((dir.key != Qt::Key_Any) && (tmpKey == MIRROR_DIR_NO) )
    {
        emit EmitButton(dir.key);
        MY_DEBUG() << "Emit dir key Button:" << dirKeyNameMap.value(dir.key);
        dir.key = Qt::Key_Any;
    }
    else
    {
        dir.key = dirButtonMap.value(tmpKey);
    }
    //接管按键使用后备箱.0-按键为摁下。
    static uint8_t lastControlKeyValue = 0x01; // 默认此刻为松开状态
    static uint32_t controlKey = Qt::Key_PageUp; //默认下次操作为开始接管

    tmpKey = (msg.key_1 >> KEY_1_BIT7_TRUNK) & 0x01;

    //松开按键为 0 -> 1 的时候，发送接管操作
    if(tmpKey == 0x01 && lastControlKeyValue == 0x00) {
        emit EmitButton(controlKey);
        MY_DEBUG()<<"Remote Control " << (controlKey == Qt::Key_PageUp ? QString("Start") : QString("Finished"));
        controlKey = controlKey == Qt::Key_PageUp ? Qt::Key_PageDown : Qt::Key_PageUp;
    }
    lastControlKeyValue = tmpKey;

}

void zhongzhi::HandleZmqVehicleRawData(std::string &topic, const void *data, int len) {
    if(data == nullptr || len == 0) {
        MY_DEBUG() << "get zmq vehicle raw data failed";
        return;
    }

    VehicleRawData* vehicleData = const_cast<VehicleRawData *>(reinterpret_cast<const VehicleRawData *>(data));

    for(int i = 0; i < vehicleData->length; i++) {
        switch (vehicleData->rawData[i].canId) {
        case 0x0C300021: { //转速
            std::memmove(&tl0x0c300021St, vehicleData->rawData->data, sizeof(TL0x0C300021Struct));
            engineSpeed = static_cast<float>(tl0x0c300021St.wire_status_engine_speed_raw) * static_cast<float>(0.125);
            break;
        }
        case 0x0C303021: { //车速
            std::memmove(&tl0x0c303021St, vehicleData->rawData[i].data, sizeof(TL0x0C303021Struct));
            speed = static_cast<float>(tl0x0c303021St.wire_status_truck_speed_raw) * (static_cast<float>(1) / static_cast<float>(256));
            break;
        }
        case 0x1830A021: { //灯光
            std::memmove(&tl0x1830a021St, vehicleData->rawData[i].data, sizeof(TL0x1830A021Struct));
            break;
        }
        case 0x1830A321: { //闪光灯
            std::memmove(&tl0x1830a321St, vehicleData->rawData[i].data, sizeof(TL0x1830A321Struct));
            break;
        }
        case 0x1830b021: { //驻车
            std::memmove(&tl0x1830b021St, vehicleData->rawData[i].data, sizeof(TL0x1830B021Struct));
            break;
        }
        default:
            break;

        }
    }

    MY_DEBUG() << "topic: " << topic.data()
               << ", imei:" << reinterpret_cast<char *>(vehicleData->imei)
               << ", type: " << static_cast<int>(vehicleData->type)
               << ", engine speed: " << static_cast<int>(engineSpeed)
               << ", speed: " << static_cast<int>(speed);

}

void zhongzhi::StartRun()
{
    readT = QThread::create(&zhongzhi::StartLoop,this);
    uart.moveToThread(readT);
    readT->start();
}

void zhongzhi::HandleMeterHeartBeatTimerSlot()
{
    if (uartMeter.isOpen())
    {
        uartMeter.write(meterHeartBeat, sizeof(meterHeartBeat));
    }
    else
    {
        InitMeterUart();
    }
}

void zhongzhi::HandleMeterDataTimerSlot()
{
    if(!uartMeter.isOpen()) return;
    uint8_t tmpData[25] = {0xaa, 0xff, 0x01, 0x02, 0x00};
    static uint8_t flash = 0;
    tmpData[24] = 0x55;
    //time
    QTime cur = QTime::currentTime();
    tmpData[4] = static_cast<uint8_t>(cur.hour());
    tmpData[5] = static_cast<uint8_t>(cur.minute());
    //mileage
    tmpData[6] = 0;
    tmpData[7] = 0;
    tmpData[8] = 0;
    //speed
    tmpData[9] = static_cast<uint8_t>(qFloor(speed / 2.6));
    tmpData[10] = static_cast<uint8_t>((speed - qFloor(speed / 2.6)) / 2.6 * 255);
    //engine speed 仪表转速单位:1*min/100r
    tmpData[11] = static_cast<uint8_t>(qFloor(engineSpeed / 100 / 2.6));
    tmpData[12] = static_cast<uint8_t>(engineSpeed - qFloor(engineSpeed / 100 / 2.6) / 2.6 * 255);
    //档位
    uint8_t Gear = 0x80; //自动档
    int gear = static_cast<int>(tl0x0c303021St.wire_status_gear_shift_raw) - 125;
    if(gear < 0) {
        tmpData[13] = Gear | GEAR_R;
    } else if(gear == 0) {
        tmpData[13] = Gear | GEAR_N;
    } else {
        tmpData[13] = Gear | GEAR_D;
    }

    tmpData[14] = 0x11; //仪表电脑上电
    if(!tl0x1830b021St.wire_status_hand_brake) { //驻车关 0:open, 1:close
        tmpData[14] |= 0x08;
    }
    if(tl0x1830a021St.wire_status_high_beam != 0x03) {  //远光关 0:open, 1:close
        tmpData[14] |= 0x40;
    }

    //灯光控制
    tmpData[15] = 0x07; //全灭
    if(!((flash++) % 10)) { //500ms 一次闪烁
        if(tl0x1830a321St.wire_status_emergency_switch == 0x01) { //双闪开
            tmpData[15] = 0x04;
        } else if(tl0x1830a021St.wire_status_left_light == 0x03) { //左转开
            tmpData[15] = 0x06;
        } else if(tl0x1830a021St.wire_status_right_light == 0x03) { //右转开
            tmpData[15] = 0x05;
        }
    }

    printf("uartMeter write: ");
    for(int i = 0; i < 25; i++) {
        printf("%02x ", tmpData[i]);
    }
    printf("\n");

    uartMeter.write(reinterpret_cast<char *>(tmpData), sizeof(tmpData));

}

void zhongzhi::InitUart() {
    uart.setPortName(UART_PATH);
    uart.setBaudRate(QSerialPort::Baud115200);
    uart.setFlowControl(QSerialPort::NoFlowControl);
    uart.setDataBits(QSerialPort::Data8);
    uart.setStopBits(QSerialPort::OneStop);
    uart.setParity(QSerialPort::NoParity);
    uart.close();
    bool ret = uart.open(QIODevice::ReadWrite);
    if (!ret)
    {
        EmitStatus(DEV_LINKSTATUS_DISCONNECTED);
    }
    else
    {
        EmitStatus(DEV_LINKSTATUS_CONNECTED_NO_DATA);
    }
    MY_DEBUG() << "Open Uart:" << UART_PATH << " ret:" << ret << " - " << uart.errorString();
}

void zhongzhi::InitMeterUart()
{
    uartMeter.setPortName(UART_METER_PATH);
    uartMeter.setBaudRate(QSerialPort::Baud115200);
    uartMeter.setFlowControl(QSerialPort::NoFlowControl);
    uartMeter.setDataBits(QSerialPort::Data8);
    uartMeter.setStopBits(QSerialPort::OneStop);
    uartMeter.setParity(QSerialPort::NoParity);
    uartMeter.close();
    bool ret = uartMeter.open(QIODevice::ReadWrite);
    MY_DEBUG() << "Open Meter Uart:" << UART_METER_PATH << " ret:" << ret << " - "
               << uartMeter.errorString();
}

static QMap<uint8_t, QString> gearMap = {{GEAR_N, "N"}, {GEAR_D, "D"}, {GEAR_R, "R"}, {GEAR_P, "P"}};

static QMap<uint8_t, QString> mirrorMap
= {{MIRROR_DIR_DOWN, "Down"},
   {MIRROR_DIR_LEFT, "Left"},
   {MIRROR_DIR_NO, "No"},
   {MIRROR_DIR_RIGHT, "Right"},
   {MIRROR_DIR_UP, "Up"},
   {MIRROR_SELECT_L, "L"},
   {MIRROR_SELECT_R, "R"},
   {MIRROR_SELECT_NO, "No"}};

static QMap<uint8_t, QString> key1Map
= {{KEY_1_BIT0_ENGINE, "点火"},
   {KEY_1_BIT1_ON, "ON"},
   {KEY_1_BIT2_ACC, "ACC"},
   {KEY_1_BIT3_SPEAKER, "喇叭"},
   {KEY_1_BIT4_SEAT_BELT, "安全带"},
   {KEY_1_BIT5_WARN_LIGHT, "双闪"},
   {KEY_1_BIT6_LOOP_INTERNAL, "内循环"},
   {KEY_1_BIT7_TRUNK, "后备箱"}};

static QMap<uint8_t, QString> key2Map
= {{KEY_2_BIT0_HIGH_BEAM, "远光灯"},
   {KEY_2_BIT1_LEFT_BEAM, "左转灯"},
   {KEY_2_BIT2_RIGHT_BEAM, "右转灯"},
   {KEY_2_BIT3_AC, "空调"},
   {KEY_2_BIT4_MIRROR_HEAT, "后视镜加热按键"},
   {KEY_2_BIT5_WIDTH_LIGHT, "示宽灯"},
   {KEY_2_BIT6_LOW_BEAM, "近光灯"},
   {KEY_2_BIT7_FOG_LIGHT, "雾灯"}};

void zhongzhi::HandleUartData()
{
    while (uart.isOpen())
    {
        if (!uart.waitForReadyRead(50))
        {
            uart.close();
            EmitStatus(DEV_LINKSTATUS_DISCONNECTED);
            continue;
        }
        uartBuf.append(uart.readAll());
        if ((uartBuf.size() >= 2) && (uartBuf.at(0) == static_cast<char>(0xaa)) && (uartBuf.at(1) == static_cast<char>(0xff)))
        {
            if(uartBuf.size()>=25 && uartBuf.at(24) == 0x55)
            {
                EmitStatus(DEV_LINKSTATUS_CONNECTED_DATA);
                MainCtrlMsg tmp;
                std::memset(&tmp,0x00,sizeof(MainCtrlMsg));
                std::memset(cacheLightData, 0x00, sizeof(cacheLightData));
                tmp.clutch = ((static_cast<uint8_t>(uartBuf.at(4))) << 8) | static_cast<uint8_t>(uartBuf.at(5));
                tmp.brake = ((static_cast<uint8_t>(uartBuf.at(6))) << 8) |static_cast<uint8_t>(uartBuf.at(7)) ;
                tmp.accelerator = ((static_cast<uint8_t>(uartBuf.at(8))) << 8) | static_cast<uint8_t>(uartBuf.at(9));
                tmp.handbrake = ((static_cast<uint8_t>(uartBuf.at(10))) << 8)|static_cast<uint8_t>(uartBuf.at(11) );
                tmp.steeringwheel =  ((static_cast<uint8_t>(uartBuf.at(12))) << 8)|static_cast<uint8_t>(uartBuf.at(13));


                tmp.gear = uartBuf.at(14);
                tmp.mirror = uartBuf.at(15);
                tmp.key_1 = uartBuf.at(16);
                tmp.key_2 = uartBuf.at(17);
                std::memmove(tmp.unused, uartBuf.data()+18 ,sizeof(tmp.unused));

                MY_DEBUG() << " 离合:" << tmp.clutch << " 刹车:" << tmp.brake
                           << " 油门:" << tmp.accelerator << " 手刹:" << tmp.handbrake
                           << " 方向盘:" << tmp.steeringwheel
                           << " 挡位:" << gearMap.value(tmp.gear & GEAR_MASK)
                           << " 后视镜方向:" << mirrorMap.value(tmp.mirror & MIRROR_DIR_MASK)
                           << " 后视镜选择:" << mirrorMap.value(tmp.mirror & MIRROR_SELECT_MASK);

                for (int i = 0; i < 8; i++)
                {
                    if (((tmp.key_1 >> i) & 0x01) == 0x00)
                    {
                        MY_DEBUG() << key1Map.value(i) << " Open";

                        switch (i) {
                        case KEY_1_BIT5_WARN_LIGHT: //双闪 0x01 5-6bit
                            cacheLightData[2] |= 0x10;
                            break;
//                        case KEY_1_BIT6_LOOP_INTERNAL: //内循环改成排气制动
//                            break;
                        default:
                            break;

                        }
                    }
                    if (((tmp.key_2 >> i) & 0x01) == 0x00)
                    {
                        MY_DEBUG() << key2Map.value(i) << " Open";

                        switch (i) {
                        case KEY_2_BIT0_HIGH_BEAM: //远光 0x40 7-8bit
                            cacheLightData[0] |= 0x40;
                            break;
                        case KEY_2_BIT1_LEFT_BEAM: //左转 0x01 1-2bit
                            cacheLightData[0] |= 0x01;
                            break;
                        case KEY_2_BIT2_RIGHT_BEAM: //右转 0x04 3-4bit
                            cacheLightData[0] |= 0x04;
                            break;
                        case KEY_2_BIT3_AC: //空调 改成 侧照明灯 0x01 1-2bit
                            cacheLightData[2] |= 0x01;
                            break;
//                        case KEY_2_BIT4_MIRROR_HEAT: //后视镜加热
//                            break;
                        case KEY_2_BIT5_WIDTH_LIGHT: //示宽灯/位置灯 0x04 3-4bit
                            cacheLightData[2] |= 0x04;
                            break;
                        case KEY_2_BIT6_LOW_BEAM: //近光 0x10 5-6bit
                            cacheLightData[0] |= 0x10;
                            break;
                        case KEY_2_BIT7_FOG_LIGHT: //雾灯 0x10 5-6bit
                            cacheLightData[1] |= 0x10;
                            break;
                        default:
                            break;
                        }

                    }
                }
                char buff[32] = {0};
                snprintf(buff, sizeof(buff), "%02x %02x %02x %02x", cacheLightData[0], cacheLightData[1], cacheLightData[2], cacheLightData[3]);
                qDebug() << "send light data: " << buff;

                SetContronData(tmp, cacheControlData);
                QByteArray controlData(reinterpret_cast<const char *>(&cacheControlData),
                                       sizeof(cacheControlData));
                QByteArray lightData(reinterpret_cast<const char *>(cacheLightData), sizeof(cacheLightData));
                emit EmitControlData(controlData);
                emit EmitControlLight(lightData);
                HandleButton(tmp);
                uartBuf.remove(0, 25);
                uart.write(heartBeat, sizeof(heartBeat));

                // send cmd 01 01 , 方向盘回转
                uart.write(controlHeartBeat, sizeof(controlHeartBeat));
            }
            else
            {
                uartBuf.remove(0,1);
            }
        }
        else
        {
            uartBuf.remove(0,1);
        }
    }
}
