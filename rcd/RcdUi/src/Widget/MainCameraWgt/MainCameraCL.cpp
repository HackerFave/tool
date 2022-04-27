#include "MainCameraCL.h"
#include <QStringList>
#include <QDebug>
#include <QSettings>
#include "../include/qtzmq.h"
#include "public/publicclass.h"
#include <QDir>
#include <QDateTime>
MainCameraCL::MainCameraCL(QObject *parent) : IController(parent)
{

}

MainCameraCL::~MainCameraCL()
{

}

void MainCameraCL::doInit()
{
    qInfo()<<"MainCameraCL::doInit()";
}

void MainCameraCL::doUnInit()
{

}

bool MainCameraCL::exitAction()
{
    return true;
}

bool MainCameraCL::intoAction()
{
    QString currImei = System::getCurrentVehicle();
    QSettings config("rcdconfig.ini",QSettings::IniFormat);
    m_pModel->setRemoteStatus(MainCameraModel::RS_Monitoring);
    m_pModel->setCameraUrl(config.value(QString("%1/Forward").arg(currImei)).toString(),
                           config.value(QString("%1/Backward").arg(currImei)).toString());

    return true;
}

// 处理模块支持的命令
void MainCameraCL::getFromAny(ModuleDir,int cmd,QVariant data)
{
    MainCameraWgtCmdType Cmd = static_cast<MainCameraWgtCmdType>(cmd);
    switch (Cmd) {
    case MC_RemoteResule:{
        uint resule = data.toUInt();
        _currentStatus = data.toUInt();
        switch (resule) {
        case RESP_REMOTE_CONTROL_VEHICLE_RECV:{
            if(isRemoteApply){
                resule = MainCameraModel::RS_RemoteControl;
            }else{
                resule = MainCameraModel::RS_Monitoring;
            }
            break;
        }
        case RESP_REMOTE_CONTROL_VEHICLE_DISCONNECT:{
            resule = MainCameraModel::RS_RemoteControlDisconnect;
            break;
        }
        default:{
            resule = MainCameraModel::RS_RemoteControlFail;
            break;
        }
        }
        m_pModel->setRemoteStatus(resule);
        break;
    }
    case MC_VehicleDelay:{
        m_pModel->setVehicleDelay(data.toUInt());
        break;
    }
    case MC_ThrottleBrakeWheel:{
        if(data.toList().size()==3)
        {
            m_pModel->setThrottleBrakeWheel(data.toList()[0].toFloat(),
                    data.toList()[1].toFloat(),
                    data.toList()[2].toFloat());
        }

        break;
    }
    case MC_Brake:{//刹车踏板百分比
        m_pModel->setBrake(data.toFloat());
        break;
    }
    case MC_Throttle_RingDila:{
        m_pModel->setThrottle(data.toList().at(0).toFloat());//加速踏板百分比
        m_pModel->setSpeedRaw(data.toList().at(1).toFloat());//发动机转速
        break;
    }
        //    case MC_Throttle:{
        //        m_pModel->setThrottle(data.toFloat());
        //        break;
        //    }
    case MC_Wheel:{
        m_pModel->setWheel(data.toFloat());
        break;
    }/*
    case MC_Speed:{
        m_pModel->setSpeed(data.toFloat());
        break;
    }
    case MC_Gear:{
        m_pModel->setGear(data.toFloat());
        break;
    }*/
    case MC_Speed_Gear:{//档位加速度
        float gear = data.toList().at(0).toFloat();
        if(gear<0){
            m_pModel->setGear(-1);//倒档位
        }else if(gear>0) {
             m_pModel->setGear(1);//前进档位
        }
        else {
             m_pModel->setGear(0);//空档位
        }

        m_pModel->setSpeed(data.toList().at(1).toFloat());//速度
        break;
    }
    case MC_InputKey:{
        inputHandler(static_cast<Qt::Key>(data.toInt()));
        qDebug()<<static_cast<Qt::Key>(data.toInt())<<"遥控驾驶申请22222222222222222222222";
        break;
    }
    case MC_PlayUrl:{
        QSettings config("rcdconfig.ini",QSettings::IniFormat);
        QString currImei = System::getCurrentVehicle();
        m_pModel->setCameraUrl(config.value(QString("%1/Forward").arg(currImei)).toString(),
                               config.value(QString("%1/Backward").arg(currImei)).toString());
        break;
    }
    case MC_VehicleData:{
        //        qDebug()<<"当前车型信息-------------------："<<data.toString();
        break;
    }
    case MC_Other:{
        QList<QVariant> list = data.toList();
        for(int i=0;i<list.size();i++){
            //            qDebug()<<"测试数据-------------------："<<list.at(i).toString();

        }
        break;
    }
    default:{break;}
    }

}

// 控制按键处理
void MainCameraCL::inputHandler(Qt::Key key)
{
    qDebug()<<"MainCameraCL::inputHandler"<<key;

    if(key == Qt::Key_Escape/*Qt::Key_Up*/){
        ModuleDir moduleDir;
        moduleDir.append(MD_IPCManager);
        emit sendToMain(moduleDir,IM_ReqRemoteEnd);

        moduleDir.clear();
        moduleDir.append(MD_MainProcessor);
        emit sendToMain(moduleDir,MP_SwitchLayout,LT_Monitor);
    }else if(key == Qt::Key_PageUp){

        if(_currentStatus==2){
        qDebug()<< static_cast<Qt::Key>(key)<<"MainCameraCL::当前状态××××××××××××××××××";
        isRemoteApply = true;
        ModuleDir moduleDir;
        moduleDir.append(MD_IPCManager);
        emit sendToMain(moduleDir,IM_ReqRemoteStart);
        }
        else {
            qDebug()<< static_cast<Qt::Key>(key)<<"MainCameraCL::当前状态××××××××××××××××××";
           isRemoteApply = false;
           ModuleDir moduleDir;
           moduleDir.append(MD_IPCManager);
           emit sendToMain(moduleDir,IM_ReqRemoteEnd);
        }
    }/*else if(key == Qt::Key_PageDown){
         qDebug()<< static_cast<Qt::Key>(key)<<"MainCameraCL::当前状态××××××××××××××××××";
        isRemoteApply = false;
        ModuleDir moduleDir;
        moduleDir.append(MD_IPCManager);
        emit sendToMain(moduleDir,IM_ReqRemoteEnd);
    }*/
}

void MainCameraCL::setModel(MainCameraModel *model)
{
    m_pModel = model;
}
