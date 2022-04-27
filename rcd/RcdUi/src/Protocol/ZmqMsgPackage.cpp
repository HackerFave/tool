#include "ZmqMsgPackage.h"
#include "../include/can/canraw.hpp"
#include "../include/qtzmq.h"
#include <QDataStream>
#include <QDebug>

ZmqMsgPackage::ZmqMsgPackage()
{

}

bool  ZmqMsgPackage::c2uButtonKey(const void *data, int)
{
    KeyButton *pKey = (KeyButton*)(data);

    qDebug() << " c2uKey:" << static_cast<Qt::Key>(pKey->key)<<static_cast<int>(pKey->key);

    ModuleDir module;
    module.append(/*MD_MainProcessor*/MD_InteractiveManager);
    QVariant variant(QVariant::UInt);
    variant.setValue(pKey->key);

    m_moduleDirs.append(module);
    m_cmds.append(/*MP_SoftKeyPress*/IA_KeyPress);
    m_datas.append(variant);

    return true;
}


bool  ZmqMsgPackage::g2uVehicleRawData(const void *data, int)
{
    VehicleRawData* vehicleData = (VehicleRawData*)(data);
    m_imei = QLatin1String(reinterpret_cast<char*>(vehicleData->imei),IMEI_LENGTH);

    ModuleDir module;
    module.append(MD_MainCamera);

    for(int i = 0;i < vehicleData->length;i++){
        switch (vehicleData->rawData[i].canId)
        {
        case 0x0C305121:{
            NT0x0C305121Struct *pDate = reinterpret_cast<NT0x0C305121Struct*>(vehicleData->rawData[i].data);
            float throttle = pDate->wire_status_acc_pedal_percent_raw * 0.1;
            float brake = pDate->wire_status_elctric_brake_pedal_percent_raw * 0.1;
            float wheel = (pDate->wire_status_front_wheel_angle_raw * 0.01 - 45) * 12;
            QList<QVariant> listVariant;
            listVariant.append(QVariant::fromValue(throttle));
            listVariant.append(QVariant::fromValue(brake));
            listVariant.append(QVariant::fromValue(wheel));
            ModuleDir module;
            module.append(MD_MainCamera);
            m_moduleDirs.append(module);
            m_cmds.append(MC_ThrottleBrakeWheel);
            m_datas.append(listVariant);
            break;
        }
        case 0x0C305321:{
            NT0x0C305321Struct *pDate = reinterpret_cast<NT0x0C305321Struct*>(vehicleData->rawData[i].data);
            float speed = (pDate->wire_status_left_wheel_speed_raw+pDate->wire_status_right_wheel_speed_raw) * 0.01 / 2;
            QVariant variant = QVariant::fromValue(speed);
            m_moduleDirs.append(module);
            m_cmds.append(MC_Speed);
            m_datas.append(variant);
            break;
        }
        case 0x0C305421:{
            NT0x0C305421Struct *pDate = reinterpret_cast<NT0x0C305421Struct*>(vehicleData->rawData[i].data);
            float gear = pDate->wire_status_gear_shift_raw - 125.0;
            QVariant variant = QVariant::fromValue(gear);

            m_moduleDirs.append(module);
            m_cmds.append(MC_Gear);
            m_datas.append(variant);
            break;
        }
            /***************新添加can TL 数据 谭创*****************/

        case 0x1830FF21:{
            TL0x1830FF21Struct *pDate = reinterpret_cast<TL0x1830FF21Struct*>(vehicleData->rawData[i].data);
            QVariant variant = QVariant::fromValue(QString(QLatin1String(pDate->data))+QString("同力车型"));
            m_moduleDirs.append(module);
            m_cmds.append(MC_VehicleData);
            m_datas.append(variant);
            break;
        }
//        case 0x1830B021:{
//            TL0x1830B021Struct *pDate = reinterpret_cast<TL0x1830B021Struct*>(vehicleData->rawData[i].data);

//            float tl1 = pDate->wire_status_vehicle_ctrl_mode; //车辆控制模式
//            float tl2 = pDate->wire_status_engine ;     //发动机激活状态
//            float tl3 = pDate->wire_status_hand_brake ; //手刹激活状态
//            float tl4 = pDate->wire_status_ebs ;        // EBS激活状态
//            float tl5 = pDate->wire_status_retarder ;   //缓速器激活状态
//            float tl6 = pDate->wire_status_transmission ; //变速箱的激活状态
//            float tl7 = pDate->wire_status_eps ;          // EPS激活状态
//            float tl8 = pDate->wire_status_lift ;         //举升激活状态
//            QList<QVariant> listVariant;
//            listVariant<<"车辆状态:"<<tl1<<tl2<<tl3<<tl4<<tl5<<tl6<<tl7<<tl8;
//            ModuleDir module;
//            module.append(MD_MainCamera);
//            m_moduleDirs.append(module);
//            m_cmds.append(MC_Other);
//            m_datas.append(listVariant);
//            break;
//        }
//        case 0x1830E021:{
//            TL0x1830E021Struct *pDate = reinterpret_cast<TL0x1830E021Struct*>(vehicleData->rawData[i].data);
//            float tl1 = pDate->wire_status_engine_falut;       //发动机故障
//            float tl2 = pDate->wire_status_transmission_falut; //变速箱故障
//            float tl3 = pDate->wire_status_ebs_falut;          // EBS故障
//            float tl4 = pDate->wire_status_eps_falut;          // EPS故障
//            QList<QVariant> listVariant;
//            listVariant<<"EBS故障状态:"<<tl1<<tl2<<tl3<<tl4;
//            ModuleDir module;
//            module.append(MD_MainCamera);
//            m_moduleDirs.append(module);
//            m_cmds.append(MC_Other);
//            m_datas.append(listVariant);
//            break;
//        }
//        case 0x18FECA21:{//故障码
//            TL0x18FECA21Struct *pDate = reinterpret_cast<TL0x18FECA21Struct*>(vehicleData->rawData[i].data);
//            float tl1 = pDate->wire_status_vehicle_falut;     //整车故障等级
//            float tl2 = pDate->wire_status_engine_falut_code; //发动机故障编码
//            float tl3 = pDate->res0;
//            float tl4 = pDate->res1;
//            float tl5 = pDate->res2;
//            float tl6 = pDate->wire_status_transmission_falut_code; //变速箱故障编码
//            float tl7 = pDate->res3;
//            float tl8 = pDate->res4;
//            QList<QVariant> listVariant;
//            listVariant<<"车辆故障状态:"<<tl1<<tl2<<tl3<<tl4<<tl5<<tl6<<tl7<<tl8;
//            ModuleDir module;
//            module.append(MD_MainCamera);
//            m_moduleDirs.append(module);
//            m_cmds.append(MC_Other);
//            m_datas.append(listVariant);
//            break;
//        }
//        case 0x18FECB21:{
//            TL0x18FECB21Struct *pDate = reinterpret_cast<TL0x18FECB21Struct*>(vehicleData->rawData[i].data);
//            float tl1 = pDate->res0;
//            float tl2 = pDate->wire_status_eps_falut_code; // EPS故障编码
//            float tl3 = pDate->wire_status_ebs_falut_code; // EBS故障编码
//            float tl4 = pDate->res1;
//            float tl5 = pDate->res2;
//            float tl6 = pDate->res3;
//            float tl7 = pDate->wire_status_electrical_system_falut_code; // 电气系统故障编码
//            float tl8 = pDate->res4;
//            QList<QVariant> listVariant;
//            listVariant<<"故障编码:"<<tl1<<tl2<<tl3<<tl4<<tl5<<tl6<<tl7<<tl8;
//            ModuleDir module;
//            module.append(MD_MainCamera);
//            m_moduleDirs.append(module);
//            m_cmds.append(MC_Other);
//            m_datas.append(listVariant);
//            break;
//        }
        case 0x0C300021:{
            TL0x0C300021Struct *pDate = reinterpret_cast<TL0x0C300021Struct*>(vehicleData->rawData[i].data);
            float throttle = pDate->wire_status_actually_accelerate_pad_raw *0.4;//实际加速踏板百分比
            qDebug()<<pDate->wire_status_actually_accelerate_pad_raw
                   <<pDate->wire_status_actually_accelerate_pad_raw *0.4
                    <<"实际加速踏板百分比--------------------------111111111111111"
                    <<pDate->wire_status_engine_speed_raw*0.125
                    <<"发动机转速-------";
            float speed_raw =pDate->wire_status_engine_speed_raw*0.125;//发动机转速
            QList<QVariant> listVariant;
            listVariant.append(QVariant::fromValue(throttle));
            listVariant.append(QVariant::fromValue(speed_raw));
            m_moduleDirs.append(module);

            m_cmds.append(MC_Throttle_RingDila);
            m_datas.append(listVariant);
            break;
        }
//        case 0x0C301121:{
//            TL0x0C301121Struct *pDate = reinterpret_cast<TL0x0C301121Struct*>(vehicleData->rawData[i].data);
//            float speed = pDate->wire_status_front_axle_aver_velocity_raw;
//            QVariant variant = QVariant::fromValue(speed);
//            m_moduleDirs.append(module);
//            m_cmds.append(MC_Speed);//速度
//            m_datas.append(variant);
//            break;
//        }
        case 0x0C301021:{
            TL0x0C301021Struct *pDate = reinterpret_cast<TL0x0C301021Struct*>(vehicleData->rawData[i].data);
            float brake = pDate->wire_status_mechanical_brake_percent_raw*0.4;
            qDebug()<<pDate->wire_status_mechanical_brake_percent_raw
                   <<pDate->wire_status_mechanical_brake_percent_raw *0.4
                    <<"刹车踏板百分比--------------------------22222222222222";
            QVariant variant = QVariant::fromValue(brake);
            m_moduleDirs.append(module);
            m_cmds.append(MC_Brake);//刹车百分比
            m_datas.append(variant);
            break;
        }
//        case 0x0C301221:{
//            TL0x0C301221Struct *pDate = reinterpret_cast<TL0x0C301221Struct*>(vehicleData->rawData[i].data);
//            float tl1 = pDate->wire_status_retarder_torque_percent_raw; //缓速器扭矩百分比
//            float tl2 = pDate-> res0;
//            float tl3 = pDate-> res1;
//            float tl4 = pDate-> res2;
//            float tl5 = pDate-> res3;
//            float tl6 = pDate-> res4;
//            float tl7 = pDate-> res5;
//            float tl8 = pDate-> check_sum_0C301221;
//            float tl9 = pDate-> rollint_counter_0C301221;

//            QList<QVariant> listVariant;
//            listVariant<<"缓速器:"<<tl1<<tl2<<tl3<<tl4<<tl5<<tl6<<tl7<<tl8<<tl9;
//            ModuleDir module;
//            module.append(MD_MainCamera);
//            m_moduleDirs.append(module);
//            m_cmds.append(MC_Other);
//            m_datas.append(listVariant);
//            break;
//        }
        case 0x0C302021:{
            TL0x0C302021Struct *pDate = reinterpret_cast<TL0x0C302021Struct*>(vehicleData->rawData[i].data);
            float wheel_angle = pDate->wire_status_steering_wheel_angle_raw*0.1-1575;    //方向盘当前转角 - 125.0;
            QVariant variant = QVariant::fromValue(wheel_angle);
            qDebug()<<wheel_angle
                   <<pDate->wire_status_steering_wheel_angle_raw*0.1-1575
                    <<"方向盘当前转角--------------------------3333333333333";
            m_moduleDirs.append(module);
            m_cmds.append(MC_Wheel);//方向盘转角
            m_datas.append(variant);
            break;
        }
        case 0x0C303021:{
            TL0x0C303021Struct *pDate = reinterpret_cast<TL0x0C303021Struct*>(vehicleData->rawData[i].data);
            float gear = pDate->wire_status_gear_shift_raw - 125.0;
            float speed = pDate->wire_status_truck_speed_raw/256;

            qDebug()<<"当前档位------------------------"<<gear
                   <<"当前车速-----------------------"
                    <<speed;
            QList<QVariant> listVariant;
            listVariant.append(QVariant::fromValue(gear));
            listVariant.append(QVariant::fromValue(speed));
            m_moduleDirs.append(module);
            m_cmds.append(MC_Speed_Gear);//速度+档位
            m_datas.append(listVariant);
            break;
        }
//        case 0x0C304021:{
//            TL0x0C304021Struct *pDate = reinterpret_cast<TL0x0C304021Struct*>(vehicleData->rawData[i].data);
//            int  tl1 = pDate->wire_status_truck_horizontal_angle_raw;    //车辆水平纵向角度
//            int  tl2 = pDate->wire_status_truck_longitudinal_angle_raw;  //车辆水平横向角度
//            int  tl3 = pDate->wire_status_truck_lift_anle_raw;           //车辆水平纵向角度
//            float tl4 = pDate->wire_status_bucket_pos_raw;                 //货箱举升角度
//            float tl5 = pDate->check_sum_0C304021 ;
//            float tl6 = pDate->rollint_counter_0C304021;

//            QList<QVariant> listVariant;
//            listVariant<<"车辆角度:"<<tl1<<tl2<<tl3<<tl4<<tl5<<tl6;
//            ModuleDir module;
//            module.append(MD_MainCamera);
//            m_moduleDirs.append(module);
//            m_cmds.append(MC_Other);
//            m_datas.append(listVariant);
//            break;
//        }
//        case 0x1830F021:{
//            TL0x1830F021Struct *pDate = reinterpret_cast<TL0x1830F021Struct*>(vehicleData->rawData[i].data);
//            QVariant variant = QVariant::fromValue(QString("车辆识别码:")+QString(QLatin1String(pDate->data)));
//            ModuleDir module;
//            module.append(MD_MainCamera);
//            m_moduleDirs.append(module);
//            m_cmds.append(MC_Other);
//            m_datas.append(variant);
//            break;
//        }
//        case 0x1830A321:{
//            TL0x1830A321Struct *pDate = reinterpret_cast<TL0x1830A321Struct*>(vehicleData->rawData[i].data);
//            break;
//        }
//        case 0x1830A021:{
//            TL0x1830A021Struct *pDate = reinterpret_cast<TL0x1830A021Struct*>(vehicleData->rawData[i].data);
//            break;
//        }
        case 0x1830A221:{
            TL0x1830A221Struct *pDate = reinterpret_cast<TL0x1830A221Struct*>(vehicleData->rawData[i].data);

            float  tl1 = pDate-> wire_status_pressure_sensor1_raw; //气压传感器1
            float  tl2 = pDate-> wire_status_pressure_sensor2_raw; //气压传感器2
            float  tl3 = pDate-> wire_status_oil_sensor_raw*0.004;       //油量传感器

            QList<QVariant> listVariant;
            listVariant<<tl3;
            ModuleDir module;
            module.append(MD_VehicleInfo);//车辆信息界面
            m_moduleDirs.append(module);
            m_cmds.append(VI_Oil);
            m_datas.append(listVariant);
            break;
        }
//        case 0x1830A121:{
//            TL0x1830A121Struct *pDate = reinterpret_cast<TL0x1830A121Struct*>(vehicleData->rawData[i].data);

//            break;
//        }
        default:
            break;
        }
    }

    return true;
}

