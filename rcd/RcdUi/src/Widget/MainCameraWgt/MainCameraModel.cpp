#include "MainCameraModel.h"
//#include "../include/can/canraw.hpp"
#include <QDebug>

MainCameraModel::MainCameraModel(QObject *parent) : IModel(parent)
{
    m_frontCameraUrl = QLatin1String("");
    m_backCameraUrl = QLatin1String("");

    m_throttle.max = 100;
    m_throttle.min = 0;

    m_brake.max = 100;
    m_brake.min = 0;

    //    m_wheel.max = 540;
    //    m_wheel.min = -540;
    m_wheel.max = 720;
    m_wheel.min = -720;

    //    m_speed.max = 50;
    //    m_speed.min = 0;
    m_speed.max = 45;
    m_speed.min = 0;
}

//void MainCameraModel::setVehicleRawData(VehicleRawData data)
//{
//    for(int i = 0;i < data.length;i++){
//        switch (data.rawData[i].canId) {
//        case 0x0C305121:{
//            NT0x0C305121Struct *pDate = reinterpret_cast<NT0x0C305121Struct*>(data.rawData[i].data);
//            m_throttle.current = pDate->wire_status_acc_pedal_percent_raw*0.1;
//            m_brake.current = pDate->wire_status_elctric_brake_pedal_percent_raw*0.1;
//            m_wheel.current = (pDate->wire_status_front_wheel_angle_raw*0.01-45)*12;
//            emit sendModelUpdate(MC_ThrottleBrakeWheel);
//            break;
//        }
//        case 0x0C305321:{
//            NT0x0C305321Struct *pDate = reinterpret_cast<NT0x0C305321Struct*>(data.rawData[i].data);
//            m_speed.current = (pDate->wire_status_left_wheel_speed_raw+pDate->wire_status_right_wheel_speed_raw)*0.01/2;
//            emit sendModelUpdate(MC_Speed);
//            break;
//        }
//        case 0x0C305421:{
//            NT0x0C305421Struct *pDate = reinterpret_cast<NT0x0C305421Struct*>(data.rawData[i].data);
//            m_gear = pDate->wire_status_gear_shift_raw - 125.0;
//            emit sendModelUpdate(MC_Gear);
//            break;
//        }
//        default:
//            break;
//        }
//    }
//}

void MainCameraModel::setCameraUrl(const QString &front,const QString &back)
{
    m_frontCameraUrl = front;
    m_backCameraUrl = back;
}

void MainCameraModel::setThrottleBrakeWheel(float throttle,float brake,float wheel)
{
    m_throttle.current = throttle;
    m_brake.current = brake;
    m_wheel.current = wheel;
    emit sendModelUpdate(MC_ThrottleBrakeWheel);
}
void MainCameraModel::setThrottle(float throttle)//加速踏板百分比
{
    m_throttle.current = throttle;
    emit sendModelUpdate(MC_Throttle);
}
void MainCameraModel::setBrake(float brake)//刹车
{

    m_brake.current = brake;
    emit sendModelUpdate(MC_Brake);
}
void MainCameraModel::setWheel(float wheel)//方向盘转角
{
    m_wheel.current = wheel;
    emit sendModelUpdate(MC_Wheel);
}
void MainCameraModel::setSpeed(float speed)
{
    m_speed.current = speed;
    emit sendModelUpdate(MC_Speed);
}
void MainCameraModel::setSpeedRaw(float speed)
{
    m_speed_raw.current = speed;
    emit sendModelUpdate(MC_Throttle_RingDila);
}

void MainCameraModel::setGear(float gear)
{
    if(m_gear != static_cast<int>(gear)){
        m_gear = gear;
        emit sendModelUpdate(MC_Gear);
    }
}

void MainCameraModel::setVehicleDelay(uint delay)
{
    m_vehicleDelay = delay;
    emit sendModelUpdate(MC_Delay);
}

void MainCameraModel::setRemoteStatus(uint status)
{
    m_remoteStatus = static_cast<RemoteStatus>(status);
    emit sendModelUpdate(MC_RemoteStatus);
}
