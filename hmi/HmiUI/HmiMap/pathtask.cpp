#include "pathtask.h"

PathTask::PathTask(QList<latlngheading* >* datalist) : PathFrame(datalist)
{
    color = QColor(27, 255, 183);
}

QRectF PathTask::boundingRect() const
{
    QRectF rect(0, 0, (lngmax*equator - lngmin*equator), (latmax*equator - latmin*equator));
    return rect;
}

void PathTask::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setBackground(Qt::NoBrush);

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(QPen(color, 8));

    for(int count = 1; count < nodeList->size(); count++)
    {
        QPointF last;

        last.setX(nodeList->at(count)->lng* equator - lngmin*equator);
        last.setY(latmax* equator - nodeList->at(count)->lat*equator);
        if(nodeList->at(count)->flag&1)//走过的路
        {
            color = QColor(255, 0, 0);
            painter->setPen(QPen(color, 10));
        }
        else
        {
            color = QColor(27, 255, 183);
            painter->setPen(QPen(color, 8));
        }
        painter->drawPoint(last);
    }
}

void PathTask::calcFinishedTask(latlngheading pos)
{
    uint8_t found=0;
    for(int count = nodeList->size()-1; count >= 1; count--)
    {
        if(found)
            nodeList->at(count)->flag|=1;
        else
        {
            if(getDistance(pos.lng, pos.lat, nodeList->at(count)->lng, nodeList->at(count)->lat)<1)
                found=1;
        }

    }
}
