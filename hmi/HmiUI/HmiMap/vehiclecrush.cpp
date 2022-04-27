#include "vehiclecrush.h"

int VehicleCrush::getType()
{
    return type;
}

void VehicleCrush::setType(char type,uint8_t personalization)
{
    this->type = type;

    nameItem->setPos((typeItem->boundingRect().width() / 2) - (nameItem->boundingRect().width() / 2), typeItem->boundingRect().height());
}

QString VehicleCrush::getName()
{
    return name;
}

void VehicleCrush::setName(QString name)
{
    this->name = name;
}

char VehicleCrush::getState()
{
    return state;
}

void VehicleCrush::setState(char state)
{
    this->state = state;
}
