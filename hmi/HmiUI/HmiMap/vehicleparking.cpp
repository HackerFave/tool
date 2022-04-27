#include "vehicleparking.h"

int VehicleParking::getType()
{
    return type;
}

void VehicleParking::setType(char type,uint8_t personalization)
{
    this->type = type;

    nameItem->setPos((typeItem->boundingRect().width() / 2) - (nameItem->boundingRect().width() / 2), typeItem->boundingRect().height());
}

QString VehicleParking::getName()
{
    return name;
}

void VehicleParking::setName(QString name)
{
    this->name = name;
}

char VehicleParking::getState()
{
    return state;
}

void VehicleParking::setState(char state)
{
    this->state = state;
}
