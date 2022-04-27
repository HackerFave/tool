#ifndef VehicleInfoModel_H
#define VehicleInfoModel_H

#include "Interface/IModel.h"
#include <string>
#include <unordered_map>
#include "../../Protocol/hmicmd.h"

const std::unordered_map<uint8_t,std::string> c_MasterInfoMap = {
    {0x01, "接收到遥控停车指令"},
    {0x02, "接收到遥控启动指令"},
    {0x03, "接收到遥控紧急停车指令"},
    {0x04, "等待允许驶入指令"},
    {0x08, "即将进行重新停靠"},
    {0x09, "到达收车等待区"},
    {0x0A, "到达加油等待区"},
    {0x0B, "到达晾车区域"}
};

const std::unordered_map<uint8_t,std::string> c_PosWorkInfoMap = {
    {PUB_VEHICLE_READY_WORK_STATU, "进入就绪状态"},
    {PUB_VEHICLE_DEALY_WORK_STATU, "进入延时状态"},
    {PUB_VEHICLE_FAULT_WORK_STATU, "进入故障状态"},
    {PUB_VEHICLE_BACKUP_WORK_STATU, "进入备用状态"}
};

const std::unordered_map<uint8_t,std::string> c_PosAiInfoMap = {
    {0x00, "转入人工驾驶模式"},
    {0x01, "转入自动驾驶模式"},
    {0x02, "转入远程驾驶模式"}
};

const std::unordered_map<uint8_t,std::string> c_PosLoadInfoMap = {
    {0x00, "变更为空载状态"},
    {0x03, "变更为满载状态"},
    {0x01, "变更为半载状态"}
};

const std::unordered_map<uint8_t,std::string> c_PosTaskInfoMap = {
    {0x02, "发送驶入申请成功"},
    {0x04, "接收到允许驶入指令"}
};

const std::unordered_map<uint8_t,std::string> c_ResWorkExcavatorOptInfoMap = {
    {PUB_WORK_OPT_OUT_TASK, "接收到驶离指令"},
    {PUB_WORK_OPT_STOP_TASK, "接收到紧急停车指令"},
    {PUB_WORK_OPT_MAN_TASK, "接收到人工接管指令"},
    {PUB_WORK_OPT_RE_IN_TASK, "接收到重新停靠指令"},
    {0X0A, "开始装车"},
};

const std::unordered_map<uint8_t,std::string> c_ResWorkCrusherOptInfoMap = {
    {0x08, "开始卸料"},
    {0x09, "禁止卸料"}
};

const std::unordered_map<uint8_t,std::string> c_ResTaskCmdInfoMap = {
    {0x01, "装载任务"},
    {0x02, "卸载任务"},
    {0x03, "收车指令"},
    {0x04, "电铲移动任务"},
    {0x05, "动态测试任务"},
    {0x07, "加油任务"}
};

const std::unordered_map<uint16_t,std::string> c_ReqWorkonStateInfoMap = {
    {0xe003, "平台调度路径失败"},
    {0xe002, "平台调度目标失败"}
};

class VehicleInfoModel : public IModel
{
    Q_OBJECT
public:
    explicit VehicleInfoModel(QObject *parent = nullptr);

    enum ModelCmdType
    {
        MC_UpdateAll=0,
        MC_AddSysInfo,

        MC_Max,
        MC_Oil
    };

    void   addSysInfo(QString &info);
    void   setOilValue(float value);
    QString m_allInfo;
    QString m_newInfo;
    float _oilvalue;//油量
};

#endif // VehicleInfoModel_H
