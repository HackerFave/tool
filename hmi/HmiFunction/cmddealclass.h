#ifndef CMDDEALCLASS_H
#define CMDDEALCLASS_H

#include "resendclass.h"
#include "hmicmdst.h"
#include "jt808.h"
#include "../pub.h"
#include <QMap>
#include <QList>
#include <QDir>
#include <QtConcurrentRun>

#define DEV_COMPAT_VERSION 2
#define DEV_MAIN_VERSION 0
#define DEV_SUB_VERSION 1


#define ARRAY_ERR_BUFSIZE 512

//HMI接收网关命令（通用）
#define	G2H_COM_ACK_CMD             (0X8001)    //通用应答
#define G2H_COM_ACK_LEN             2
#define G2H_POS_CMD                 (0X0200)    //位置定时上传
#define G2H_POS_LEN                 sizeof(struct pubVehiclePosST)
#define G2H_REQ_WORKON_STATU_CMD    (0X8F02)    //平台下发请求运营状态变更
#define G2H_REQ_WORKON_STATU_LEN    sizeof(struct pubVehicleWorkStatuST)
#define G2H_RES_WORKON_STATU_CMD    (0X8C01)    //接收平台确认运营状态变更
#define G2H_RES_WORKON_STATU_LEN    sizeof(struct pubVehicleResWorkStatuST)
#define G2H_REQ_WORK_OPT_CMD        (0X0305)    //显示网关发送作业现场调度请求命令
#define G2H_REQ_WORK_OPT_LEN        sizeof(struct pubVehicleReqWorkOptST)
#define G2H_RES_WORK_OPT_CMD        (0X0306)    //网关发送hmi，主控确定收到调度命令。
#define G2H_RES_WORK_OPT_LEN        sizeof(struct pubVehicleReqWorkOptST)
#define G2H_RES_WORK_L_OPT_CMD      (0X8306)    //网关发送作业现场调度控制命令
#define G2H_RES_WORK_L_OPT_LEN      sizeof(struct pubVehicleResWorkOptST)
#define G2H_COLL_WARNNING_CMD       (0X8401)    //V2X预警
#define G2H_COLL_WARNNING_LEN       sizeof(struct pubVehicleCollWarnningST)
#define G2H_DOWN_MAP_CMD            (0X8F0E)    //下载地图文件
#define G2H_DOWN_MAP_LEN            sizeof(struct pubVehicleDownMapST)
#define G2H_VERSION_CMD             (0X00F1)    //版本号
#define G2H_VERSION_LEN             sizeof(struct versionST)
#define G2H_DOWN_FILE_CMD           (0X8F0A)    //下载文件
#define G2H_DOWN_FILE_LEN           sizeof(struct fileDownST)
#define G2H_START_UP_CMD            (0x1030)    //开机命令
#define G2H_START_UP_LEN            sizeof(struct startUpST)
#define G2H_SHUT_DOWN_ACK_CMD       (0X1050)    //回复关机应答
#define G2H_SHUT_DOWN_ACK_LEN       2
#define G2H_DEV_NAME_CMD            (0X8002)    //获取设备imei和名称
#define G2H_DEV_NAME_LEN            sizeof(struct pubVehicleDevNameST)
#define G2H_PARA_QUERY_CMD          (0X8104)    //服务器参数查询
#define G2H_PARA_NOTIFY_CMD          (0X8103)   //服务器参数下发
#define G2H_CLEAR_VEHICLE_ERROR_CMD          (0X0F0E)   //清除主控故障
#define G2H_CLEAR_VEHICLE_ERROR_LEN          sizeof(struct pubClearVehicleErrorST)

//HMI接收网关命令（辅助车辆）
#define G2H_MAT_ACK_CMD             (0X8F04)    //网关回复物料变更
#define G2H_MAT_ACK_LEN             2
//HMI接收网关命令（矿卡）
#define G2H_REQ_TASK_CMD            (0X0F0C)    //接收请求任务
#define G2H_REQ_TASK_LEN            sizeof(struct pubVehicleReqTaskST)
#define G2H_RES_TASK_CMD            (0X8F0C)    //接收相应任务
#define G2H_RES_TASK_LEN            sizeof(struct pubVehicleResTaskST)
#define G2H_REQ_ROAD_CMD            (0X0F07)    //请求路权
#define G2H_REQ_ROAD_LEN            sizeof(struct pubVehicleReqRoadST)
#define G2H_RES_ROAD_CMD            (0X8F07)    //相应请求路权
#define G2H_RES_ROAD_LEN            sizeof(struct pubVehicleResRoadST)
#define G2H_FREE_ROAD_CMD           (0X0F08)    //释放路权
#define G2H_FREE_ROAD_LEN           sizeof(struct pubVehicleReqRoadST)
#define G2H_TEMPORARY_TASK_CMD      (0X8F0D)    //临时调度
#define G2H_TEMPORARY_TASK_LEN      1
#define G2H_MASTER_WARNNING_CMD     (0X0402)    //主控发送预警信息
#define G2H_MASTER_WARNNING_LEN     sizeof(struct pubVehicleMasterWarnningST)
#define G2H_UP_FILE_ACK_CMD         (0X0F0B)    //文件上传结果
#define G2H_UP_FILE_ACK_LEN         1
#define G2H_DRIVE_PERMISSION_CMD      (0X8F0F)    //行车许可下发
#define G2H_AREA_BOUNDARY_CMD         (0X8F10)    //区域边界下发
#define G2H_AREA_BOUNDARY_LEN       sizeof(struct pubAreaBoundaryST)

