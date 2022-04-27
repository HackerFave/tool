#ifndef HMICMDST_H
#define HMICMDST_H

#include <QObject>
#include <QMetaType>
#include <QString>
#include <QMap>
#include <QStringList>
#include "warnning.h"

typedef unsigned char		u8;
typedef char				s8;
typedef unsigned short		u16;
typedef short				s16;
typedef unsigned int		u32;
typedef int					s32;

#pragma pack(1)
/**
 * 定位数据结构体
**/
struct latlngheading
{
    double lat;
    double lng;
    float height;
    float heading;
    uint8_t flag;//bit0 任务路径，走过的点0未走过 1走过;bit1-2 0:空位 1:有车 2.即将停车的点
    uint8_t flag2;
};
struct DiggerBoundaryPathST
{
    float heading;
    double latitude;
    double longitude;
    float height;
    char direction;//0x01: 左侧区域有效  0x02: 右侧区域有效
    float offset;
    char located;//0:未定位;1:GPS定位;2:差分定位
    char flag1;
    char flag2;
};
/**
 * 定位数据采集结构体
**/
struct collectlatlngheading
{
    double lat;
    double lng;
    float height;
    float heading;
    bool flag;//判定是否空缺
    uint8_t flag2;//手动指点判断 0自动生成停靠点  1手动指点
};

/**
* 电铲配置文件结构图
**/
struct paraExcavatorST
{
    double vehicleLength;
    double vehicleWidth;
    double vehicleHight;
    bool fixedPoint;
    double stopLat;
    double stopLng;
    float stopHeight;
    float stopHeading;
    double stopLength;
    double stopWidth;
    double stopCompAngle;
    float shovelArmLen;
    int selfCheckStopPointSwitch;
};

/**
* 矿卡配置文件结构图
**/
struct paraMinetrunckST
{
    double vehicleLength;
    double vehicleWidth;
};

/**
 * 破碎站配置文件结构体
**/
struct paraCrusherST
{
    double vehicleLength;
    double vehicleWidth;
    double vehicleHight;
    double stopLat;
    double stopLng;
    float stopHeading;
};

/**
 * 停车场配置文件结构体
**/
struct paraParkingST
{
    double vehicleLength;
    double vehicleWidth;
    double vehicleHight;
    double Lat;
    double Lng;
    float Heading;
    double stop1Lat;
    double stop1Lng;
    float stop1Heading;
    double stop2Lat;
    double stop2Lng;
    float stop2Heading;
    double stop3Lat;
    double stop3Lng;
    float stop3Heading;
    double stop4Lat;
    double stop4Lng;
    float stop4Heading;
};
struct paraDiggerST
{
    double vehicleLength;
    double vehicleWidth;
    double boundaryFixValue;
    float   gpsHeightofVehicle;
    int sidefixvalue;
    int manualStopNum;
};


struct paraCollectorST
{
    double boundaryFixValue;
    float   gpsHeightofVehicle;
};

/**
 * 共有配置文件结构体
**/
struct paraPubST
{
    int vehicleType;//1:电铲  2:破碎站   3:推土机
    double vehicleLength;
    double vehicleWidth;
    double vehicleHight;
    QString localIp;
    int localPort;
    QString remoteIp;
    int remotePort;
    int compatibleVer;
    int mainVer;
    int subVer;
    QString mapPath;
    QString taskPath;
    QString boundaryPath;
    QString roadPath;
    QString permissionPath;
    QString boundaryDBPath;
};
/**
 * 配置文件汇总
**/
struct paraST
{
    struct paraPubST pub;
    struct paraExcavatorST excavator;
    struct paraMinetrunckST minetrunck;
    struct paraCrusherST crusher;
    struct paraDiggerST digger;
    struct paraCollectorST collector;
    struct paraParkingST parking;
};


/**
 * 矿卡辅助车辆通用数据结构
 */
