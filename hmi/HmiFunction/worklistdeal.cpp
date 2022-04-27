#include "worklistdeal.h"
#include <cstring>
#include "log.h"

WorkListDeal* WorkListDeal::handle = NULL;
QMutex* WorkListDeal::handleMutex = new QMutex;
QMutex* WorkListDeal::listMutex = new QMutex;

QList<struct pubVehicleDevNameStateST>* WorkListDeal::inList = new QList<struct pubVehicleDevNameStateST>;
QList<struct pubVehicleDevNameStateST>* WorkListDeal::runList = new QList<struct pubVehicleDevNameStateST>;
QList<struct pubVehicleDevNameStateST>* WorkListDeal::outList = new QList<struct pubVehicleDevNameStateST>;

WorkListDeal::WorkListDeal()
{
    inList->clear();
    runList->clear();
    outList->clear();
}

void WorkListDeal::insertInList(struct pubVehicleDevNameStateST element)
{
    listMutex->lock();
    if(!inList->contains(element))
    {
        LOG_DEBUG("will append node:%s ",element.localname);
        inList->append(element);
    }
    else
    {
        for(int count = 0; count < inList->size(); count++)
        {
            if((strncmp(inList->at(count).localimei, element.localimei, 15) == 0) && (strncmp(inList->at(count).localname, element.localname, 20) == 0))
            {
                LOG_DEBUG("will replace ndoe :%s to %s",inList->at(count).localname,element.localname);
                inList->replace(count, element);
            }
        }
    }
    listMutex->unlock();
}

void WorkListDeal::insertRunList(struct pubVehicleDevNameStateST element)
{
    listMutex->lock();
    if(!runList->contains(element))
    {
        runList->append(element);
    }
    else
    {
        for(int count = 0; count < runList->size(); count++)
        {
            if(std::strncmp(runList->at(count).localimei, element.localimei, 15) == 0)
            {
                runList->replace(count, element);
            }
        }
    }
    listMutex->unlock();
}

void WorkListDeal::insertOutList(struct pubVehicleDevNameStateST element)
{
    listMutex->lock();
    if(!outList->contains(element))
    {
        outList->append(element);
    }
    else
    {
        for(int count = 0; count < outList->size(); count++)
        {
            if(std::strncmp(outList->at(count).localimei, element.localimei, 15) == 0)
            {
                outList->replace(count, element);
            }
        }
    }
    listMutex->unlock();
}

void WorkListDeal::setFirstInList(struct pubVehicleDevNameStateST element)
{
    listMutex->lock();
    int index = inList->indexOf(element);
    if(index > 0)
    {
        inList->removeAt(index);
        inList->insert(0,element);
    }
    else if(index < 0)
    {
        LOG_DEBUG("Not Found element in InList,IMEI:%s",element.localimei);
    }
    listMutex->unlock();
}

void WorkListDeal::removeInList(struct pubVehicleDevNameStateST element)
{
    listMutex->lock();
    inList->removeOne(element);
    listMutex->unlock();
}

void WorkListDeal::removeRunList(struct pubVehicleDevNameStateST element)
{
    listMutex->lock();
    runList->removeOne(element);
    listMutex->unlock();
}

void WorkListDeal::removeOutList(struct pubVehicleDevNameStateST element)
{
    listMutex->lock();
    outList->removeOne(element);
    listMutex->unlock();
}

void WorkListDeal::removeFirstInList()
{
    listMutex->lock();
    if(!inList->isEmpty())
    {
        inList->removeFirst();
    }
    listMutex->unlock();
}

void WorkListDeal::removeFirstRunList()
{
    listMutex->lock();
    if(!runList->isEmpty())
    {
        runList->removeFirst();
    }
    listMutex->unlock();
}

void WorkListDeal::removeFirstOutList()
{
    listMutex->lock();
    if(!outList->isEmpty())
    {
        outList->removeFirst();
    }
    listMutex->unlock();
}

bool WorkListDeal::getFirstInList(struct pubVehicleDevNameStateST* node)
{
    if(node == NULL)
    {
        return false;
    }

    if(inList->size() != 0)
    {
        listMutex->lock();
        memmove((char*)(node), (char*)(&inList->first()), sizeof(struct pubVehicleDevNameStateST));
        listMutex->unlock();

        return true;
    }

    std::memset(node,0x00,sizeof(struct pubVehicleDevNameStateST));
    return false;
}

bool WorkListDeal::getFirstRunList(struct pubVehicleDevNameStateST* node)
{
    if(node == NULL)
    {
        return false;
    }
    if(inList->size() != 0)
    {
        listMutex->lock();
        memmove((char*)(node), (char*)(&runList->first()), sizeof(struct pubVehicleDevNameStateST));
        listMutex->unlock();

        return true;
    }
    else
    {
        std::memset(node,0x00,sizeof(struct pubVehicleDevNameStateST));
        LOG_DEBUG("RunList is Empty!!");
        return false;
    }
}

bool WorkListDeal::getFirstOutList(struct pubVehicleDevNameStateST* node)
{
    if(node == NULL)
    {
        return false;
    }
    if(inList->size() != 0)
    {
        listMutex->lock();
        memmove((char*)(node), (char*)(&outList->first()), sizeof(struct pubVehicleDevNameStateST));
        listMutex->unlock();

        return true;
    }
    else
    {
        std::memset(node,0x00,sizeof(struct pubVehicleDevNameStateST));
        LOG_DEBUG("OutList is Empty!!");
        return false;
    }
}

bool WorkListDeal::getImeiInListByName(struct pubVehicleDevNameStateST* node)
{
    for(int count = 0; count < inList->size(); count++)
    {
        if(strncmp(inList->at(count).localname, node->localname, 20) == 0)
        {
            memmove((char*)(node), (char*)(&(inList->at(count))), sizeof(struct pubVehicleDevNameStateST));
            return true;
        }
    }

    return false;
}

bool WorkListDeal::getImeiRunListByName(struct pubVehicleDevNameStateST* node)
{
    for(int count = 0; count < runList->size(); count++)
    {
        if(strncmp(runList->at(count).localname, node->localname, 20) == 0)
        {
            memmove((char*)(node), (char*)(&(inList->at(count))), sizeof(struct pubVehicleDevNameStateST));
            return true;
        }
    }

    return false;
}

bool WorkListDeal::getImeiOutListByName(struct pubVehicleDevNameStateST* node)
{
    for(int count = 0; count < outList->size(); count++)
    {
        if(strncmp(outList->at(count).localname, node->localname, 20) == 0)
        {
            memmove((char*)(node), (char*)(&(inList->at(count))), sizeof(struct pubVehicleDevNameStateST));
            return true;
        }
    }

    return false;
}

QList<struct pubVehicleDevNameStateST>* WorkListDeal::getInList()
{
    return inList;
}

QList<struct pubVehicleDevNameStateST>* WorkListDeal::getRunList()
{
    return runList;
}

QList<struct pubVehicleDevNameStateST>* WorkListDeal::getOutList()
{
    return outList;
}
