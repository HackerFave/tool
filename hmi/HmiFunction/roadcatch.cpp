#include "roadcatch.h"

RoadCatch* RoadCatch::handle = NULL;
QMutex* RoadCatch::handleMutex = new QMutex;

RoadCatch::RoadCatch(QObject *parent) : QObject(parent)
{
    catchTimer = new QTimer;
    realTimeDataTimer = new QTimer;

    connect(catchTimer, SIGNAL(timeout()), this, SLOT(catchTimerSlot()));
    connect(realTimeDataTimer, SIGNAL(timeout()), this, SLOT(realTimeDataTimerSlot()));
}

void RoadCatch::startCatch()
{
    catchTimer->setInterval(500);
    catchTimer->start();

    roadList.clear();

    realTimeDataTimer->setInterval(1000);
    realTimeDataTimer->start();
}

void RoadCatch::reStartCatch()
{
    catchTimer->start(500);
}

void RoadCatch::stopCatch()
{
    catchTimer->stop();
    realTimeDataTimer->stop();
}

void RoadCatch::insertPos(latlngheading pos)
{
    struct latlngheading* node = new struct latlngheading;
    memmove((char*)(node), (char*)(&pos), sizeof(struct latlngheading));
    roadList.append(node);
}

QList<latlngheading*> RoadCatch::getRoadList()
{
    return roadList;
}

void RoadCatch::catchTimerSlot()
{
    PosInfo *curhandle = PosInfo::getHandle();
    struct latlngheading* pos= new struct latlngheading;
    curhandle->getCurPos(pos);

    roadList.append(pos);
}

void RoadCatch::realTimeDataTimerSlot()
{
    //更新地图显示
}
