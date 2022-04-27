#include "vehicleexcavator.h"


int VehicleExcavator::getType()
{
    return type;
}

void VehicleExcavator::setType(char type,uint8_t personalization)
{
    this->type = type;

    nameItem->setPos((typeItem->boundingRect().width() / 2) - (nameItem->boundingRect().width() / 2), typeItem->boundingRect().height());
}

QString VehicleExcavator::getName()
{
    return name;
}

void VehicleExcavator::setName(QString name)
{
    this->name = name;
}


char VehicleExcavator::getState()
{
    return state;
}

void VehicleExcavator::setState(char state)
{
    this->state = state;
}
