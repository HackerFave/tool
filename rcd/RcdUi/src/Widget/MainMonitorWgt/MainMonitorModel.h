#ifndef MainMonitorModel_H
#define MainMonitorModel_H

#include <QMap>
#include <QVector>
#include "Interface/IModel.h"

class MainMonitorModel : public IModel
{
    Q_OBJECT
public:

    enum ModelCmdType
    {
        MC_UpdateAll=0,
        MC_UpdateVehicle,
        MC_UpdateSelect,

        MC_Max
    };

    struct VehicleInfo
    {
        QString   imei;
        QString   name;
        QString   url;
        float     speed;
        quint8    gear;

        VehicleInfo():imei(""),name(""),url(""),speed(0),gear(0)
        {};
    };



    explicit MainMonitorModel(QObject *parent = nullptr);
    bool addVehicle(VehicleInfo &vehicle);
    bool removeVehicle(QString &vehicle);

    QVector<QString>            m_showVehicle;

    QMap<QString,VehicleInfo>   m_vehicleMap;
    QList<QString>              m_vehicleList;
    quint8                      m_select;
    quint8                      m_pageIndex;
    quint8                      m_maxPage;

};

#endif // MainMonitorModel_H
