#ifndef PATHTASK_H
#define PATHTASK_H

#include <QObject>
#include "pathframe.h"

class PathTask : public PathFrame
{
    Q_OBJECT
public:
    explicit PathTask(QList<latlngheading *>* datalist);
    void calcFinishedTask(struct latlngheading);

private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

};

#endif // PATHTASK_H