bool  ZmqMsgPackage::g2uVehicleStatus(const void *data, int len)
{
    VehicleStatus vehicleStatus;
    std::memcpy(&vehicleStatus,data,len);
    ModuleDir moduleDir;
    moduleDir.append(MD_MainMonitor);

    m_moduleDirs.append(moduleDir);
    m_cmds.append(MM_VehicleStatus);
    m_datas.append(QVariant::fromValue(vehicleStatus));

    return true;
}

bool  ZmqMsgPackage::g2uVehicleDelay(const void *data, int)
{
    VehicleDelay* pVehicleDelay = (VehicleDelay*)(data);
    m_imei = QLatin1String(reinterpret_cast<char*>(pVehicleDelay->imei),IMEI_LENGTH);

    ModuleDir moduleDir;
    moduleDir.append(MD_MainCamera);
    uint delayMs = pVehicleDelay->delayUs/1000;

    m_moduleDirs.append(moduleDir);
    m_cmds.append(MC_VehicleDelay);
    m_datas.append(QVariant::fromValue(delayMs));

    return true;
}

bool  ZmqMsgPackage::g2uRespRemotel(const void *data, int )
{
    RespRemoteControl *pRespRemoteControl = (RespRemoteControl*)data;
    m_imei = QLatin1String(reinterpret_cast<char*>(pRespRemoteControl->imei),IMEI_LENGTH);
    ModuleDir moduleDir;
    moduleDir.append(MD_MainCamera);
    uint remoteResule = pRespRemoteControl->result;
    qDebug()<<pRespRemoteControl->result<<"遥控返回结果********************";
    m_moduleDirs.append(moduleDir);
    m_cmds.append(MC_RemoteResule);
    m_datas.append(QVariant::fromValue(remoteResule));

    return true;
}

