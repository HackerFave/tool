#ifndef PATHFRAME_H
#define PATHFRAME_H

#include <QObject>

#include <QGraphicsItem>
#include <QPainter>

#include "pub.h"

class PathFrame : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit PathFrame(QList<latlngheading *>* datalist);
    ~PathFrame();
    void setNum(char num)
    {
        this->num = num;
    }
    void setType(char type,uint8_t personalization)
    {
        this->type = type;
    }
    void setColor(QColor color)
    {
        this->color = color;
    }
    QPointF getLeftTopPoint()
    {
        return QPointF(lngmin, latmax);
    }

public:
    int num;
    int type;
    QColor color;

    double latmin;
    double latmax;
    double lngmin;
    double lngmax;

    QList<latlngheading*>* nodeList;
};

#endif // PATHFRAME_H
