#ifndef VEHICLEMINE_H
#define VEHICLEMINE_H

#include <QObject>

#include "vehicleframe.h"

class VehicleMine : public VehicleFrame
{
public:
    VehicleMine(char type, QString name, QSize size, uint8_t personalization) : VehicleFrame(type, name, size,personalization){}

    int getType();
    void setType(char);
    void setType(char,uint8_t);
    QString getName();
    void setName(QString);
    char getState();
    void setState(char state);
};

#endif // VEHICLEMINE_H
