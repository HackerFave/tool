#include "boundarycatch.h"

BoundaryCatch* BoundaryCatch::handle = NULL;
QMutex* BoundaryCatch::handleMutex = new QMutex;

BoundaryCatch::BoundaryCatch(QObject *parent) : QObject(parent)
{
    catchTimer = new QTimer;
    realTimeDataTimer = new QTimer;

    connect(catchTimer, SIGNAL(timeout()), this, SLOT(catchTimerSlot()));
    connect(realTimeDataTimer, SIGNAL(timeout()), this, SLOT(realTimeDataTimerSlot()));
}

void BoundaryCatch::startCatch()
{
    catchTimer->setInterval(500);
    catchTimer->start();

    boundaryList.clear();

    realTimeDataTimer->setInterval(1000);
    realTimeDataTimer->start();
}

void BoundaryCatch::reStartCatch()
{
    catchTimer->start(500);
}

void BoundaryCatch::stopCatch()
{
    catchTimer->stop();
    realTimeDataTimer->stop();
}

void BoundaryCatch::insertPos(struct latlngheading pos)
{
    struct latlngheading* node = new struct latlngheading;
    memmove((char*)(node), (char*)(&pos), sizeof(struct latlngheading));
    boundaryList.append(node);
}

QList<latlngheading*> BoundaryCatch::getBoundaryList()
{
    return boundaryList;
}

void BoundaryCatch::catchTimerSlot()
{
    PosInfo *curhandle = PosInfo::getHandle();
    struct latlngheading* pos= new struct latlngheading;
    curhandle->getCurPos(pos);

    boundaryList.append(pos);
}

void BoundaryCatch::realTimeDataTimerSlot()
{
    //更新地图显示
}