//车辆实时位置信息结构体
#if(PROTOCAL_TYPE == 1)
struct pubVehiclePosST
{
    char    warnning[16];   //报警
    u32		status;         //状态
    double	gLat;           //维度
    double	gLng;           //经度
    float	gHeight;		//海拔高度
    float	speed;          //目前车速
    float   maxSpeed;       //限速
    float	gHead;          //偏航角
    float   wheelAngle;     //前轮转角
    float	accx;           //前向加速度
    float	accy;           //侧向加速度
    float	accz;           //横摆角速度
    float   lateralError;   //横向误差米
    float   gHeadError;//航向角误差度
    float   stopLongitudinalError;//停靠点纵向误差
    float   stopLateralError;//停靠点横向误差
    float   wheelAngleError;
    u8      statusMachine[32];//子状态机
    u8      statu;          //运营状态
    u16     delay;          //延迟原因
    u16     loadNum;        //道路编号
    float   remainDis;      //剩余距离
    u8      runStatu;       //运行状态
    u16     stopReason;     //停车原因
    u16     taskNum;        //任务编号
    u8      taskWorkStatu;  //任务执行状态
    u16     mater;         //物料编码
    qint64  utcTim;         //时间
};
#elif(PROTOCAL_TYPE == 2)
struct pubVehiclePosST
{
    char    warnning[16];   //报警
    u32		status;         //状态
    double	gLat;           //维度
    double	gLng;           //经度
    float	gHeight;		//海拔高度
    float	speed;          //目前车速
    float   maxSpeed;       //限速
    float	gHead;          //偏航角
    float   wheelAngle;     //前轮转角
    float	accx;           //前向加速度
    float	accy;           //侧向加速度
    float	accz;           //横摆角速度
    float   lateralError;   //横向误差米
    float   gHeadError;//航向角误差度
    float   stopLongitudinalError;//停靠点纵向误差
    float   stopLateralError;//停靠点横向误差
    float   wheelAngleError;
    u8      statusMachine[32];//子状态机
    u8      statu;         //运营状态
    u16     delay;          //延迟原因
    u16     loadNum;        //道路编号
    float   remainDis;      //剩余距离
    u8      runStatu;       //运行状态
    u16     stopReason;     //停车原因
    u16     taskNum;        //任务编号
    u8      taskWorkStatu;  //任务执行状态
    u16     mater;         //物料编码
    qint64  utcTim;         //时间

    char    curPathName[39];//当前路径文件名称
    char    curBoundaryName[39];//当前作业区区域边界文件名称
    quint32 curPosNum;      //当前位置点序号
};
#endif
Q_DECLARE_METATYPE(struct pubVehiclePosST)
//车辆运营状态变更
struct pubVehicleWorkStatuST
{
    u8      statuType;                  //运营状态
    u16     fault;                      //故障原因
    u16     delayTim;                   //延时时长
};
Q_DECLARE_METATYPE(struct pubVehicleWorkStatuST)
//hmi发送平台通用应答
struct pubVehicleCloudComAckST
{
    u16 serialNum;  //序列号
    u16 id; //对应id
    u8  result; //结果，0成功，其他失败
};
Q_DECLARE_METATYPE(struct pubVehicleCloudComAckST)
//作业现场调度请求命令G→H(0305)
struct pubVehicleReqWorkOptST
{
    char localimei[15];
    char remoteimei[15];
    char    taskType;                   //作业类型
    char   cmdType;                    //命令类型
    qint64  utcTime;
    char localname[20];
    char remotename[20];
};
Q_DECLARE_METATYPE(struct pubVehicleReqWorkOptST)
//矿卡HMI发主控调度请求命令G→H(0F21)
struct pubVehicleReqWorkOpt0F21ST
{
    char    taskType;                   //作业类型
    char   cmdType;                    //命令类型
    qint64  utcTime;
};
Q_DECLARE_METATYPE(struct pubVehicleReqWorkOpt0F21ST)
//作业现场调度响应命令G→H(8306)
struct pubVehicleResWorkOptST
{
    char localimei[15];
    char remoteimei[15];
    char taskType; //作业类型
    char cmdType;  //命令类型
    char loadStatu; //满载、空载、半载
    uint16_t   materType;
    double  lat;
    double  lng;
    float   heading;
    qint64  utcTime;
    char localname[20];
    char remotename[20];
};
Q_DECLARE_METATYPE(struct pubVehicleResWorkOptST)
//作业现场调度响应命令G→H(8307)
struct pubVehicleResWorkOpt8307ST
{
    char remoteimei[15];
    char remotename[20];
    char taskType; //作业类型
    char cmdType;  //命令类型
    char loadStatu; //满载、空载、半载
    uint16_t   materType;
    uint16_t stopNum;//停靠位序号
    qint64  utcTime;
};
Q_DECLARE_METATYPE(struct pubVehicleResWorkOpt8307ST)
//作业现场调度响应命令G→H(8308)
struct pubVehicleResWorkOpt8308ST
{
    char remoteimei[15];
    char remotename[20];
    char taskType; //作业类型
    char cmdType;  //命令类型
    qint64  utcTime;
};
Q_DECLARE_METATYPE(struct pubVehicleResWorkOpt8308ST)
//DIGGER就绪  H->G(8B02)
struct pubDiggerReadyST
{
    double latitude;
    double longitude;
};
Q_DECLARE_METATYPE(struct pubDiggerReadyST)
//DIGGER更新分组信息8b01
struct pubGroupStatusST
{
    uint8_t type;
    uint16_t groupNum;
    uint16_t stopPointNum;
    uint8_t status;
};
Q_DECLARE_METATYPE(struct pubGroupStatusST)
struct pubBoundaryUpdateST
{
    char boundaryFile[39];
    uint8_t md5[32];
    uint16_t groupLen;
};

