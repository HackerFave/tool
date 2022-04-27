#ifndef STOPCALCLASS_H
#define STOPCALCLASS_H

#include <QObject>
#include "hmicmdst.h"

#define POS_AVER_TIMES  10

class stopCalClass : public QObject
{
    Q_OBJECT
public:
    explicit stopCalClass(QObject *parent = 0);

    static bool stopCalFun(struct latlngheading *ret, float heading);
    static bool stopCalAvOffsetFun(struct latlngheading *ret, float stopHeadingCompAngle);

    static void averPosFun(struct latlngheading *pos);

    static uint8_t calcLoadStopPointCnt;
    static struct latlngheading tmpAvPos;
    static struct latlngheading avPos;
    static bool avPosOk;
    static bool stopOk;
signals:

public slots:
};

#endif // STOPCALCLASS_H
