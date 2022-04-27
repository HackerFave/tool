#ifndef VEHICLEITEM_H
#define VEHICLEITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QEvent>
#include <QCursor>
#include <QSize>
#include <QPixmap>
#include <QMouseEvent>

class vehicleItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit vehicleItem(QObject *parent = NULL);
    explicit vehicleItem(QSize size, QString pixmap, qreal angle, bool flag=true, QObject *parent=NULL);
    void setVehicleState(QSize size, QString pixmap);
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    bool sceneEvent(QEvent *event);

private:
    QPixmap *vehiclePixmap;
    qreal angle;
signals:
    void itemFocusedSignal(qreal);
};

#endif // VEHICLEITEM_H
