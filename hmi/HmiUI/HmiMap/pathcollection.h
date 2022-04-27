#ifndef PATHCOLLECTION_H
#define PATHCOLLECTION_H

#include <QObject>

#include "pathframe.h"

class PathCollection : public PathFrame
{
    Q_OBJECT
public:
    explicit PathCollection(QList<latlngheading *>* datalist);

private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
signals:

public slots:
};

#endif // PATHCOLLECTION_H
