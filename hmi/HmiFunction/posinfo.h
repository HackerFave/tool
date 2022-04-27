#ifndef POSINFO_H
#define POSINFO_H

#include <QObject>
#include <QMutex>

#include "hmicmdst.h"

class PosInfo : public QObject
{
    Q_OBJECT
public:
    static PosInfo* getHandle()
    {
        if(handle == NULL)
        {
            handleMutex->lock();
            if(handle == NULL)
            {
                handle = new PosInfo;
            }
            handleMutex->unlock();
        }

        return handle;
    }

    struct pubVehiclePosST getPosInfo();//获取心跳包
    void getCurPos(struct latlngheading *pos);//获取当前位置
    void getCurPos(struct collectlatlngheading *pos);//获取当前位置
    char* getWarnning();//获取告警信息
    u32 getStatus();//获取车辆状态
    u8  getWorkStatu();//获取运营状态
    u16 getDelay();//获取延时状态
    qint64 getUtcTime();//获取utc时间
    char getRssi();//获取信号质量
    char getAiState();//获取人工或者Ai驾驶状态
    char getGearState();//获取档位
    char getEngineState();//获取发动机状态
    char getGpsState();//获取定位状态
    float getMaxSpeed();//获取最大速度
    float getSpeed();//获取当前速度
    bool getLoadState();//获取当前装载状态

    bool getCloudLinkStatus();//获取云端连接状态,true-连接, false-未连接
    bool getV2xLinkStatus();//获取V2X连接状态
    bool getMasterLinkStatus();//获取Master连接状态

    bool getWarnningBit(enum WarningBit _bit); //获取报警标志位,返回true-该位异常。false-该位正常

    bool getGpsUpdateFlag();//get gps updata flag
    void setGpsUpdateFlag(bool flag);//set gps flag

private:
    PosInfo();

    static PosInfo* handle;
    static QMutex* handleMutex;

    struct pubVehiclePosST info;

    static bool gpsUpdateFlag;

signals:

public slots:
    void setPosInfo(struct pubVehiclePosST);
};

#endif // POSINFO_H
