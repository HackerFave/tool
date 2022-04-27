#ifndef ROADCATCH_H
#define ROADCATCH_H

#include <QObject>
#include <QTimer>

#include "posinfo.h"

class RoadCatch : public QObject
{
    Q_OBJECT
public:

    static RoadCatch* getHandle()
    {
        if(handle == NULL)
        {
            handleMutex->lock();
            if(handle == NULL)
            {
                handle = new RoadCatch;
            }
            handleMutex->unlock();
        }

        return handle;
    }

    QTimer *catchTimer;
    QTimer *realTimeDataTimer;

private:
    QList<latlngheading*> roadList;
    explicit RoadCatch(QObject *parent = 0);

    static RoadCatch* handle;
    static QMutex* handleMutex;


public slots:
    void startCatch();
    void reStartCatch();
    void stopCatch();
    void insertPos(struct latlngheading);
    QList<latlngheading*> getRoadList();

    void catchTimerSlot();
    void realTimeDataTimerSlot();
};

#endif // ROADCATCH_H
