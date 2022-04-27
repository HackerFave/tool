#ifndef HMICMD
#define HMICMD

#include "hmicmdst.h"

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
#define G2H_RES_WORK_L_OPT_CMD      (0X8306)    //网关发送作业现场调度控制命令V2X
#define G2H_RES_WORK_L_OPT_LEN      sizeof(struct pubVehicleResWorkOptST)
#define G2H_RES_WORK_EXCAVATOR_OPT_CMD      (0X8307)    //网关发送作业现场调度控制命令V2N电铲
#define G2H_RES_WORK_EXCAVATOR_OPT_LEN      sizeof(struct pubVehicleResWorkOpt8307ST)
#define G2H_RES_WORK_CRUSHER_OPT_CMD      (0X8308)    //网关发送作业现场调度控制命令V2N破碎站
#define G2H_RES_WORK_CRUSHER_OPT_LEN      sizeof(struct pubVehicleResWorkOpt8308ST)
#define G2H_COLL_WARNNING_CMD       (0X8401)    //V2X预警
#define G2H_COLL_WARNNING_LEN       sizeof(struct pubVehicleCollWarnningST)
#define G2H_VEHICLE_AROUND_CMD       (0X8402)    //V2X周边车辆信息
#define G2H_VEHICLE_AROUND_LEN       sizeof(struct pubVehicleSurroundingST)
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
#define H2G_CLEAR_VEHICLE_ERROR_CMD          (0X0F0E)   //清除主控故障
#define H2G_CLEAR_VEHICLE_ERROR_LEN          sizeof(struct pubClearVehicleErrorST)
#define G2H_CLEAR_VEHICLE_ERROR_CMD          (0X8F0E)   //主控的 清除主控故障 回复（平台）
#define G2H_CLEAR_VEHICLE_ERROR_LEN          (H2G_CLEAR_VEHICLE_ERROR_LEN)   //主控的 清除主控故障 回复（平台）
#define G2H_WEB_CONTROL_CMD          (0X8F09)   //web平台控制指令
#define G2H_WEB_CONTROL_CMD_LEN          sizeof(struct pubWebControlST)

//HMI接收网关命令（辅助车辆）
#define G2H_MAT_ACK_CMD             (0X8F04)    //网关回复物料变更
#define G2H_MAT_ACK_LEN             2
#define H2G_DIGGER_UPDATE_BOUNDARY_CMD         (0X8B01)    //HMI接收停靠位分组状态更新
//HMI接收网关命令（矿卡）
#define G2H_RES_REMOTE_CMD            (0X2062)    //主控响应远程遥控驾驶控制命令
#define G2H_RES_REMOTE_LEN            2
#define G2H_REQ_REMOTE_CMD            (0X2060)    //网关接收到驾驶舱发送的远程遥控请求命令后，发送此命令到主控
#define G2H_REQ_REMOTE_LEN            1
#define G2H_MASTER_INFO_CMD            (0X0500)    //主控触发事件
#define G2H_MASTER_INFO_LEN            1


#define G2H_REQ_MAN_CMD            (0X0404)    //接收请求人工驾驶任务
#define G2H_REQ_MAN_LEN            1
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
#define G2H_RES_TEST_CMD         (0X8F1C)    //测试响应
#define G2H_RES_TEST_LEN       sizeof(struct pubMasterTestST)

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
#define H2G_PLAY_SOUND_CMD           (0X0F20)    //给网关发送语音报警
#define H2G_PLAY_SOUND_LEN           100
//HMI发送网关命令（矿卡）
#define H2G_REQ_OFF_VEHICLE_CMD            (0X2063)    //晾车命令
#define H2G_REQ_OFF_VEHICLE_LEN            1
#define H2G_SET_LOAD_CMD            (0X8403)    //设置空满载状态
#define H2G_SET_LOAD_LEN            1
#define H2G_REQ_WORK_OPT_CMD        (0X0305)    //请求现场作业操作
#define H2G_REQ_WORK_OPT_LEN        sizeof(struct pubVehicleReqWorkOptST)
#define H2G_REQ_WORK_OPT0F21_CMD        (0X0F21)    //请求HMI->主控现场作业操作
#define H2G_REQ_WORK_OPT0F21_LEN        sizeof(struct pubVehicleReqWorkOpt0F21ST)
#define H2G_REQ_TEST_CMD            (0X0F1C)    //测试任务
#define H2G_REQ_TEST_LEN            1
//HMI发送网关命令（辅助车辆）
#define H2G_REQ_MATER_CMD           (0X0F03)    //HMI设置物料变更
#define H2G_REQ_MATER_LEN           2
#define H2G_RES_WORK_OPT_CMD        (0X8306)    //HMI发送作业现场调度控制命令
#define H2G_RES_WORK_OPT_LEN        sizeof(struct pubVehicleResWorkOptST)
#define H2G_COLLECT_MAP_CMD         (0X0F0F)    //HMI发送地图采集命令
#define H2G_COLLECT_MAP_LEN         1
#define H2G_DIGGER_READY_CMD         (0X8B02)    //HMI发送digger就位命令
#define H2G_DIGGER_READY_LEN         sizeof(struct pubDiggerReadyST)


#endif // HMICMD
