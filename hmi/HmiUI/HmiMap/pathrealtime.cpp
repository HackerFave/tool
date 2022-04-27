#include "pathrealtime.h"

PathRealTime::PathRealTime(struct collectlatlngheading*pos)
{
    latmax = pos->lat;
    lngmax = pos->lng;
    latmin = pos->lat;
    lngmin = pos->lng;

    centrallat = pos->lat;
    centrallng = pos->lng;

    color = Qt::white;
    list.clear();
    list.append(pos);
}

PathRealTime::~PathRealTime()
{
    if(!list.isEmpty())
        list.clear();
}

QRectF PathRealTime::boundingRect() const
{
    return QRectF(lngmin*equator - centrallng*equator, \
                  -latmin*equator + centrallat*equator, \
                  (lngmax*equator - lngmin*equator), \
                  (-latmax*equator + latmin*equator));
}

void PathRealTime::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setBackground(Qt::NoBrush);

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(QPen(color, 4));

    for(int count = 0; count < list.size()-1;)
    {
        QPointF front;
        QPointF back;

        front.setX(list.at(count)->lng*equator - centrallng*equator);
        front.setY(centrallat*equator - list.at(count)->lat*equator);

        back.setX(list.at(count + 1)->lng*equator - centrallng*equator);
        back.setY(centrallat*equator - list.at(count + 1)->lat*equator);

//        painter->drawPoint(front);
        if(!(list.at(count)->flag && list.at(count + 1)))
            painter->drawLine(front, back);

        if(list.at(count)->lat > latmax)
        {
            latmax = list.at(count)->lat;
        }

        if(list.at(count)->lng > lngmax)
        {
            lngmax = list.at(count)->lng;
        }

        if(list.at(count)->lat < latmin)
        {
            latmin = list.at(count)->lat;
        }

        if(list.at(count)->lng < lngmin)
        {
            lngmin = list.at(count)->lng;
        }
        count += 1;
    }
}

void PathRealTime::updateList(QList<struct collectlatlngheading *> &list)
{
    this->list = list;
    this->update();
}