//HMI发送网关命令（通用）
#define H2G_COM_ACK_CMD             (0X0001)    //通用应答
#define H2G_COM_ACK_LEN             2
#define H2G_HEART_CMD               (0x0002)    //HMI心跳包
#define H2G_HEART_LEN               0
#define H2G_VERSION_CMD             (0X00F1)    //版本号
#define H2G_VERSION_LEN             sizeof(struct versionST)
#define H2G_SET_WORK_STATU_CMD      (0X0F01)    //营运状态变更设置
#define H2G_SET_WORK_STATU_LEN      sizeof(struct pubVehicleWorkStatuST)
#define H2G_UP_FILE_CMD             (0X8F0B)    //上传文件
#define H2G_UP_FILE_LEN             sizeof(struct fileUpST)
#define H2G_START_UP_ACK_CMD        (0X2030)    //回复开机应答
#define H2G_START_UP_ACK_LEN        4
#define H2G_SHUT_DOWN_CMD           (0X2050)    //发送关机命令
#define H2G_SHUT_DOWN_LEN           4
#define H2G_CHECK_PARA_CMD           (0X0104)    //发送服务器参数查询-HMI开机发送
#define H2G_CHECK_PARA_LEN           0
//HMI发送网关命令（矿卡）
#define H2G_SET_LOAD_CMD            (0X8403)    //设置空满载状态
#define H2G_SET_LOAD_LEN            1
#define H2G_REQ_WORK_OPT_CMD        (0X0305)    //请求现场作业操作
#define H2G_REQ_WORK_OPT_LEN        sizeof(struct pubVehicleReqWorkOptST)
//HMI发送网关命令（辅助车辆）
#define H2G_REQ_MATER_CMD           (0X0F03)    //HMI设置物料变更
#define H2G_REQ_MATER_LEN           2
#define H2G_RES_WORK_OPT_CMD        (0X8306)    //HMI发送作业现场调度控制命令
#define H2G_RES_WORK_OPT_LEN        sizeof(struct pubVehicleResWorkOptST)
#define H2G_COLLECT_MAP_CMD         (0X0F0F)    //HMI发送地图采集命令
#define H2G_COLLECT_MAP_LEN         1

class CmdDealClass : public ReSendClass
{
    Q_OBJECT
public:
    CmdDealClass();
    void udpRecvMsgDeal(QByteArray array);
    int  cmdAnalyzeDeal(char *buf, int len);
    //g->h通用
    int  g2hComAckDeal(jt808MsgSt msg);
    int  g2hPosDeal(jt808MsgSt msg);
    int  g2hReqWorkOnStatusDeal(jt808MsgSt msg);
    int  g2hResWorkOnStatuDeal(jt808MsgSt msg);
    int  g2hReqWorkOptDeal(jt808MsgSt msg);
    int  g2hResWorkOptDeal(jt808MsgSt msg);
    int  g2hResWorkLOptDeal(jt808MsgSt msg);
    int  g2hCollWarningDeal(jt808MsgSt msg);
    int  g2hDownMapDeal(jt808MsgSt msg);
    int  g2hVersionDeal(jt808MsgSt msg);
    int  g2hDownFileDeal(jt808MsgSt msg);
    int  g2hStartUpDeal(jt808MsgSt msg);
    int  g2hShutDownAckDeal(jt808MsgSt msg);
    int  g2hDevNameDeal(jt808MsgSt msg);
	int  g2hParaQueryDeal(jt808MsgSt msg);
    int  g2hParaNotifyDeal(jt808MsgSt msg);
    int  g2hUpFileAckDeal(jt808MsgSt msg);
    //g->h辅助车辆
    int  g2hMatAckDeal(jt808MsgSt msg);
     //g->h矿卡
    int  g2hReqTaskDeal(jt808MsgSt msg);
    int  g2hResTaskDeal(jt808MsgSt msg);
    int  g2hReqRoadDeal(jt808MsgSt msg);
    int  g2hResRoadDeal(jt808MsgSt msg);
    int  g2hFreeRoadDeal(jt808MsgSt msg);
    int  g2hTemporaryTaskDeal(jt808MsgSt msg);
    int  g2hMasterWarnningDeal(jt808MsgSt msg);
    int  g2hDrivePermissionDeal(jt808MsgST msg);
    int  g2hAreaBoundaryDeal(jt808MsgST msg);
    //h->g通用
    int  h2gComAckDeal(char *buf, short cmd);
    int  h2gHeartDeal(char *buf);
    int  h2gVersionDeal(char *buf, struct versionST);
    int  h2gSetWorkStatuDeal(char *buf, pubVehicleWorkStatuST statu);
    int  h2gUpFileDeal(char *buf, fileUpST);
    int  h2gStartUpAckDeal(char *buf);
    int  h2gShutDownDeal(char *buf);
    //h->g矿卡
    int  h2gSetLoadDeal(char *buf, char load);
    //h->g辅助车辆
    int  h2gResWorkOptDeal(char *buf, pubVehicleResWorkOptST opt);
    int  h2gReqWorkOptDeal(char *buf, pubVehicleReqWorkOptST opt);
    int  h2gReqMaterDeal(char *buf, short mater);
    int  h2gCollectMapDeal(char *buf, char type);
    //功能函数
    void warnninOpt(char *buf, int len);
    void g2hDownMapThread(jt808MsgSt msg);
    void g2hDownFileThread(jt808MsgSt msg);
    void g2hResTaskThread(jt808MsgSt msg);

private:
    QByteArray cmdRecvArray;
    qint64 linkLastTime;
    #define STATU_TIMER_INTERVAL 1000
    QTimer *statuTimer;
    #define HEART_TIMER_INTERVAL 5000
    #define HEART_TIMER_TIMEOUT  5000
    QTimer *heartTimer;
    QMap<int, QString> warnInMap;
	
