#include "../include/trgearshift.h"
#include "msg.h"
#include "mydebug.h"
#include "zmq/zmqmsg.hpp"
#include <QDebug>
#include <QThread>
#include <cstring>
#include <hidapi/hidapi.h>

TrGearShift::TrGearShift(QObject *parent) : QObject(parent)
{
}

TrGearShift::~TrGearShift()
{
}

void TrGearShift::StartRecvRawData()
{
    hid_device *hidDevice = NULL;
    while (1)
    {
        hidDevice = hid_open(TR_GEARSHIFT_VID, TR_GEARSHIFT_PID, NULL);
        if (hidDevice == NULL)
        {
            MY_DEBUG() << "Open Tr GearShift Failed:" << hid_error(hidDevice) << " wait seconds";
            emit EmitGearShiftStatus(DEV_LINKSTATUS_DISCONNECTED);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }
        else
        {
            while (1)
            {
                unsigned char buf[128] = {0x00};
                int res = hid_read(hidDevice, buf, sizeof(buf));
                if (res > 0)
                {
                    HandleGearShiftRawData(buf, res);
                    emit EmitGearShiftStatus(DEV_LINKSTATUS_CONNECTED_DATA);
                }
                else
                {
                    qCritical() << "Read Tr GearShift error." << hid_error(hidDevice);
                    break;
                }
            }
        }
        hid_close(hidDevice);
        hidDevice = NULL;
    }
}

void TrGearShift::HandleGearShiftRawData(const uint8_t *data, int len)
{
    GearSt tmp;
    std::memset(&tmp, 0x00, sizeof(GearSt));
    std::memmove(&tmp, data, len);
    quint8 gearShift = GEARSHIFT_CTRL_N;
    switch (tmp.gear)
    {
    case 0x20:
        gearShift = GEARSHIFT_CTRL_D;
        break;
    case 0x10:
        gearShift = GEARSHIFT_CTRL_R;
        break;
    default:
        gearShift = GEARSHIFT_CTRL_N;
        break;
    }
    MY_DEBUG() << "Get Tr GearShift : " << static_cast<int>(gearShift);
    emit EmitGearShift(gearShift);
}
