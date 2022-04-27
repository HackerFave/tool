#include "devmanage.h"

DevManage::DevManage() {}
DevManage::~DevManage() {}

void DevManage::AddDev(qintptr socketDesc, std::shared_ptr<DevHandle> devHandle)
{
    DevInfo tmp ;
    tmp.imei.clear();
    tmp.type = 0;
    tmp.socketDescriptor = socketDesc;
    tmp.dev = devHandle;
    std::lock_guard<std::mutex> lk(listMutex);
    devList.push_back(tmp);
    qDebug() << "Add New Dev :" << socketDesc;
}

void DevManage::UpdateDev(qintptr socketDesc, const std::string &imei, int type)
{
    std::lock_guard<std::mutex> lk(listMutex);
    for (auto &it : devList)
    {
        if (it.socketDescriptor == socketDesc)
        {
            it.imei = imei;
            it.type = type;
            qDebug() << " Update Dev," << socketDesc << " Imei:" << imei.c_str()
                     << " Type:" << type;
            break;
        }
    }
}

std::shared_ptr<DevHandle> DevManage::GetDevHandle(const std::string &imei)
{
    std::lock_guard<std::mutex> lk(listMutex);
    for (auto &it : devList)
    {
        if (it.imei == imei)
        {
            return it.dev;
        }
    }
    return nullptr;
}

std::shared_ptr<DevHandle> DevManage::GetDevHandle(qintptr socketDesc)
{
    std::lock_guard<std::mutex> lk(listMutex);
    for (auto &it : devList)
    {
        if (it.socketDescriptor == socketDesc)
        {
            return it.dev;
        }
    }
    return nullptr;
}

void DevManage::DelDev(const std::string &imei)
{
    qDebug() << " Start Del Dev:" << imei.c_str();
    std::lock_guard<std::mutex> lk(listMutex);
    for (auto it = devList.begin(); it != devList.end(); it++)
    {
        if (it->imei == imei)
        {
            devList.erase(it);
            break;
        }
    }
}

void DevManage::DelDev(const qintptr socketDesc)
{
    qDebug() << " Start Del Dev:" << socketDesc;

    std::lock_guard<std::mutex> lk(listMutex);
    for (auto it = devList.begin(); it != devList.end(); it++)
    {
        if (it->socketDescriptor == socketDesc)
        {
            devList.erase(it);
            break;
        }
    }
}
