#include "stoppoints.h"

StopPoints::StopPoints(QList<latlngheading* >* datalist) : PathFrame(datalist)
{
    color = QColor(207, 207, 207);
    off=QPoint(0,0);
}

void StopPoints::setPointOffset(QPointF offset)
{
    off=offset;
}



QRectF StopPoints::boundingRect() const
{
    QRectF rect(0, 0, (lngmax*equator - lngmin*equator), (latmax*equator - latmin*equator));
    return rect;
}

void StopPoints::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)


    painter->setBackground(Qt::NoBrush);

    painter->setRenderHint(QPainter::Antialiasing, true);

    painter->setPen(QPen(color, 1));
    QFont font=painter->font();
    font.setPointSize(40);
    painter->setFont(font);
    QImage image;
    image.load(":/image/resource/stopPos1.png");
    //image=image.scaled(70,110); xuzk2

    for(int count = 0; count < nodeList->size(); count++)
    {

        QPointF front;
        QPointF back;

        front.setX(nodeList->at(count)->lng*equator - lngmin*equator);
        front.setY(latmax*equator - nodeList->at(count)->lat*equator);

        back.setX(nodeList->at(count + 1)->lng*equator - lngmin*equator);
        back.setY(latmax*equator - nodeList->at(count + 1)->lat*equator);


        {
            painter->save();
            painter->translate(front);//设置坐标原点
            painter->rotate(nodeList->at(count)->heading);//旋转的角度
//            qDebug()<<count<<":"<<nodeList->at(count)->heading;
            painter->drawImage(off/*QPointF(-70,-70)*/,image);//xuzk2
            painter->drawText(off,QString("%1").arg(count+1));
            painter->restore();
        }
    }
}
