#ifndef DEVMANAGE_H
#define DEVMANAGE_H

#include "devhandle.h"
#include <list>
#include <memory>
#include <mutex>
#include <string>

typedef struct
{
    qintptr socketDescriptor;
    std::string imei;
    int type;
    std::shared_ptr<DevHandle> dev;
} DevInfo;

class DevManage
{
public:
    DevManage();
    virtual ~DevManage();
    void AddDev(qintptr socketDesc, std::shared_ptr<DevHandle> devHandle);

    void UpdateDev(qintptr socketDesc, const std::string &imei, int type);
    std::shared_ptr<DevHandle> GetDevHandle(const std::string &imei);
    std::shared_ptr<DevHandle> GetDevHandle(qintptr socketDesc);

    void DelDev(const std::string &imei);
    void DelDev(const qintptr socketDesc);

private:
    std::mutex listMutex;
    std::list<DevInfo> devList;
};

#endif // DEVMANAGE_H
