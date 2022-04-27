#include "WgtFactory.h"
#include <QDebug>

#include "Widget/MainCameraWgt/MainCameraWgt.h"
#include "Widget/RightCameraWgt/RightCameraWgt.h"
#include "Widget/LeftCameraWgt/LeftCameraWgt.h"
#include "Widget/CloudWebWgt/CloudWebWgt.h"
#include "Widget/VehicleInfoWgt/VehicleInfoWgt.h"
#include "Widget/MainMonitorWgt/MainMonitorWgt.h"
#include "Widget/DriveCameraWgt/DriveCameraWgt.h"
#include "Widget/AllInfoWgt/AllInfoWgt.h"
#include "Widget/WarnInfoWgt/WarnInfoWgt.h"


void WgtFactory::initWidget()
{
    //创建耗时的
    int procType = System::getProcessType();
    if(procType & PT_RightProc){
        RightCameraWgt::GetInstance();//右摄像头界面
        VehicleInfoWgt::GetInstance();//车辆信息界面
    }
    if(procType & PT_MidProc){
        MainCameraWgt::GetInstance();//主界面相机界面
        MainMonitorWgt::GetInstance();//监视器界面
    }
    if(procType & PT_LeftProc){
        CloudWebWgt::GetInstance();
        LeftCameraWgt::GetInstance();
        DriveCameraWgt::GetInstance();
    }
}

IFunWidget * WgtFactory::getFunWidget(ModuleType type)
{
    IFunWidget *pFunWidget = nullptr;
    switch (type) {
        case MD_MainCamera:
        {
            //qInfo() << "MD_MainCamera";
            pFunWidget = MainCameraWgt::GetInstance();
        }
        break;
        case MD_MainMonitor:
        {
            //qInfo() << "MD_MainMonitor";
            pFunWidget = MainMonitorWgt::GetInstance();
        }
        break;
        case MD_RightCamera:
        {
            //qInfo() << "MD_RightCamera";
            pFunWidget = RightCameraWgt::GetInstance();
        }
        break;
        case MD_LeftCamera:
        {
            //qInfo() << "MD_LeftCamera";
            pFunWidget = LeftCameraWgt::GetInstance();
        }
        break;
        case MD_DriveCamera:
        {
            //qInfo() << "MD_DriveCamera";
            pFunWidget = DriveCameraWgt::GetInstance();
        }
        break;
        case MD_CloudWeb:
        {
            //qInfo() << "MD_CloudWeb";
            pFunWidget = CloudWebWgt::GetInstance();
        }
        break;
        case MD_VehicleInfo:
        {
            //qInfo() << "MD_VehicleInfo";
            pFunWidget = VehicleInfoWgt::GetInstance();
        }
        break;
        case MD_AllInfo:
        {
            //qInfo() << "MD_AllInfo";
            pFunWidget = AllInfoWgt::GetInstance();
        }
        break;
        case MD_WarnInfo:
        {
            //qInfo() << "MD_WarnInfo";
            pFunWidget = WarnInfoWgt::GetInstance();
        }
        break;
        default:
        {
            qWarning() << "can't find this ModuleType";
            break;
        }
    }

    return pFunWidget;
}

