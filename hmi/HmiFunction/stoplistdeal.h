#ifndef STOPLISTDEAL_H
#define STOPLISTDEAL_H

#include <QObject>
#include <QMutex>
#include <QStringList>

#include "pub.h"
#include "paraparsing.h"

class StopListDeal : public QObject
{
    Q_OBJECT
public:
    static StopListDeal* getHandle()
    {
        if(handle == NULL)
        {
            handleMutex->lock();
            if(handle == NULL)
            {
                handle = new StopListDeal;
            }
            handleMutex->unlock();
        }

        return handle;
    }

    void clearStopList();
    void generateStopList(QString lpxpath, double sidefixvalue, double vehiclewidth, double vehiclelength);

    QList<latlngheading* > stopList;
private:
    static StopListDeal* handle;
    static QMutex* handleMutex;
    StopListDeal();

signals:
    void generateComplete();
public slots:
};

#endif // STOPLISTDEAL_H
