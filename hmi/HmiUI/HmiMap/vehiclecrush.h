#ifndef VEHICLECRUSH_H
#define VEHICLECRUSH_H

#include <QObject>

#include "vehicleframe.h"

class VehicleCrush : public VehicleFrame
{
public:
    VehicleCrush(char type, QString name, QSize size, uint8_t personalization) : VehicleFrame(type, name, size,personalization){}

    int getType();
    void setType(char,uint8_t personalization);
    QString getName();
    void setName(QString);
    char getState();
    void setState(char state);
};

#endif // VEHICLECRUSH_H
