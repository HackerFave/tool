#ifndef MainCameraModel_H
#define MainCameraModel_H

#include "Interface/IModel.h"

class MainCameraModel : public IModel
{
    Q_OBJECT
public:
    explicit MainCameraModel(QObject *parent = nullptr);

    //void setVehicleRawData(VehicleRawData data);
    void setCameraUrl(const QString &front,const QString &back);
    void setThrottleBrakeWheel(float throttle,float brake,float wheel);
    void setThrottle(float throttle);//加速踏板百分比
    void setBrake(float brake);//刹车
    void setWheel(float wheel);//方向盘转角
    void setSpeedRaw(float wheel);//发动机转速
    void setSpeed(float speed);
    void setGear(float gear);
    void setVehicleDelay(uint);
    void setRemoteStatus(uint);

    enum ModelCmdType
    {
        MC_UpdateAll=0,
        MC_ThrottleBrakeWheel,
        MC_Speed,
        MC_Gear,
        MC_Delay,
        MC_RemoteStatus,
        MC_Max,
        MC_Throttle,
        MC_Brake,
        MC_Wheel,
        MC_Throttle_RingDila
    };

    enum RemoteStatus{
        RS_Monitoring = 0,
        RS_RemoteControl,
        RS_RemoteControlFail,
        RS_RemoteControlDisconnect
    };

    typedef struct
    {
        float   current;
        float   max;
        float   min;
    } CtrlType;

    CtrlType    m_throttle{0,0,0};
    CtrlType    m_brake{0,0,0};
    CtrlType    m_wheel{0,0,0};

    CtrlType    m_speed{0,0,0};

    int         m_gear{0};

    CtrlType    m_tachometer{0,0,0};
    CtrlType    m_speed_raw{0,0,0};//发动机转速
    QString     m_frontCameraUrl;
    QString     m_backCameraUrl;

    uint        m_vehicleDelay{0};
    RemoteStatus        m_remoteStatus{RS_Monitoring};


};

#endif // MainCameraModel_H
