#ifndef WORKLISTDEAL_H
#define WORKLISTDEAL_H

#include <QObject>
#include <QList>
#include <QMutex>
#include <QDebug>

#include "hmicmdst.h"

class WorkListDeal : public QObject
{
    Q_OBJECT
public:
    static WorkListDeal* getHandle()
    {
        if(handle == NULL)
        {
            handleMutex->lock();
            if(handle != NULL)
            {
                handle = new WorkListDeal;
            }
            handleMutex->unlock();
        }

        return handle;
    }

    void insertInList(struct pubVehicleDevNameStateST);
    void insertRunList(struct pubVehicleDevNameStateST);
    void insertOutList(struct pubVehicleDevNameStateST);
    void setFirstInList(struct pubVehicleDevNameStateST);

    void removeInList(struct pubVehicleDevNameStateST);
    void removeRunList(struct pubVehicleDevNameStateST);
    void removeOutList(struct pubVehicleDevNameStateST);

    void removeFirstInList();
    void removeFirstRunList();
    void removeFirstOutList();

    bool getFirstInList(struct pubVehicleDevNameStateST* );
    bool getFirstRunList(struct pubVehicleDevNameStateST* );
    bool getFirstOutList(struct pubVehicleDevNameStateST* );

    bool getImeiInListByName(struct pubVehicleDevNameStateST* );
    bool getImeiRunListByName(struct pubVehicleDevNameStateST* );
    bool getImeiOutListByName(struct pubVehicleDevNameStateST* );

    QList<struct pubVehicleDevNameStateST>* getInList();
    QList<struct pubVehicleDevNameStateST>* getRunList();
    QList<struct pubVehicleDevNameStateST>* getOutList();

    static QList<struct pubVehicleDevNameStateST>* inList;
    static QList<struct pubVehicleDevNameStateST>* runList;
    static QList<struct pubVehicleDevNameStateST>* outList;

    static QMutex* listMutex;

private:
    static WorkListDeal *handle;
    static QMutex* handleMutex;

    WorkListDeal();


};

#endif // WORKLISTDEAL_H
