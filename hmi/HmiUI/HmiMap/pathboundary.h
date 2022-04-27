#ifndef PATHBOUNDARY_H
#define PATHBOUNDARY_H

#include <QObject>

#include "pathframe.h"

class PathBoundary : public PathFrame
{
    Q_OBJECT
public:
    explicit PathBoundary(QList<latlngheading *>* datalist);

private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // PATHBOUNDARY_H
