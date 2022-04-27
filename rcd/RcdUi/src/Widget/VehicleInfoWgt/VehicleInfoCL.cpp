#include "VehicleInfoCL.h"
#include "../../Protocol/hmicmd.h"
#include <QDebug>
#include <QTimer>

VehicleInfoCL::VehicleInfoCL(QObject *parent) : IController(parent)
{

}

VehicleInfoCL::~VehicleInfoCL()
{

}

void VehicleInfoCL::doInit()
{
    qInfo()<<"VehicleInfoCL::doInit()";
}

void VehicleInfoCL::doUnInit()
{

}

bool VehicleInfoCL::exitAction()
{
    return true;
}

bool VehicleInfoCL::intoAction()
{

    return true;
}

void VehicleInfoCL::getFromAny(ModuleDir moduleDir,int cmd,QVariant data)
{
    moduleDir.clear();
    VehicleInfoWgtCmdType Cmd = static_cast<VehicleInfoWgtCmdType>(cmd);

    switch (Cmd) {
    case VI_LoadNewUrl:{
        QString url = data.toString();
        break;
    }
    case VI_G2HmiInfo:{
        this->g2hCmdMgsDeal(data);
        break;
    }
    case VI_Oil:{
        m_pModel->setOilValue(data.toList()[0].toFloat());
    }
    default:{break;}
    }

}

void VehicleInfoCL::g2hCmdMgsDeal(QVariant &data)
{
    QVariantList datalist = data.toList();
    uint16_t cmd = datalist.first().toUInt();
    datalist.removeFirst();
    QString tmpStr(QDateTime::currentDateTime().toString("hh:mm:ss : "));

    switch (cmd) {
    case G2H_MASTER_INFO_CMD:{
        auto it = c_MasterInfoMap.find(datalist.last().toUInt());
        if(it != c_MasterInfoMap.end()){
            tmpStr += QString::fromStdString(it->second);
            m_pModel->addSysInfo(tmpStr);
        }
    }break;
    case G2H_POS_CMD:{

        auto it = c_PosWorkInfoMap.find(datalist.at(0).toInt());
        if(it != c_PosWorkInfoMap.end()){
            QString into = tmpStr + QString::fromStdString(it->second);
            m_pModel->addSysInfo(into);
        }

        it = c_PosAiInfoMap.find(datalist.at(1).toInt());
        if(it != c_PosAiInfoMap.end()){
            QString into = tmpStr + QString::fromStdString(it->second);
            m_pModel->addSysInfo(into);
        }

        it = c_PosLoadInfoMap.find(datalist.at(2).toInt());
        if(it != c_PosLoadInfoMap.end()){
            QString into = tmpStr + QString::fromStdString(it->second);
            m_pModel->addSysInfo(into);
        }

        it = c_PosTaskInfoMap.find(datalist.at(3).toInt());
        if(it != c_PosTaskInfoMap.end()){
            QString into = tmpStr + QString::fromStdString(it->second);
            m_pModel->addSysInfo(into);
        }

        if(datalist.at(4).toInt() > 0){
            QString into = tmpStr + "车辆超速";
            m_pModel->addSysInfo(into);
        }

    }break;
    case G2H_RES_WORK_EXCAVATOR_OPT_CMD:{

        auto it = c_ResWorkExcavatorOptInfoMap.find(datalist.last().toUInt());
        if(it != c_ResWorkExcavatorOptInfoMap.end()){
            tmpStr += QString::fromStdString(it->second);
            m_pModel->addSysInfo(tmpStr);
        }

    }break;
    case G2H_RES_WORK_CRUSHER_OPT_CMD:{

        auto it = c_ResWorkCrusherOptInfoMap.find(datalist.last().toUInt());
        if(it != c_ResWorkCrusherOptInfoMap.end()){
            tmpStr += QString::fromStdString(it->second);
            m_pModel->addSysInfo(tmpStr);
        }

    }break;
    case G2H_RES_TASK_CMD:{
        QString tStr;
        tStr.append("接收到");

        auto it = c_ResTaskCmdInfoMap.find(datalist.last().toUInt());
        if(it != c_ResTaskCmdInfoMap.end()){
            tStr.append(it->second.c_str());
        }
        tmpStr += tStr;
        m_pModel->addSysInfo(tmpStr);

    }break;
    case G2H_REQ_WORKON_STATU_CMD:{

        auto it = c_ReqWorkonStateInfoMap.find(datalist.last().toUInt());
        if(it != c_ReqWorkonStateInfoMap.end()){
            tmpStr += QString::fromStdString(it->second);
            m_pModel->addSysInfo(tmpStr);
        }

    }break;
    case G2H_DRIVE_PERMISSION_CMD:{

        QString tStr;
        tStr.append("接收到新的行车许可");
        tmpStr += tStr;
        m_pModel->addSysInfo(tmpStr);

    }break;
    case G2H_RES_TEST_CMD:{
        QString tStr;

        struct pubMasterTestST tResTest = datalist.first().value<pubMasterTestST>();
        if(tResTest.taskType == 0x01){
            tStr.append("动态测试");
        }else if(tResTest.taskType == 0x02){
            tStr.append("制动测试");
        }else if(tResTest.taskType == 0x03){
            tStr.append("举升测试");
        }else{
            tStr.append("未知任务:" + QString::number(tResTest.taskType));
        }

        if(tResTest.result == 0x01){
            tStr = "正在进行" + tStr;
        }else if(tResTest.result == 0x02){
            tStr.append("成功");

            if(tResTest.taskType == 0x03){
                tStr.append(",静态测试完成");
            }else if(tResTest.taskType == 0x01){
                tStr.append(",动态测试完成");
            }

        }else if(tResTest.result == 0x03){
            tStr.append("失败");
            if(tResTest.reason == 0x00){
                tStr.append("失败，原因:未知");
            }else if(tResTest.reason == 0x01){
                tStr.append(",停靠误差过大,请求人工接管");
            }else{
                tStr.append("失败，原因:" + QString::number(tResTest.reason));
            }
        }else{
            tStr = QString("结果未知:%1,原因未知:%2").arg(QString::number(tResTest.result))
                                                   .arg(QString::number(tResTest.reason));
        }

        tmpStr += tStr;
        m_pModel->addSysInfo(tmpStr);

    }break;
    default:
        break;
    }
}

void VehicleInfoCL::setModel(VehicleInfoModel *model)
{
    m_pModel = model;
}
