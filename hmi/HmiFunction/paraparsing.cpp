#include "paraparsing.h"
#include "log.h"

ParaParsing* ParaParsing::handle = NULL;
QMutex ParaParsing::paraMutex;

ParaParsing* ParaParsing::instance()
{
    if(handle == NULL) {
        QMutexLocker locker(&paraMutex);
        if(handle == NULL) {
            handle = new ParaParsing();
        }
    }
    return handle;
}

ParaParsing::ParaParsing()
{
    pubParsing();
    excavatorParsing();
    minetrunckParsing();
    crusherParsing();
    diggerParsing();
    collectorParsing();
    parkingParsing();
}

void ParaParsing::pubParsing(void)
{
    QSettings *set = new QSettings("/conf/hmi/public.ini", QSettings::IniFormat);
    paraSt.pub.vehicleType = set->value("/public/vehicleType",QString("1")).toInt();
    paraSt.pub.vehicleWidth = set->value("/public/vehicleWidth",QString("3.26")).toDouble();
    paraSt.pub.vehicleLength = set->value("/public/vehicleLength",QString("10")).toDouble();
    paraSt.pub.vehicleHight = set->value("/public/vehicleHight",QString("3.828")).toDouble();
    paraSt.pub.localIp = set->value("/public/localIp", QString("192.168.20.30")).toString();
    paraSt.pub.localPort = set->value("/public/localPort", 10087).toInt();
    paraSt.pub.remoteIp = set->value("/public/remoteIp", QString("192.168.20.20")).toString();
    paraSt.pub.remotePort = set->value("/public/remotePort", 10087).toInt();
    paraSt.pub.compatibleVer = set->value("/public/compatibleVer").toInt();
    paraSt.pub.mainVer =  set->value("/public/mainVer").toInt();
    paraSt.pub.subVer = set->value("/public/subVer").toInt();
    paraSt.pub.mapPath = set->value("/public/mapPath", QString("/storage/mapinfo")).toString();
    paraSt.pub.taskPath = set->value("/public/taskPath", QString("/sotrage/taskinfo")).toString();
    paraSt.pub.boundaryPath = set->value("/public/boundaryPath", QString("/storage/upload")).toString();
    paraSt.pub.roadPath = set->value("/public/roadPath", QString("/storage/upload")).toString();
    paraSt.pub.permissionPath = set->value("/public/permissionPath", QString("/storage/permissioninfo")).toString();
    delete set;
}

void ParaParsing::excavatorParsing(void)
{
    QSettings *set = new QSettings("/conf/hmi/excavator.ini", QSettings::IniFormat);
    paraSt.excavator.vehicleLength = set->value("/excavator/vehicleLength").toDouble();
    paraSt.excavator.vehicleWidth = set->value("/excavator/vehicleWidth").toDouble();
    paraSt.excavator.vehicleHight = set->value("/excavator/vehicleHight").toDouble();
    paraSt.excavator.fixedPoint = set->value("/stop/fixedPoint").toBool();
    paraSt.excavator.stopLat = set->value("/stop/stopLat").toDouble();
    paraSt.excavator.stopLng = set->value("/stop/stopLng").toDouble();
    paraSt.excavator.stopHeading = set->value("/stop/stopHeading").toDouble();
    paraSt.excavator.stopLength = set->value("/stop/stopLength").toDouble();
    paraSt.excavator.stopWidth = set->value("/stop/stopWidth").toDouble();
    paraSt.excavator.stopCompAngle = set->value("/stop/stopCompAngle").toDouble();
    delete set;
}

void ParaParsing::minetrunckParsing(void)
{
    QSettings *set = new QSettings("/conf/hmi/minetrunck.ini", QSettings::IniFormat);
    paraSt.minetrunck.vehicleLength = set->value("/minetrunck/vehicleLength").toDouble();
    paraSt.minetrunck.vehicleWidth = set->value("/minetrunck/vehicleWidth").toDouble();
    delete set;
}

