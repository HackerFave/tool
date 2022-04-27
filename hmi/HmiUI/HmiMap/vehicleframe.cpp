#include "vehicleframe.h"
#include <QDebug>
VehicleFrame::VehicleFrame(char type, QString name, QSize size, uint8_t personalization)
{
    this->type = type;
    this->name = name;
    this->heading = 0;
    this->size = size;
    off=QPoint(0,0);

    nameItem = new QGraphicsTextItem;
    typeItem = new QGraphicsPixmapItem;
    group = new QGraphicsItemGroup;

    nameItem->setFont(QFont("Microsoft YaHei"));
    nameItem->setDefaultTextColor(Qt::white);
    nameItem->setPlainText(name);

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
        case EXCAVATOR:
        {
            if(personalization==0)
                typeItem->setPixmap(QPixmap(":/image/resource/selfexcavator.png").scaled(size, Qt::KeepAspectRatio));
            else if(personalization==1)
                typeItem->setPixmap(QPixmap(":/image/resource/excavator.png").scaled(size, Qt::KeepAspectRatio));
            break;
        }
        case DIGGER:
        {
            if(personalization==0)
                typeItem->setPixmap(QPixmap(":/image/resource/selfdigger.png").scaled(size, Qt::KeepAspectRatio));
            else if(personalization==1)
                typeItem->setPixmap(QPixmap(":/image/resource/digger.png").scaled(size, Qt::KeepAspectRatio));
            break;
        }
        case CRUSHER:
        {
            if(personalization==0)
                typeItem->setPixmap(QPixmap(":/image/resource/selfcrush.png").scaled(size, Qt::KeepAspectRatio));
            else if(personalization==1)
                typeItem->setPixmap(QPixmap(":/image/resource/crushMap.png").scaled(size, Qt::KeepAspectRatio));
            break;
        }
        case PARKING:
        {
            typeItem->setPixmap(QPixmap(":/image/resource/parkingMap.png").scaled(size, Qt::KeepAspectRatio));
            break;
        }
        case LOADER:
        {
            break;
        }
        case COLLECTOR:
        {
            if(personalization==0)
                typeItem->setPixmap(QPixmap(":/image/resource/selfcollector.png").scaled(size, Qt::KeepAspectRatio));
            else if(personalization==1)
                typeItem->setPixmap(QPixmap(":/image/resource/collector.png").scaled(size, Qt::KeepAspectRatio));
            break;
        }
        default:
            break;
    }

    nameItem->setPos((typeItem->boundingRect().width() / 2) - (nameItem->boundingRect().width() / 2), typeItem->boundingRect().height());

    group->addToGroup(nameItem);
    group->addToGroup(typeItem);

    typeItem->setTransformOriginPoint(typeItem->boundingRect().center());
    group->setTransformOriginPoint(typeItem->boundingRect().center());
//    qDebug()<<group->boundingRect().width()<<group->boundingRect().height();
//    group->translate(group->boundingRect().width()/2,group->boundingRect().height()/2);
}

VehicleFrame::~VehicleFrame()
{
    delete nameItem;
    delete typeItem;
    delete group;
}

//QRectF VehicleFrame::boundingRect() const
//{
//    qreal x=group->boundingRect().x();
//    qreal y=group->boundingRect().y();
//    return QRectF(-x/2,-y/2,x,y);
//}

void VehicleFrame::setHeading(float heading)
{
    this->heading = heading;
    typeItem->setRotation(heading);
}

float VehicleFrame::getHeading()
{
    return heading;
}

void VehicleFrame::setPointOffset(QPointF offset)
{
    off=offset;
}
void VehicleFrame::setPos(QPointF pos)
{
    group->setPos(pos);
}

void VehicleFrame::setPos(double lng, double lat)
{
    lng-=100;//不知道怎么设置item的中心，暂时这么玩吧
    lat-=100;
    lng-=off.x();
    lat-=off.y();
    group->setPos(lng, lat);
    typeItem->setPos(lng , lat);
    nameItem->setPos(lng + (typeItem->boundingRect().width() / 2) - (nameItem->boundingRect().width() / 2), lat + typeItem->boundingRect().height());
}

QPointF VehicleFrame::getPos()
{
    return group->pos();//若用到该函数，请将偏移加上，setpos
}

void VehicleFrame::setImei(QByteArray imei)
{
    memmove(Imei, imei.left(15).data(), 15);
}

QByteArray VehicleFrame::getImei()
{
    return QByteArray(Imei,15);
}
