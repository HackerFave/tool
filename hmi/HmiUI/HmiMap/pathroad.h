#ifndef PATHROAD_H
#define PATHROAD_H

#include <QObject>
#include "pathframe.h"

class PathRoad : public PathFrame
{
    Q_OBJECT
public:
    explicit PathRoad(QList<latlngheading *>* datalist);

private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
signals:

public slots:
};

#endif // PATHROAD_H
