#ifndef VEHICLEPARKING_H
#define VEHICLEPARKING_H

#include <QObject>

#include "vehicleframe.h"

class VehicleParking : public VehicleFrame
{
public:
    VehicleParking(char type, QString name, QSize size, uint8_t personalization) : VehicleFrame(type, name, size,personalization){}

    int getType();
    void setType(char,uint8_t personalization);
    QString getName();
    void setName(QString);
    char getState();
    void setState(char state);
};

#endif // VEHICLEPARKING_H
