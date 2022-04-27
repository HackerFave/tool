#ifndef STOPPOINTS_H
#define STOPPOINTS_H
#include <QObject>
#include "pathframe.h"
#include "HmiFunction/stoplistdeal.h"
class StopPoints : public PathFrame
{
    Q_OBJECT
public:
    explicit StopPoints(QList<latlngheading *>* datalist);
    void setPointOffset(QPointF );
private:
    QPointF off;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

signals:

public slots:
};

#endif // STOPPOINTS_H