    std::vector<size_t> warnList;

private:
    void g2hDrivePermissionThread(jt808MsgST msg);


signals:
    //hmi通讯层与应用层接口
    void gatewayOfflineSignal(bool);  //网关掉线
    void g2hPosSignal(struct pubVehiclePosST);  //网关发送hmi位置信息
    void g2hReqWorkOnStatuSignal(struct pubVehicleWorkStatuST); //平台发送网关运营状态变更
    void g2hResWorkOnStatuSignal(struct pubVehicleResWorkStatuST); //收到平台运营状态变更确认
    void g2hReqWorkOptSignal(struct pubVehicleReqWorkOptST);//网关请求现场作业0305
    void g2hReqWorkRealOptSignal(struct pubVehicleReqWorkOptST);//网关回应实际现场作业状态0306
    void g2hResWorkOptSignal(struct pubVehicleResWorkOptST); //辅助车辆（网关已收到矿卡现场作业），矿卡（接收到辅助车辆相应作业信息）
    void g2hCollWarnningSignal(struct pubVehicleCollWarnningST); //预警
    //void g2hDownMapSignal(struct pubVehicleDownMapST); //接收网关下载地图命令
    void g2hDownMapFileSignal(int, QString);   //发送地图文件更新信号，bool表示成功失败
    void g2hDownTaskFileSignal(bool, QString, QStringList);   //发送任务文件更新信号，bool表示成功失败
    void g2hDownPermissionFileSignal(bool, QStringList,quint32);
    void g2hVersionSignal(struct versionST); //版本号
    //void g2hDownFileSignal(struct fileDownST ); //下载文件
    void g2hStartUpSignal(struct startUpST); //网关发送开机命令
    void g2hShutDownAckSignal(); //网关关机应答
    void g2hDevNameSignal(struct pubVehicleDevNameST); //网关检权获取名称后发送给hmi
    void g2hMatAckSignal(short mater); //物料变更
    void g2hReqTaskSignal(struct pubVehicleReqTaskST ); //请求任务
    void g2hResTaskSignal(struct pubVehicleResTaskST ); //接收平台响应任务
    void g2hReqRoadSignal(struct pubVehicleReqRoadST ); //申请路权
    void g2hResRoadSignal(struct pubVehicleResRoadST ); //平台回复路权申请
    void g2hFreeRoadSignal(struct pubVehicleReqRoadST ); //平台下发释放路权
    void g2hTemporaryTaskSignal(char ); //平台下发临时调度作业
    void g2hMasterWarnningSignal(struct pubVehicleMasterWarnningST); //主控发送预警
    void g2hInsertWarnningSignal(int); //新增报警
    void g2hRemoveWarnningSignal(int); //取消报警

    void g2hDrivePermissionSignal(struct pubDrivePermissionListST);//行车许可文件

	
public slots:
    void heartSlots();
    void statuSlots();
    void h2gCheckParaCmdSlot();
    //hmi应用层与通讯层接口
    void  h2gSetWorkStatuSlots(pubVehicleWorkStatuST statu); //设置运用状态
    void  h2gUpFileSlots(fileUpST); //文件上传,fileDir不用部分为0
    void  h2gShutDownSlots();  //发送网关关机命令
    void  h2gSetLoadSlots(char); //设置矿卡空满载
    void  h2gReqMaterSlots(short mater); //设置物料类型
    void  h2gResWorkOptSlots(pubVehicleResWorkOptST opt); //相应现场调度
	void  h2gReqWorkOptSlots(struct pubVehicleReqWorkOptST); //发送请求作业操作命令，主要驶推出作业
    void  h2gCollectMapDealSlots(char); //接收hmi地图采集信号
    void  h2gSendHmiVerSlots(struct versionST version);//发送版本号


    void h2gClearVehicleError(pubClearVehicleErrorST err);

};

#endif // CMDDEALCLASS_H
