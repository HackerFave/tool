#include "pathcollection.h"

PathCollection::PathCollection(QList<latlngheading *>* datalist):PathFrame(datalist)
{
    color = Qt::black;
}

QRectF PathCollection::boundingRect() const
{
    QRectF rect(0, 0, (lngmax*equator - lngmin*equator), (latmax*equator - latmin*equator));
    return rect;
}

void PathCollection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setBackground(Qt::NoBrush);

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(QPen(color, 1));

    for(int count = 0; count < nodeList->size(); count++)
    {
        QPointF last;

        last.setX(nodeList->at(count)->lng*equator - lngmin*equator);
        last.setY(latmax*equator - nodeList->at(count)->lat*equator);

        painter->drawPoint(last);
    }
}
