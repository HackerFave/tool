#ifndef VEHICLEPIXMAPITEM_H
#define VEHICLEPIXMAPITEM_H

#include <QObject>
#include <QPainter>

#include <QDebug>

#include <QGraphicsPixmapItem>

class VehiclePixmapItem : public QGraphicsPixmapItem
{
public:
    explicit VehiclePixmapItem(QString path, QSize size);
    explicit VehiclePixmapItem(QString path);
    explicit VehiclePixmapItem();

    QSize getVehiclePixmapSize();
    void setVehiclePixmapSize(QSize size);

    QPixmap getVehiclePixmap();
    void setVehiclePixmap(QString path);

    QPixmap vehiclePix;
};

#endif // VEHICLEPIXMAPITEM_H
