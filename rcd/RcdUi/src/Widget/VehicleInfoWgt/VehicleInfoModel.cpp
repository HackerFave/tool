#include "VehicleInfoModel.h"
#include <QDateTime>

VehicleInfoModel::VehicleInfoModel(QObject *parent) : IModel(parent)
{
//    m_allInfo.append(QDateTime::currentDateTime().toString("hh:mm:ss : ") + QString::fromStdString(c_ResWorkExcavatorOptInfoMap.find(PUB_WORK_OPT_OUT_TASK)->second));
//    m_allInfo.append("1122\r\n");
//    m_allInfo.append(QDateTime::currentDateTime().toString("hh:mm:ss : ") + QString::fromStdString(c_PosWorkInfoMap.find(PUB_VEHICLE_DEALY_WORK_STATU)->second));
//    m_allInfo.append("\r\n");
//    m_allInfo.append(QDateTime::currentDateTime().toString("hh:mm:ss : ") + QString::fromStdString(c_PosWorkInfoMap.find(PUB_VEHICLE_FAULT_WORK_STATU)->second));
//    m_allInfo.append("\r\n");
//    m_allInfo.append(QDateTime::currentDateTime().toString("hh:mm:ss : ") + QString::fromStdString(c_PosWorkInfoMap.find(PUB_VEHICLE_FAULT_WORK_STATU)->second));
//    m_allInfo.append("\r\n");
//    m_allInfo.append(QDateTime::currentDateTime().toString("hh:mm:ss : ") + QString::fromStdString(c_PosWorkInfoMap.find(PUB_VEHICLE_FAULT_WORK_STATU)->second));
//    m_allInfo.append("\r\n");
//    m_allInfo.append(QDateTime::currentDateTime().toString("hh:mm:ss : ") + QString::fromStdString(c_MasterInfoMap.find(0x03)->second) + QString::fromStdString(c_MasterInfoMap.find(0x03)->second)
//                  + QString::fromStdString(c_MasterInfoMap.find(0x03)->second)+ QString::fromStdString(c_MasterInfoMap.find(0x03)->second)
//                  + QString::fromStdString(c_MasterInfoMap.find(0x03)->second));
//    m_allInfo.append("\r\n");

}

void  VehicleInfoModel::addSysInfo(QString &info)
{
    m_newInfo = info;
    emit sendModelUpdate(MC_AddSysInfo);
}
void  VehicleInfoModel::setOilValue(float value)
{
    _oilvalue = value;
    emit sendModelUpdate(MC_Oil);
}
