#include "vehicleitem.h"

vehicleItem::vehicleItem(QObject *parent) : QObject(parent)
{
    vehiclePixmap = new QPixmap;
    setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable);
}

vehicleItem::vehicleItem(QSize size, QString pixmap, qreal angle, bool flag, QObject *parent) : QObject(parent)
{
    this->angle = angle;
    if(flag == false)
        this->setEnabled(false);

    vehiclePixmap = new QPixmap;
    setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable);
    this->setCursor(QCursor(Qt::OpenHandCursor));

    this->setVehicleState(size, pixmap);
    this->setTransformOriginPoint(this->boundingRect().center());
    this->setRotation(angle);
    this->setFlag(QGraphicsItem::ItemIsSelectable);
}

void vehicleItem::setVehicleState(QSize size, QString pixmap)
{
    QPixmap pixmapTmp;
    pixmapTmp.load(pixmap);
    *vehiclePixmap = pixmapTmp.scaled(size.width(),size.height(),Qt::KeepAspectRatio);
    this->setPixmap(*vehiclePixmap);
}

bool vehicleItem::sceneEvent(QEvent *event)
{
    switch(event->type())
    {
    case QEvent::GrabMouse:
        focusInEvent(dynamic_cast<QFocusEvent*>(event));
        break;
    case QEvent::UngrabMouse:
        focusOutEvent(dynamic_cast<QFocusEvent*>(event));
        break;
    default:
        break;
    }
    return false;
}

void vehicleItem::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event);

    QPixmap pixmapTmp;
    pixmapTmp.load(":/image/resource/mineChecked.png");
    *vehiclePixmap = pixmapTmp.scaled(50,70,Qt::KeepAspectRatio);
    this->setPixmap(*vehiclePixmap);
    update();
    emit itemFocusedSignal(angle);
}

void vehicleItem::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event);

    QPixmap pixmapTmp;
    pixmapTmp.load(":/image/resource/mineUnChecked.png");
    *vehiclePixmap = pixmapTmp.scaled(50,70,Qt::KeepAspectRatio);
    this->setPixmap(*vehiclePixmap);
    update();
}
