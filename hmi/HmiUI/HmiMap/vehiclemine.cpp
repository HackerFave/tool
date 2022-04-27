#include "vehiclemine.h"



int VehicleMine::getType()
{
    return type;
}



void VehicleMine::setType(char type,uint8_t personalization)
{
    this->type = type;

    switch (type)
    {
        case MINELOADER:
        {
            if(personalization==0)
                typeItem->setPixmap(QPixmap(":/image/resource/selfmineload.png").scaled(size, Qt::KeepAspectRatio));
            else if(personalization==1)
                typeItem->setPixmap(QPixmap(":/image/resource/mineload.png").scaled(size, Qt::KeepAspectRatio));
            break;
        }
        case MINEUNLOADER:
        {
            if(personalization==0)
                typeItem->setPixmap(QPixmap(":/image/resource/selfmineunload.png").scaled(size, Qt::KeepAspectRatio));
            else if(personalization==1)
                typeItem->setPixmap(QPixmap(":/image/resource/mineunload.png").scaled(size, Qt::KeepAspectRatio));
            break;
        }
        default:
            break;
    }

    nameItem->setPos((typeItem->boundingRect().width() / 2) - (nameItem->boundingRect().width() / 2), typeItem->boundingRect().height());
}

QString VehicleMine::getName()
{
    return name;
}

void VehicleMine::setName(QString name)
{
    this->name = name;
}

char VehicleMine::getState()
{
    return state;
}

void VehicleMine::setState(char state)
{
    this->state = state;
}
