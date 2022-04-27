#include "vehiclepixmapitem.h"

VehiclePixmapItem::VehiclePixmapItem(QString path, QSize size)
{
    QPixmap pix;
    pix.load(path);
    setPixmap(pix.scaled(size, Qt::KeepAspectRatio));
}

VehiclePixmapItem::VehiclePixmapItem(QString path)
{
    QPixmap pix;
    pix.load(path);
    setPixmap(pix);
}

VehiclePixmapItem::VehiclePixmapItem()
{
}

QSize VehiclePixmapItem::getVehiclePixmapSize()
{
    return this->boundingRect().size().toSize();
}

void VehiclePixmapItem::setVehiclePixmapSize(QSize size)
{
    this->boundingRect().setRect(0, 0, size.width(), size.height());
}

QPixmap VehiclePixmapItem::getVehiclePixmap()
{
    return this->pixmap();
}

void VehiclePixmapItem::setVehiclePixmap(QString path)
{
    QPixmap pix;
    pix.load(path);
    setPixmap(pix);

    qDebug()<<path;
}


