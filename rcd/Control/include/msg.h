#ifndef MSG_H
#define MSG_H
#include <cstdint>

#pragma pack(1)
typedef struct
{
    uint16_t steeringWheelAngle;
    uint8_t brake;
    uint8_t clutch;
    //第5字节
    uint8_t seatBelt : 1;
    uint8_t outSideView : 1;
    uint8_t handBrake : 1;
    uint8_t byte5Res1 : 1;
    uint8_t rightMirror : 1;
    uint8_t byte5Res2 : 1;
    uint8_t rightTurnLED : 1;
    uint8_t leftTurnLED : 1;
    //第6字节
    uint8_t acc : 1;
    uint8_t lowBeam : 1;
    uint8_t heighBeam : 1;
    uint8_t inSideView : 1;
    uint8_t widthLight : 1;
    uint8_t wiper1 : 1;
    uint8_t wiper2 : 1;
    uint8_t flashLed : 1;
    //第7字节
    uint8_t leftMirror : 1;
    uint8_t gear1 : 1;
    uint8_t gear2 : 1;
    uint8_t gear3 : 1;
    uint8_t gear4 : 1;
    uint8_t gear5 : 1;
    uint8_t gearR : 1;
    uint8_t byte7Res1 : 1;
    //第8字节
    uint8_t res;
} JoystickMsg;

typedef struct
{
    char pading;                 // == 0x07
    uint16_t steeringWheelAngle; //方向盘。左打死：0x0000 中间： 0x7FFF 右打死：0xFFFF
    uint16_t brake;              //制动。不踩：0x03ff 踩死:0x0000
    uint16_t accelerator;        //油门。不踩：0x03ff 踩死:0x0000
    uint16_t clutch;             //离合。不踩:0x03ff 踩死:0x0000
    uint16_t pading0;
    uint8_t button;    // 0x01.左拨片，0x02.右拨片，0x04.三角按键，0x08.方框按键, 0x10.圆圈按键 , 0x20.X按键, 0x40.SE按键 ,0x80.ST按键
    uint8_t LR_Button; //0x01.R2按键，0x02.L2按键 0x04.L3按键，0x08.R3按键，0x10.中间标志按键
    uint8_t pading1;
    uint8_t dirButton; // 0x00.上，0x01.右上，0x02.右，0x03.右下，0x04.下 ，0x05.左下，0x06.左，0x07.左上，0x0f.不按
    uint8_t pading2[49];
} Tr300rsSt;

typedef struct
{
    char pading;
    char gear;
    char pad10[10];
} GearSt;
#pragma pack()
#endif // MSG_H