void ParaParsing::diggerParsing(void)
{
    QSettings *set = new QSettings("/conf/hmi/digger.ini", QSettings::IniFormat);
    paraSt.digger.boundaryFixValue = set->value("/digger/boundaryFixValue").toDouble();
    paraSt.digger.vehicleLength = set->value("/digger/vehicleLength").toDouble();
    paraSt.digger.vehicleWidth = set->value("/digger/vehicleWidth").toDouble();
    paraSt.digger.gpsHeightofVehicle = set->value("/digger/gpsHeightofVehicle").toFloat();

    delete set;
}

void ParaParsing::collectorParsing(void)
{
    QSettings *set = new QSettings("/conf/hmi/collector.ini", QSettings::IniFormat);
    paraSt.collector.boundaryFixValue = set->value("/collector/boundaryFixValue").toDouble();
    paraSt.collector.gpsHeightofVehicle = set->value("/collector/gpsHeightofVehicle").toFloat();
    delete set;
}

void ParaParsing::crusherParsing(void)
{
    QSettings *set = new QSettings("/conf/hmi/crusher.ini", QSettings::IniFormat);
    paraSt.crusher.vehicleLength = set->value("/crusher/vehicleLength").toDouble();
    paraSt.crusher.vehicleWidth = set->value("/crusher/vehicleWidth").toDouble();
    paraSt.crusher.vehicleHight = set->value("/crusher/vehicleHight").toDouble();
    paraSt.crusher.stopLat = set->value("/crusher/stopLat","0").toDouble();
    paraSt.crusher.stopLng = set->value("/crusher/stopLng","0").toDouble();
    paraSt.crusher.stopHeading = set->value("/crusher/stopHeading","0").toDouble();

    delete set;
}

void ParaParsing::parkingParsing(void)
{
    QSettings *set = new QSettings("/conf/hmi/parking.ini", QSettings::IniFormat);
    paraSt.parking.vehicleLength = set->value("/parking/vehicleLength").toDouble();
    paraSt.parking.vehicleWidth = set->value("/parking/vehicleWidth").toDouble();
    paraSt.parking.vehicleHight = set->value("/parking/vehicleHight").toDouble();

    //车亭坐标
    paraSt.parking.Lat = set->value("/parking/Lat","0").toDouble();
    paraSt.parking.Lng = set->value("/parking/Lng","0").toDouble();
    paraSt.parking.Heading = set->value("/parking/Heading","0").toFloat();

    //停车点4个，为0则不在地图上显示
    paraSt.parking.stop1Lat = set->value("/parking/stop1Lat","0").toDouble();
    paraSt.parking.stop1Lng = set->value("/parking/stop1Lng","0").toDouble();
    paraSt.parking.stop1Heading = set->value("/parking/stop1Heading","0").toFloat();


    paraSt.parking.stop2Lat = set->value("/parking/stop2Lat","0").toDouble();
    paraSt.parking.stop2Lng = set->value("/parking/stop2Lng","0").toDouble();
    paraSt.parking.stop2Heading = set->value("/parking/stop2Heading","0").toFloat();


    paraSt.parking.stop3Lat = set->value("/parking/stop3Lat","0").toDouble();
    paraSt.parking.stop3Lng = set->value("/parking/stop3Lng","0").toDouble();
    paraSt.parking.stop3Heading = set->value("/parking/stop3Heading","0").toFloat();


    paraSt.parking.stop4Lat = set->value("/parking/stop4Lat","0").toDouble();
    paraSt.parking.stop4Lng = set->value("/parking/stop4Lng","0").toDouble();
    paraSt.parking.stop4Heading = set->value("/parking/stop4Heading","0").toFloat();
    delete set;
}

void ParaParsing::writedigger(double fixvalue, double length, double width)
{
    QSettings *set = new QSettings("/conf/hmi/digger.ini", QSettings::IniFormat);
    paraSt.digger.boundaryFixValue = fixvalue;
    paraSt.digger.vehicleLength = length;
    paraSt.digger.vehicleWidth = width;

    set->setValue("/digger/boundaryFixValue", fixvalue);
    set->setValue("/digger/vehicleLength", length);
    set->setValue("/digger/vehicleWidth", width);

    system("sync");
    delete set;
}

void ParaParsing::setPara(QString filePath,QString key,QString value)
{
    QSettings *set = new QSettings(filePath, QSettings::IniFormat);
    set->setValue(key,value);
    delete set;
}
