#include "pathpermission.h"
#define LEN_SHOW 60
PathPermission::PathPermission(QList<latlngheading* >* datalist) : PathFrame(datalist)
{
    color = QColor(255, 0, 0);
}


QRectF PathPermission::boundingRect() const
{
    QRectF rect(0, 0, (LEN_SHOW), (LEN_SHOW));
    return rect;
}

void PathPermission::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)


    painter->setBackground(Qt::NoBrush);

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(QPen(color, 8));
    for(int count = 0; count < nodeList->size(); count++)
    {
//        double x1,y1,x2,y2;
//        float h;
//        h=nodeList->at(count)->heading;//3.1415926;
//        h-=90;
//        x1=-LEN_SHOW*cos(h);
//        y1=+LEN_SHOW*sin(h);
//        x2=LEN_SHOW*cos(h);
//        y2=LEN_SHOW*sin(h);
//        QPoint front,back;
//        front.setX(x1);front.setY(y1);
//        back.setX(x2);back.setY(y2);
//        painter->drawLine(front,back);
        QPointF front;

        front.setX(nodeList->at(count)->lng*equator - lngmin*equator);//0,0
        front.setY(latmax*equator - nodeList->at(count)->lat*equator);//0,0

        painter->save();
        QFont font=painter->font();
        font.setPointSize(LEN_SHOW);
        painter->setFont(font);
        painter->translate(front);//设置坐标原点
        painter->rotate(nodeList->at(count)->heading+90);//旋转的角度
        painter->drawText(QPointF(LEN_SHOW/2,LEN_SHOW/2),"|");
        painter->restore();
    }
}
