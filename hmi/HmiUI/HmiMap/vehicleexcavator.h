#ifndef VEHICLEEXCAVATOR_H
#define VEHICLEEXCAVATOR_H

#include <QObject>

#include "vehicleframe.h"

class VehicleExcavator : public VehicleFrame
{
public:
    VehicleExcavator(char type, QString name, QSize size, uint8_t personalization) : VehicleFrame(type, name, size, personalization){}

    int getType();
    void setType(char,uint8_t personalization);
    QString getName();
    void setName(QString);
    char getState();
    void setState(char state);
};

#endif // VEHICLEEXCAVATOR_H
