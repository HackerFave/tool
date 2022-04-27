#ifndef PATHPERMISSION_H
#define PATHPERMISSION_H

#include <QObject>
#include "pathframe.h"

class PathPermission : public PathFrame
{
    Q_OBJECT
public:
    explicit PathPermission(QList<latlngheading *>* datalist);

private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // PATHPERMISSION_H
