#include "posinfo.h"
#include "log.h"
#include "stopCalClass.h"

PosInfo* PosInfo::handle = NULL;
QMutex* PosInfo::handleMutex = new QMutex;
bool PosInfo::gpsUpdateFlag = false;

PosInfo::PosInfo()
{
    this->info.speed = 0;
    this->info.maxSpeed = 0;
    info.status &= ~(0x03 << 16);//初始化为P档
}

void PosInfo::setPosInfo(struct pubVehiclePosST info)
{
    memmove(&(this->info), &info, sizeof(struct pubVehiclePosST));
    struct latlngheading pos;
    getCurPos(&pos);
    stopCalClass::averPosFun(&pos);
}

struct pubVehiclePosST PosInfo::getPosInfo()
{
    return info;
}

void PosInfo::getCurPos(struct latlngheading* pos)
{
    pos->lat = info.gLat;
    pos->lng = info.gLng;
    pos->heading = info.gHead;
}
void PosInfo::getCurPos(struct collectlatlngheading* pos)
{
    pos->lat = info.gLat;
    pos->lng = info.gLng;
    pos->height = info.gHeight;
    pos->heading = info.gHead;

}

char* PosInfo::getWarnning()
{
    return info.warnning;
}

u32 PosInfo::getStatus()
{
    return info.status;
}

u8 PosInfo::getWorkStatu()
{
    return info.statu;
}

u16 PosInfo::getDelay()
{
    return info.delay;
}

qint64 PosInfo::getUtcTime()
{
    return info.utcTim;
}

char PosInfo::getRssi()
{
    return info.status>>24;
}

char PosInfo::getAiState()
{
    char result;
    result = info.status>>5;
    result &= 0x03;

    return result;
}

char PosInfo::getGearState()
{
    char result;
    result = info.status>>16;
    result &= 0x03;

    return result;
}

char PosInfo::getEngineState()
{
    char result;
    result = info.status>>10;
    result &= 0x03;

    return result;
}

char PosInfo::getGpsState()
{
    char result;
    result = info.status>>1;
    result &= 0x03;

    return result;
}

float PosInfo::getMaxSpeed()
{
    return info.maxSpeed;
}

float PosInfo::getSpeed()
{
    return info.speed;
}

bool PosInfo::getLoadState()
{
    if(info.mater == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool PosInfo::getWarnningBit(WarningBit _bit)
{
    uint arraryIndex = _bit / 8;
    uint bitIndex = _bit % 8;
    bool result = false;
    uint len = sizeof(info.warnning);
    if(arraryIndex < len)
    {
        result = info.warnning[arraryIndex] & (0x01 << bitIndex);
    }
    return result;
}

bool PosInfo::getCloudLinkStatus()
{
    return !getWarnningBit(v2n);
}

bool PosInfo::getV2xLinkStatus()
{
    return !getWarnningBit(v2x);
}

bool PosInfo::getMasterLinkStatus()
{
    return !getWarnningBit(master);
}

void PosInfo::setGpsUpdateFlag(bool flag)
{
    gpsUpdateFlag = flag;
}

bool PosInfo::getGpsUpdateFlag()
{
    return gpsUpdateFlag;
}

