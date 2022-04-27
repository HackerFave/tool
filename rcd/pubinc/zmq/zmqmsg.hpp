/**
 * @file zmqmsg.hpp
 * @author lujiefeng
 * @brief 
 * @version 0.1
 * @date 2020-08-21
 * 
 * @copyright Copyright () 2020
 * 
 */
#include <cstdint>

#pragma once

#define IMEI_LENGTH 15

//控制设备连接状态 ControlDevLinkStatus
//断开
#define DEV_LINKSTATUS_DISCONNECTED 0
//已连接,无数据
#define DEV_LINKSTATUS_CONNECTED_NO_DATA 1
//已连接,有数据
#define DEV_LINKSTATUS_CONNECTED_DATA 2

//请求遥控命令 ReqRemoteControl.opt
//未知命令
#define REQ_REMOTE_CONTROL_UNKNOWN 0
//遥控开始
#define REQ_REMOTE_CONTROL_START 1
//遥控结束
#define REQ_REMOTE_CONTROL_END 2

//遥控请求响应结果 RespRemoteControl.result
//未知
#define RESP_REMOTE_CONTROL_UNKNOWN 0
//车辆未连接
#define RESP_REMOTE_CONTROL_VEHICLE_DISCONNECT 1
//车辆已接受
#define RESP_REMOTE_CONTROL_VEHICLE_RECV 2
//车辆接受超时
#define RESP_REMOTE_CONTROL_VEHICLE_TIMEOUT 3
//车辆拒绝
#define RESP_REMOTE_CONTROL_VEHICLE_REJECT 4

//车辆状态 VehicleStatus.status
//未知
#define VEHICLE_STATUS_UNKNOWN 0
//车辆已连接
#define VEHICLE_STATUS_CONNECTED 1
//车辆断开连接
#define VEHICLE_STATUS_DISCONNECTED 2
//车辆CAN无数据
#define VEHICLE_STATUS_NO_CAN 3


//远程控制数据中档位定义
#define GEARSHIFT_CTRL_N 3
#define GEARSHIFT_CTRL_D 2
#define GEARSHIFT_CTRL_R 4

//远程控制数据中货箱控制
#define BUCKET_CTRL_HOLDON 0
#define BUCKET_CTRL_UP 1
#define BUCKET_CTRL_DOWN 2


#pragma pack(1)

typedef struct
{
    uint64_t utcTime;
    uint16_t steeringWheelAngle; //方向盘角度
    uint16_t throttlePedal;      //油门踏板开度百分比
    uint16_t brakePedal;         //电制动踏板开度百分比
    uint16_t hydraulicBrake;     //机械制动制动控制
    uint16_t retarderCtrl;       //缓速器控制
    uint8_t exhuastBrake;       //气压制动控制    0:气压制动失效，1:气压制动使能
    uint8_t parkCtrl;           //装载制动控制    0:解除驻车，1:驻车
    uint8_t bucketCtrl;         //货箱控制  0:保持 1:上升 2:下降
    uint8_t trumpetCtrl;        //转向灯控制
    uint8_t gearShiftCtrl;      //档位控制 2:D, 3:N, 4:R
    uint8_t eStop;             //紧急停车
} ControlData;

typedef struct
{
    uint8_t wheel;     //方向盘设备连接状态
    uint8_t gearShift; //档位控制器连接状态
    uint8_t bucket;    //举升设备连接状态
} ControlDevLinkStatus;

typedef struct
{
    uint8_t imei[IMEI_LENGTH]; //设备IMEI
    uint8_t opt;               //遥控命令
} ReqRemoteControl;

typedef struct
{
    uint8_t imei[IMEI_LENGTH]; //设备IMEI
    uint8_t result;            //遥控命令请求响应结果
} RespRemoteControl;

typedef struct
{
    uint8_t imei[IMEI_LENGTH]; //设备IMEI
    uint8_t type;              //车辆类型
    uint8_t status;            //车辆状态
} VehicleStatus;

typedef struct
{
    uint32_t canId;
    uint8_t data[8];
} CanRawData;

typedef struct
{
    uint8_t imei[IMEI_LENGTH]; //设备IMEI
    uint8_t type;              //车辆类型
    uint8_t length;            //CanRawData 长度
    CanRawData rawData[0];     //车身数据
} VehicleRawData;

typedef struct
{
    uint8_t imei[IMEI_LENGTH];
    uint32_t delayUs;
}VehicleDelay;

typedef struct
{
    uint32_t key;
}KeyButton;

typedef struct
{
    uint8_t imei[IMEI_LENGTH];
    uint16_t bodyLen;
    uint8_t body[256];
}HmiMsg;
#pragma pack()
