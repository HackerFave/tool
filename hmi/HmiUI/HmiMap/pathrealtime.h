#ifndef PATHREALTIME_H
#define PATHREALTIME_H

#include <QObject>

#include <QGraphicsItem>
#include <QPainter>

#include "pub.h"

class PathRealTime : public QGraphicsItem
{
public:
    PathRealTime(struct collectlatlngheading* pos);
    ~PathRealTime();
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    void updateList(QList<struct collectlatlngheading*>&);

    mutable QList<struct collectlatlngheading*> list;

    double latmax;
    double lngmax;
    double latmin;
    double lngmin;
    double centrallat;
    double centrallng;

    QColor color;
};

#endif // PATHREALTIME_H