//8401预警数据格式
struct pubVehicleCollWarnningST
{
    char dimei[15];         //目标id
	char dName[20];         //目标名称
    char collType;          //预警类型
    float dVehicleDis;      //目标距离
    float collTim;          //预警时间
    char vehicleType;       //车辆类型
    float dVehicleLength;   //车辆长
    float dVehicleWidth;    //车辆宽
    float dVehicleHeight;   //车辆高
    double dVehicleLat;     //车辆纬度
    double dVehicleLng;     //车辆经度
    float dVehicleHeading;  //车辆航向
    float dVehicleSpeed;    //车辆速度
    float dVehicleSpeedX;   //车辆东向速度
    float dVehicleSpeedY;   //车辆北向速度
    float wheelCorner;      //前轮转角
    short roadNum;          //道路编号
    char roadDir;           //道路方向
    qint64 utcTim;          //utc时间

    bool operator ==(const struct pubVehicleCollWarnningST &other)
    {
        if(strncmp(this->dimei, other.dimei, 15) == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool operator <(const struct pubVehicleCollWarnningST &other) const
    {
        return strncmp(this->dimei, other.dimei, 15);
    }
};
Q_DECLARE_METATYPE(struct pubVehicleCollWarnningST)
//8402周边车辆
struct pubVehicleSurroundingST
{
    char dimei[15];         //车辆id
    char vehicleType;             //车辆类型
    char dName[20];         //车辆名称
    float dVehicleLength;   //车辆长
    float dVehicleWidth;    //车辆宽
    float dVehicleHeight;   //车辆高
    double dVehicleLat;     //车辆纬度
    double dVehicleLng;     //车辆经度
    float dVehicleElevation;//车辆高程
    float dVehicleSpeed;    //车辆速度
    float dVehicleHeading;  //车辆航向

    bool operator ==(const struct pubVehicleSurroundingST &other)
    {
        if(strncmp(this->dimei, other.dimei, 15) == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool operator <(const struct pubVehicleSurroundingST &other) const
    {
        return strncmp(this->dimei, other.dimei, 15);
    }
};
Q_DECLARE_METATYPE(struct pubVehicleSurroundingST)
//地图车辆8401和8402的合集
using pubVehicleMapST=struct pubVehicleCollWarnningST;
//版本号
#if 0
struct versionST
{
    uint8_t versionflag;
    uint8_t termialType;
    uint8_t vehicleType;
    uint8_t ccu;
    uint32_t mcu;
    uint32_t icu;
    uint32_t vcu;
    uint32_t hmi;
    uint32_t cgu;
    short vehicleNum;
    char imei[15];
};
#else

struct versionST
{
    uint8_t versionflag;
    uint8_t ccu[6];
    uint8_t mcu[6];
    uint8_t icu[6];
    uint8_t vcu[6];
    uint8_t hmi[6];
    uint8_t cgu[6];
    short vehicleNum;
    char imei[15];
};
#endif
Q_DECLARE_METATYPE(struct versionST)
union versionUN
{
    struct versionST data;
    uint8_t var8[sizeof(struct versionST)];
};
//文件下载
struct fileDownST
{
    char fileType;
#if(PROTOCAL_TYPE==2)
    char areaType;
#endif
    char result;
    char dImei[15];
    char md5[32];
    char dName[20];
    char fileDir[1024];
};
Q_DECLARE_METATYPE(struct fileDownST)

//下载地图文件
struct pubVehicleDownMapST
{
    char md5[32];
    char fileDir[1024];
};
Q_DECLARE_METATYPE(struct pubVehicleDownMapST)
//for emit
struct MAP_RESULT_ST
{
    char cmd;//0:g2hDownMapFileSignal 地图  1:g2hDownTaskFileSignal 任务
    int ret;
    QString filePath;
    QStringList taskList;
};
Q_DECLARE_METATYPE(struct MAP_RESULT_ST)
//文件上传
struct fileUpST
{
    char fileType;
    char areaType;
    char result;
    char md5[32];
    char fileDir[100];
};
Q_DECLARE_METATYPE(struct fileUpST)

//设备名称
struct pubVehicleDevNameST
{
    char localimei[15];
    char localname[20];

    bool operator==(const struct pubVehicleDevNameST& other)
    {
        if(strncmp(this->localimei, other.localimei, 15) == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};
Q_DECLARE_METATYPE(struct pubVehicleDevNameST)

struct pubVehicleDevNameStateST
{
    char localimei[15];
    char localname[20];
    char vehicleState;
    char workState;

    bool operator==(const struct pubVehicleDevNameStateST& other)
    {
        if((strncmp(this->localimei, other.localimei, 15) == 0) && (strncmp(this->localname, other.localname, 20) == 0))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool operator < (const struct pubVehicleDevNameStateST& other) const
    {
        return strncmp(this->localimei, other.localimei, 15);
    }
};
Q_DECLARE_METATYPE(struct pubVehicleDevNameStateST)


//网关开机
struct startUpST
{
    float vehicleLength;
    float vehicleWidth;
    float vehicleHeight;
    char startFlag; //上一次关机状态;00正常开机，01异常开机
};
Q_DECLARE_METATYPE(struct startUpST)

/**
 * 辅助车辆特有数据结构
 */


/**
 * 矿卡特有数据结构
 */
//平台运营状态确认
struct pubVehicleResWorkStatuST
{
    short serial;
    short id;
    char  ret;
};
Q_DECLARE_METATYPE(struct pubVehicleResWorkStatuST)

//hmi发送请求任务
struct pubVehicleReqTaskST
{
    double lat;
    double lng;
    float heading;
    char taskType;  //01装载任务，02卸载任务
    short mater;
};
Q_DECLARE_METATYPE(struct pubVehicleReqTaskST)

//hmi收到远程遥控
struct pubVehicleResRemoteST
{
    char opt;  //01遥控开始，02遥控结束
    char result;//01接受命令，02拒绝命令
};

Q_DECLARE_METATYPE(struct pubVehicleResRemoteST)
//平台相应任务文件
struct pubVehicleResTaskST
{
    char taskType;  //任务类型  
    short taskNum;  //任务编码
    char result;    //调度结果
    char remoteimei[15];    //目标imei
    char md5[32];   //文件md5
    char remotename[20];    //目标名称
#ifdef MAINPATHEXCAVATORID_FEATURE
    short mainPathExcavatorID;
#endif
    char taskFile[1024];    //文件路径
};
Q_DECLARE_METATYPE(struct pubVehicleResTaskST)
//路劝申请
struct pubVehicleReqRoadST
{
    char roadType;
    char roadDir;
    short roadNum;
};
Q_DECLARE_METATYPE(struct pubVehicleReqRoadST)

//路权申请响应
struct pubVehicleResRoadST
{
    char roadType;
    char roadDir;
    short roadNum;
    char result;
};
Q_DECLARE_METATYPE(struct pubVehicleResRoadST)
//行车许可下发列表
struct pubDrivePermissionListST
{
    u16 roadNum;
    double Lat;
    double Lng;
    char MD5[32];
    char fileName[39];
    uint32_t endPointNum;
};
Q_DECLARE_METATYPE(struct pubDrivePermissionListST)
//区域边界下发
struct pubAreaBoundaryST
{
    u16 taskNum;
    u8 MD5[32];
    char fileName[39];
};
Q_DECLARE_METATYPE(struct pubAreaBoundaryST)
//动静态测试反馈
struct pubMasterTestST
{
    u8 taskType;//0x01:动态，0x02:静态测试
    u8 result;//1成功 2失败
    u8 reason;//0:未知
};
Q_DECLARE_METATYPE(struct pubMasterTestST)
//主控发送预警
struct pubVehicleMasterWarnningST
{
    float disy;     //纵向预警距离
    float speedy;   //纵向速度
    float disx;     //横向预警距离
    float speedx;   //横向速度
    float tim;      //预警时间单位s
    char warnningType;  //障碍物类型
};
Q_DECLARE_METATYPE(struct pubVehicleMasterWarnningST)
//hmi清除主控故障H→G(0F0E)
#ifndef NEW0F0E_FEATURE
struct pubClearVehicleErrorST
{
    char errorType;
    char errorFlag[16];
};
#else
struct pubClearVehicleErrorST
{
    char webid[8];
    char errorType;
    uint8_t flags[16];
};
#endif
Q_DECLARE_METATYPE(struct pubClearVehicleErrorST)
//平台发送远程控制指令
struct pubWebControlST
{
    char type;
};
Q_DECLARE_METATYPE(struct pubWebControlST)
#pragma pack()

//文件操作
#define PUB_FILE_SUCCESS        true
#define PUB_FILE_FALSE          false
//道路类型
#define PUB_VEHICLE_ROAD_TYPE_CROSSING      0X01    //路口
#define PUB_VEHICLEROAD_TYPE_ONEWAY         0X02    //单行道
#define PUB_VEHICLEROAD_DIR_LOAD            0X01    //满载
#define PUB_VEHICLEROAD_DOR_UNLOAD          0X02    //卸载
//车辆运行状态操作结构体(hmi中涉及)
#define PUB_VEHICLE_READY_WORK_STATU            0X01        //车辆就绪运行状态
#define PUB_VEHICLE_DEALY_WORK_STATU            0X02        //车辆延时停运状态
#define PUB_VEHICLE_FAULT_WORK_STATU            0X03        //车辆故障停运状态
#define PUB_VEHICLE_BACKUP_WORK_STATU           0X04        //车辆备用停运状态
#define PUB_VEHICLE_NA_WORK_STATU               0X05        //车辆备用停运状态
//物料编码
#define PUB_MATER_NULL                  0X0000        //无物料
#define PUB_MATER_CLOUD_ROCK            0X0001        //白云岩
#define PUB_MATER_HIGH_ORE              0X0002        //高磁矿
#define PUB_MATER_LOW_ORE               0X0003        //低磁矿
#define PUB_MATER_HIGH_OXYGEN           0X0004        //高氧矿
#define PUB_MATER_LOW_OXYGEN            0X0005        //低氧矿
#define PUB_MATER_BIG_BLOCK             0X0006        //大块
#define PUB_MATER_EARTH                 0X0007        //土方
#define PUB_MATER_WASTE_ROCK            0X0008        //废岩
//现场调度操作结构体，主要驶入、驶出等(master,dsrc)
#define PUB_WORK_OPT_LOAD_TYPE      (0X01)      //装在作业
#define PUB_WORK_OPT_UNLOAD_TYPE    (0X02)      //卸载作业
//载货状态
#define PUB_WORK_STATE_EMPTYLOAD_TYPE   (0X01)      //空载作业
#define PUB_WORK_STATE_FULLLOAD_TYPE    (0X02)      //满载作业
#define PUB_WORK_STATE_HALFLOAD_TYPE    (0X03)      //半载作业
#define PUB_WORK_STATE_EMPTYUNLOAD_TYPE (0X04)      //空载卸载状态
#define PUB_WORK_STATE_HALFUNLOAD_TYPE  (0X03)      //半载卸载状态

//现场调度任务
#define PUB_WORK_OPT_NULL_TASK       (0X00)      //网关相应主控现场作业任务调度-无任务
#define PUB_WORK_OPT_OUT_TASK        (0X01)      //网关相应主控现场作业任务调度-可驶离
#define PUB_WORK_OPT_IN_TASK         (0X02)      //网关相应主控现场作业任务调度-可驶入
#define PUB_WORK_OPT_FINISH_TASK     (0X03)      //网关相应主控现场作业任务调度-停靠完成
#define PUB_WORK_OPT_STOP_TASK       (0X04)      //网关相应主控现场作业任务调度-紧急停车任务
#define PUB_WORK_OPT_RECOVER_TASK    (0X05)      //网关相应主控现场作业任务调度-停车后恢复正常行驶
#define PUB_WORK_OPT_MAN_TASK        (0X06)      //网关相应主控现场作业任务调度-停车后人工接管
#define PUB_WORK_OPT_RE_IN_TASK      (0X07)      //网关相应主控现场作业任务调度-停车后重新退出重进
#define PUB_WORK_OPT_UNLOAD_TASK     (0X08)      //网关相应主控现场作业任务调度-允许卸料
#define PUB_WORK_OPT_CLEAR_TASK      (0X11)      //清除驶入驶出任务

#define PUB_WORK_OPT_RUN             (0X09)      //网关相应主控现场作业任务调度-正在驶入

//延时时长
#define PUB_DELAY_TIME_10MIN            0x0258        //10分钟
#define PUB_DELAY_TIME_30MIN            0x0708        //30分钟
#define PUB_DELAY_TIME_60MIN            0x0E10        //60分钟
#define PUB_DELAY_TIME_M30MIN           0xFFFF        //大于60分钟
//故障原因
#define PUB_FAULT_PWR                   0x0001        //电机故障
#define PUB_FAULT_CABLE                 0x0002        //电缆故障
#define PUB_FAULT_CABLE_BROKEN          0x0003        //电铲断钢缆
#define PUB_FAULT_MOTOR_FAULT           0x0003        //矿卡发动机故障
#define PUB_FAULT_OTHER                 0x0FFF        //其他故障
//延时原因
#define PUB_DELAY_STOP                  0xE001        //暂停派遣
#define PUB_DELAY_SCHEDULE_FALUT        0xE002        //调度失败
#define PUB_DELAY_ERROR_ROAD            0xE003        //无效道路
#define PUB_DELAY_AVOIDBLAST            0xE004        //避炮
#define PUB_DELAY_OIL                   0xE005        //停车加油
#define PUB_DELAY_REMOTE                0xF001        //遥控停车
#define PUB_DELAY_EMERGENCY             0xF002        //紧急制动
#define PUB_DELAY_OVER                  0xF003        //任务终止
#define PUB_DELAY_CHANGE                0xF004        //障碍物停车
#define PUB_DELAY_COLD                  0xF005        //车辆冷却
#define PUB_DELAY_HITCH                 0xF006        //短时故障
#define PUB_DELAY_WAIT                  0xF007        //等待路权
//延时原因-电铲
#define PUB_DELAY_EXCAVATOR_CHECK                 0X0001        //点检
#define PUB_DELAY_EXCAVATOR_CHANGE                0X0002        //交接班
#define PUB_DELAY_EXCAVATOR_FOOD                  0X0003        //班中餐
#define PUB_DELAY_EXCAVATOR_PUSH                  0x0004        //推铲根
#define PUB_DELAY_EXCAVATOR_AVOIDBLAST            0X0005        //避炮
#define PUB_DELAY_EXCAVATOR_SHORT                 0X0006        //短时故障
#define PUB_DELAY_EXCAVATOR_SWEEP                 0X0007        //扫货
#define PUB_DELAY_EXCAVATOR_CARGO                 0X0008        //倒货
#define PUB_DELAY_EXCAVATOR_DISPLACE              0X0009        //移位
#define PUB_DELAY_EXCAVATOR_DIG                   0X000A        //挖根底
#define PUB_DELAY_EXCAVATOR_POWER                 0X000B        //停电
#define PUB_DELAY_EXCAVATOR_DROP                  0X000C        //掉牙尖
#define PUB_DELAY_EXCAVATOR_PUMP                  0X000D        //挖泵坑
#define PUB_DELAY_EXCAVATOR_CABLE                 0X000E        //倒电缆
#define PUB_DELAY_FORBID_IN			              0X000F        //暂停收车
#define PUB_DELAY_WEATHER_REASON                  0X0010        //矿卡天气待机
#define PUB_DELAY_OTHER_REASON                    0X0FFF        //其他延时
//车辆类型
#define PUB_MINING_TRUCk		0X0081	//矿卡
#define PUB_ELECTRIC_SHOVEL		0X0082	//电铲
#define PUB_BULLDOZER	 		0X0083	//推土机
#define PUB_EXCAVATOR	  		0X0084	//挖掘机
#define PUB_MAP_COLLECTOR 		0X0085	//路径采集车
#define PUB_SPRINKLER    		0X0086	//洒水车
#define PUB_ROLLER			  	0X0087	//平地机
#define PUB_CRUSH               0x0088  //破碎站
#define PUB_CAR    				0X0095	//外来小车
//文件上传结果
#define PUB_COPY_FILE_FALSE     0X00    //文件上传---拷贝失败
#define PUB_COPY_FILE_TRUE      0X01    //文件上传---拷贝成功
#define PUB_UP_FILE_FALSE       0X02    //文件上传---上传失败
#define PUB_UP_FILE_TRUE        0X03    //文件上传---上传成功

#endif // HMICMDST_H
