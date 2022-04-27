#ifndef BOUNDARYCATCH_H
#define BOUNDARYCATCH_H

#include <QObject>
#include <QTimer>

#include "posinfo.h"

class BoundaryCatch : public QObject
{
    Q_OBJECT
public:

    static BoundaryCatch* getHandle()
    {
        if(handle == NULL)
        {
            handleMutex->lock();
            if(handle == NULL)
            {
                handle = new BoundaryCatch;
            }
            handleMutex->unlock();
        }

        return handle;
    }
    QTimer *catchTimer;
    QTimer *realTimeDataTimer;

private:
    QList<latlngheading*> boundaryList;
    explicit BoundaryCatch(QObject *parent = 0);

    static BoundaryCatch* handle;
    static QMutex* handleMutex;

public slots:
    void startCatch();
    void reStartCatch();
    void stopCatch();
    void insertPos(struct latlngheading);
    QList<latlngheading*> getBoundaryList();

    void catchTimerSlot();
    void realTimeDataTimerSlot();
};

#endif // BOUNDARYCATCH_H