bool  ZmqMsgPackage::u2uForward(const void *data, int)
{
    IpcForwardData *forward = (IpcForwardData*)data;
    QByteArray array(reinterpret_cast<char*>(forward->forwardData),forward->length);
    QDataStream in(array);
    QVariant varData;
    in >> varData;
    QList<QVariant> variantList;
    variantList.append(QVariant::fromValue(forward->moduleType));
    variantList.append(QVariant::fromValue(forward->cmd));
    variantList.append(varData);

    ModuleDir moduleDir;
    moduleDir.append(MD_MainProcessor);

    m_moduleDirs.append(moduleDir);
    m_cmds.append(MP_IpcForward);
    m_datas.append(variantList);

    return true;
}

bool  ZmqMsgPackage::encodeIpcForward(QByteArray &dest, ModuleDir &moduleDir, int &cmd, QVariant &data)
{
    QByteArray array;
    QDataStream out(&array,QIODevice::ReadWrite);
    out << data;
    uint16_t len = sizeof(IpcForwardData) + sizeof(char) * array.length();

    auto  forwardptr = std::unique_ptr<IpcForwardData,void(*)(void*)>(static_cast<IpcForwardData*>(malloc(len)),free);

    forwardptr->moduleType = moduleDir.first();
    forwardptr->cmd = cmd;
    forwardptr->length = array.length();
    memcpy(forwardptr->forwardData,array.data(),forwardptr->length);

    dest = QByteArray(reinterpret_cast<char*>(forwardptr.get()),len);

    return true;
}

bool  ZmqMsgPackage::encodeReqRemote(QByteArray &dest, QString &imei,uint8_t cmd)
{
    ReqRemoteControl reqRemote = {{0x00},0x00};
    memcpy(reqRemote.imei,imei.toUtf8().data(),IMEI_LENGTH);
    reqRemote.opt = cmd;
    dest = QByteArray(reinterpret_cast<char*>(&reqRemote),sizeof(reqRemote));

    return true;
}

bool  ZmqMsgPackage::encodeReqSendHmiMsg(QByteArray &dest, QString &imei,QByteArray &data)
{
    HmiMsg reqHmiMsg = {{0x00},0x00,{0x00}};
    memcpy(reqHmiMsg.imei,imei.toUtf8().data(),IMEI_LENGTH);
    reqHmiMsg.bodyLen = data.size();
    memcpy(reqHmiMsg.body,data.constData(),reqHmiMsg.bodyLen);
    dest = QByteArray(reinterpret_cast<char*>(&reqHmiMsg),sizeof(HmiMsg));

    return true;
}


