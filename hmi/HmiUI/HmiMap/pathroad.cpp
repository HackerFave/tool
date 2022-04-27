#include "pathroad.h"

PathRoad::PathRoad(QList<latlngheading *>* datalist) : PathFrame(datalist)
{
    color = QColor(207, 207, 207);
}

QRectF PathRoad::boundingRect() const
{
    QRectF rect(0, 0, (lngmax*equator - lngmin*equator), (latmax*equator - latmin*equator));
    return rect;
}

void PathRoad::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setBackground(Qt::NoBrush);

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(QPen(color, 1));

    for(int count = 0; count < nodeList->size()-1; count += 1)
    {
        QPointF front;
        QPointF back;

        front.setX(nodeList->at(count)->lng*equator - lngmin*equator);
        front.setY(latmax*equator - nodeList->at(count)->lat*equator);

        back.setX(nodeList->at(count + 1)->lng*equator - lngmin*equator);
        back.setY(latmax*equator - nodeList->at(count + 1)->lat*equator);
        if(count%80==0)
        {
            painter->save();
            QFont font=painter->font();
            font.setPointSize(50);
            painter->setFont(font);
            painter->translate(front);//设置坐标原点
            painter->rotate(nodeList->at(count)->heading);//旋转的角度
            painter->drawText(QPointF(-25,-25),"▲");
            painter->restore();
        }
        if( nodeList->at(count + 1))
            painter->drawLine(front, back);

        if(nodeList->at(count)->lat > latmax)
        {
            latmax = nodeList->at(count)->lat;
        }

        if(nodeList->at(count)->lng > lngmax)
        {
            lngmax = nodeList->at(count)->lng;
        }

        if(nodeList->at(count)->lat < latmin)
        {
            latmin = nodeList->at(count)->lat;
        }

        if(nodeList->at(count)->lng < lngmin)
        {
            lngmin = nodeList->at(count)->lng;
        }
    }
}
