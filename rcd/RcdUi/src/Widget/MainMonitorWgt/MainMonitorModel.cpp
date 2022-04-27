#include "MainMonitorModel.h"
#include <QSettings>

MainMonitorModel::MainMonitorModel(QObject *parent) : IModel(parent),
    m_showVehicle(6,QLatin1String("")),
    m_select(0),
    m_pageIndex(0),
    m_maxPage(1)
{
    QSettings config("rcdconfig.ini",QSettings::IniFormat);

    VehicleInfo vehicle;
    vehicle.imei = QLatin1String("882918394121200");
    vehicle.url = config.value(QString("%1/Forward").arg(vehicle.imei)).toString();
    vehicle.name = config.value(QString("%1/Name").arg(vehicle.imei)).toString();
    m_vehicleMap[vehicle.imei] = vehicle;
    m_vehicleList.append(vehicle.imei);

//    vehicle.imei = QLatin1String("882918394121211");
//    vehicle.url = config.value(QString("%1/Forward").arg(vehicle.imei)).toString();
//    vehicle.name = config.value(QString("%1/Name").arg(vehicle.imei)).toString();
//    m_vehicleMap[vehicle.imei] = vehicle;
//    m_vehicleList.append(vehicle.imei);

//    vehicle.imei = QLatin1String("882918394121222");
//    vehicle.url = config.value(QString("%1/Forward").arg(vehicle.imei)).toString();
//    vehicle.name = config.value(QString("%1/Name").arg(vehicle.imei)).toString();
//    m_vehicleMap[vehicle.imei] = vehicle;
//    m_vehicleList.append(vehicle.imei);

//    vehicle.imei = QLatin1String("882918394121333");
//    vehicle.url = config.value(QString("%1/Forward").arg(vehicle.imei)).toString();
//    vehicle.name = config.value(QString("%1/Name").arg(vehicle.imei)).toString();
//    m_vehicleMap[vehicle.imei] = vehicle;
//    m_vehicleList.append(vehicle.imei);

}

bool MainMonitorModel::addVehicle(VehicleInfo &vehicle)
{
    bool ret = false;
    auto iter = m_vehicleMap.find(vehicle.imei);
    if(iter == m_vehicleMap.end()){
        m_vehicleList.append(vehicle.imei);
        ret = true;
    }
    m_vehicleMap[vehicle.imei] = vehicle;
    return ret;
}

bool MainMonitorModel::removeVehicle(QString &imei)
{
    m_vehicleList.removeOne(imei);
    m_vehicleMap.remove(imei);
    return true;
}
