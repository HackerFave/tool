#include "HmiCmdPackage.h"

std::atomic<uint16_t> JT808::serialNum(0);

HmiCmdPackage::HmiCmdPackage()
{

}
//网关到hmi数据
int  HmiCmdPackage::hmiDecodeG2H(uint8_t *buf, int len)
{
    Jt808MsgST msg;
    if(JT808::deCodeMsg(JT808::defaultMsg, buf, len, msg) < 0) {
        return -1;
    }

    switch(msg.msgId)
    {
        case G2H_COM_ACK_CMD:           return g2hComAckDeal(msg);
        case G2H_POS_CMD:               return g2hPosDeal(msg);
        case G2H_REQ_WORKON_STATU_CMD:  return g2hReqWorkOnStatusDeal(msg);
        case G2H_RES_WORKON_STATU_CMD:  return g2hResWorkOnStatuDeal(msg);
//        case G2H_REQ_WORK_OPT_CMD:      return g2hReqWorkOptDeal(msg);
//        case G2H_RES_WORK_OPT_CMD:      return g2hResWorkOptDeal(msg);
//        case G2H_RES_WORK_L_OPT_CMD:    return g2hResWorkLOptDeal(msg);
        case G2H_RES_WORK_EXCAVATOR_OPT_CMD:  return g2hResWorkExcavatorOptDeal(msg);
        case G2H_RES_WORK_CRUSHER_OPT_CMD:    return g2hResWorkCrusherOptDeal(msg);
//        case G2H_COLL_WARNNING_CMD:     return g2hCollWarningDeal(msg);
//        case G2H_VEHICLE_AROUND_CMD:     return g2hVehicleAroundDeal(msg);
//        case G2H_DOWN_MAP_CMD:          return g2hDownMapDeal(msg);
//        case G2H_VERSION_CMD:           return g2hVersionDeal(msg);
//        case G2H_DOWN_FILE_CMD:         return g2hDownFileDeal(msg);
//        case G2H_START_UP_CMD:          return g2hStartUpDeal(msg);
//        case G2H_SHUT_DOWN_ACK_CMD:     return g2hShutDownAckDeal(msg);
//        case G2H_DEV_NAME_CMD:          return g2hDevNameDeal(msg);
//        //case G2H_MAT_ACK_CMD:           return g2hMatAckDeal(msg);
//        case G2H_REQ_MAN_CMD:           return g2hReqManDeal(msg);
//        case G2H_RES_REMOTE_CMD:        return g2hResRemoteDeal(msg);
        case G2H_MASTER_INFO_CMD:       return g2hMasterInfoDeal(msg);
//        case G2H_REQ_REMOTE_CMD:        return g2hReqRemoteDeal(msg);
        case G2H_RES_TEST_CMD:          return g2hResTestDeal(msg);
//        case G2H_REQ_TASK_CMD:          return g2hReqTaskDeal(msg);
        case G2H_RES_TASK_CMD:          return g2hResTaskDeal(msg);
//        case G2H_REQ_ROAD_CMD:          return g2hReqRoadDeal(msg);
//        case G2H_RES_ROAD_CMD:          return g2hResRoadDeal(msg);
//        case G2H_FREE_ROAD_CMD:         return g2hFreeRoadDeal(msg);
//        case G2H_TEMPORARY_TASK_CMD:    return g2hTemporaryTaskDeal(msg);
//        case G2H_MASTER_WARNNING_CMD:   return g2hMasterWarnningDeal(msg);
//        case G2H_PARA_QUERY_CMD:        return g2hParaQueryDeal(msg);
//        case G2H_PARA_NOTIFY_CMD:       return g2hParaNotifyDeal(msg);
//        case G2H_UP_FILE_ACK_CMD:       return g2hUpFileAckDeal(msg);
        case G2H_DRIVE_PERMISSION_CMD:  return g2hDrivePermissionDeal(msg);
//        //case H2G_DIGGER_UPDATE_BOUNDARY_CMD:  return g2hUpdateGroupStatusDeal(msg);
//        case G2H_AREA_BOUNDARY_CMD:     return g2hAreaBoundaryDeal(msg);
//        case G2H_WEB_CONTROL_CMD:       return g2hWebControlDeal(msg);
//        case G2H_CLEAR_VEHICLE_ERROR_CMD:       return g2hClearErrorDeal(msg);
//        case H2G_CLEAR_VEHICLE_ERROR_CMD:       return h2gClearErrorDeal(msg);
        default: qDebug("[0x%04x] no id\n", msg.msgId);     return -1;
    }
}

//G->H
int  HmiCmdPackage::g2hComAckDeal(Jt808MsgST &msg)
{
    if(msg.bodyLen != G2H_COM_ACK_LEN) {
        qWarning("g2hComAck length error,true[%d],recv[%d]\n", G2H_COM_ACK_LEN, msg.bodyLen);
        return -1;
    }

    unsigned short subId;
    memmove(&subId, msg.body, msg.bodyLen);
    qDebug("cmd recv g2hComAck, subId[0x%04x]", subId);

    switch(subId) {
    case H2G_VERSION_CMD:
    case H2G_SET_WORK_STATU_CMD:
    case H2G_UP_FILE_CMD:
    case H2G_START_UP_ACK_CMD:
    case H2G_SHUT_DOWN_CMD:
    case H2G_REQ_MATER_CMD:
    case H2G_RES_WORK_OPT_CMD:
    case H2G_REQ_WORK_OPT_CMD:
    case H2G_REQ_TEST_CMD:
    case H2G_SET_LOAD_CMD:
    case H2G_CHECK_PARA_CMD:
    case H2G_CLEAR_VEHICLE_ERROR_CMD:
    case H2G_REQ_OFF_VEHICLE_CMD:
    case H2G_DIGGER_READY_CMD:
        break;
    case H2G_COLLECT_MAP_CMD:
        break;
    }
    return 0;
}


int  HmiCmdPackage::g2hPosDeal(Jt808MsgST &msg)
{
    static int sWorkStatus = -1;
    static int sAiStatus = -1;
    static int sLoadStatus = -1;
    static int sTaskStatus = -1;

    if(msg.bodyLen != G2H_POS_LEN) {
        qWarning("g2hPos length error,true[%ld],recv[%d]\n", G2H_POS_LEN, msg.bodyLen);
        return -1;
    }

    struct pubVehiclePosST pos;
    memmove(reinterpret_cast<char *>(&pos), msg.body, msg.bodyLen);
    qDebug("cmd recv g2hPos:%d",pos.status);
    QVariantList datalist;
    datalist.append(msg.msgId);

    bool flag = false;
    if(static_cast<int>(pos.statu) != sWorkStatus){
        flag = true;
        sWorkStatus = pos.statu;
        datalist.append(sWorkStatus);
    }else{
        datalist.append(-1);
    }

    int tAiStatus = (pos.status >> 5) & 0x03 ;
    if(tAiStatus != sAiStatus){
        flag = true;
        sAiStatus = tAiStatus;
        datalist.append(sAiStatus);
    }else{
        datalist.append(-1);
    }

    int tLoadStatus = (pos.status >> 8) & 0x03 ;
    if(tLoadStatus != sLoadStatus){
        flag = true;
        sLoadStatus = tLoadStatus;
        datalist.append(sLoadStatus);
    }else{
        datalist.append(-1);
    }

    if(static_cast<int>(pos.taskWorkStatu) != sTaskStatus){
        flag = true;
        sTaskStatus = pos.taskWorkStatu;
        datalist.append(sTaskStatus);
    }else{
        datalist.append(-1);
    }

    if(pos.speed >= pos.maxSpeed){
        datalist.append(1);
    }else{
        datalist.append(-1);
    }

    if(flag){
        ModuleDir tempMd;
        tempMd.append(MD_VehicleInfo);

        m_moduleDirs.append(tempMd);
        m_cmds.append(VI_G2HmiInfo);
        m_datas.append(QVariant::fromValue(datalist));
    }

    return 0;
}

int  HmiCmdPackage::g2hReqWorkOnStatusDeal(Jt808MsgST &msg)
{
    if(msg.bodyLen != G2H_REQ_WORKON_STATU_LEN) {
        qWarning("g2hReqWorkOnStatus length error,true[%ld],recv[%d]\n", G2H_REQ_WORKON_STATU_LEN, msg.bodyLen);
        return -1;
    }

    struct pubVehicleWorkStatuST tWorkStatus;
    memmove(reinterpret_cast<char *>(&tWorkStatus), msg.body, msg.bodyLen);
    qDebug("cmd recv g2hReqWorkOnStatus, statu[%d], fault[%d], delayTime[%d] ", tWorkStatus.statuType, tWorkStatus.fault, tWorkStatus.delayTim);

    if(tWorkStatus.statuType == PUB_VEHICLE_DEALY_WORK_STATU){
        ModuleDir tempMd;
        tempMd.append(MD_VehicleInfo);

        QVariantList datalist;
        datalist.append(msg.msgId);
        datalist.append(tWorkStatus.fault);

        m_moduleDirs.append(tempMd);
        m_cmds.append(VI_G2HmiInfo);
        m_datas.append(QVariant::fromValue(datalist));
    }
    return 0;
}

int  HmiCmdPackage::g2hResWorkExcavatorOptDeal(Jt808MsgST &msg)
{
    if(msg.bodyLen != G2H_RES_WORK_EXCAVATOR_OPT_LEN) {
        qWarning("g2hResWorkExcavatorOptDeal length error,true[%ld],recv[%d]\n", G2H_RES_WORK_EXCAVATOR_OPT_LEN, msg.bodyLen);
        return -1;
    }

    struct pubVehicleResWorkOpt8307ST work;
    memmove(reinterpret_cast<char *>(&work), msg.body, msg.bodyLen);
    qDebug("cmd recv g2hResWorkExcavatorOptDeal, taskType[0x%02x], cmdType[0x%02x], remotename[%s], remoteimei[%s], tim[%lld]",
              work.taskType, work.cmdType, QByteArray(work.remotename,20).data(), QByteArray(work.remoteimei,15).data(), work.utcTime);

    ModuleDir tempMd;
    tempMd.append(MD_VehicleInfo);

    QVariantList datalist;
    datalist.append(msg.msgId);
    datalist.append(static_cast<uint8_t>(work.cmdType));

    m_moduleDirs.append(tempMd);
    m_cmds.append(VI_G2HmiInfo);
    m_datas.append(QVariant::fromValue(datalist));

    return 0;
}

int  HmiCmdPackage::g2hResWorkCrusherOptDeal(Jt808MsgST &msg)
{
    if(msg.bodyLen != G2H_RES_WORK_CRUSHER_OPT_LEN) {
        qWarning("g2hResWorkCrusherOptDeal length error,true[%ld],recv[%d]\n", G2H_RES_WORK_CRUSHER_OPT_LEN, msg.bodyLen);
        return -1;
    }

    struct pubVehicleResWorkOpt8308ST work;
    memmove(reinterpret_cast<char *>(&work), msg.body, msg.bodyLen);
    qDebug("cmd recv g2hResWorkCrusherOptDeal, taskType[0x%02x], cmdType[0x%02x], remoteimei[%s], remotename[%s], tim[%lld]",
              work.taskType, work.cmdType, QByteArray(work.remoteimei,15).data(), QByteArray(work.remotename,20).data(), work.utcTime);

    ModuleDir tempMd;
    tempMd.append(MD_VehicleInfo);

    QVariantList datalist;
    datalist.append(msg.msgId);
    datalist.append(static_cast<uint8_t>(work.cmdType));

    m_moduleDirs.append(tempMd);
    m_cmds.append(VI_G2HmiInfo);
    m_datas.append(QVariant::fromValue(datalist));
    return 0;
}

int HmiCmdPackage::g2hMasterInfoDeal(Jt808MsgST &msg)
{

    if(msg.bodyLen != G2H_MASTER_INFO_LEN) {
        qWarning("g2hMasterInfoDeal length error,true[%d],recv[%d]\n", G2H_MASTER_INFO_LEN, msg.bodyLen);
        return -1;
    }

    uint8_t eventType = msg.body[0];

    ModuleDir tempMd;
    tempMd.append(MD_VehicleInfo);

    QVariantList datalist;
    datalist.append(msg.msgId);
    datalist.append(eventType);

    m_moduleDirs.append(tempMd);
    m_cmds.append(VI_G2HmiInfo);
    m_datas.append(QVariant::fromValue(datalist));

    return 0;
}

int  HmiCmdPackage::g2hResTestDeal(Jt808MsgST &msg)
{
    if(msg.bodyLen < G2H_RES_TEST_LEN) {
        qWarning("g2hResTest length error,true[%ld],recv[%d]\n", G2H_RES_TEST_LEN, msg.bodyLen);
    }

    struct pubMasterTestST tTest;
    memmove((char *)&tTest, msg.body, msg.bodyLen);

    ModuleDir tempMd;
    tempMd.append(MD_VehicleInfo);

    QVariantList datalist;
    datalist.append(msg.msgId);
    datalist.append(QVariant::fromValue(tTest));

    m_moduleDirs.append(tempMd);
    m_cmds.append(VI_G2HmiInfo);
    m_datas.append(QVariant::fromValue(datalist));

    return 0;
}

int  HmiCmdPackage::g2hResTaskDeal(Jt808MsgST &msg)
{
//    qDebug("will run task thread");
    if(msg.bodyLen < 10) {
        qWarning("g2hResTask length error,true[%ld],recv[%d]\n", G2H_RES_TASK_LEN, msg.bodyLen);
    }

    struct pubVehicleResTaskST task;
    memset((char *)&task, 0, sizeof(task));
    memmove((char *)&task, msg.body, msg.bodyLen);
#ifdef MAINPATHEXCAVATORID_FEATURE
    qDebug("cmd recv g2hResTask, result[0x%02x], taskType[0x%02x], file[%s], remoteimei[%s], pathid[%d]",
              task.result, task.taskType, task.taskFile, QByteArray(task.remoteimei, 15).data(),task.mainPathExcavatorID);
#else
    qDebug("cmd recv g2hResTask, result[0x%02x], taskType[0x%02x], file[%s], remoteimei[%s]",
              task.result, task.taskType, task.taskFile, QByteArray(task.remoteimei, 15).data());
#endif

    ModuleDir tempMd;
    tempMd.append(MD_VehicleInfo);

    QVariantList datalist;
    datalist.append(msg.msgId);
    datalist.append(static_cast<uint8_t>(task.taskType));

    m_moduleDirs.append(tempMd);
    m_cmds.append(VI_G2HmiInfo);
    m_datas.append(QVariant::fromValue(datalist));

    return 0;
}

int HmiCmdPackage::g2hDrivePermissionDeal(Jt808MsgST &msg)
{
    static uint32_t sEndNum = 0;
    u8 len=msg.body[0];

    if(msg.bodyLen != len*sizeof(struct pubDrivePermissionListST)+1) {
        qWarning("g2hDrivePermissionDeal length error,true[%ld],recv[%d]\n", len*sizeof(struct pubDrivePermissionListST)+1, msg.bodyLen);
        return -1;
    }

    //行车许可下发收到后处理
    struct pubDrivePermissionListST permission;
    memmove(reinterpret_cast<char *>(&permission), msg.body+1, sizeof(struct pubDrivePermissionListST));//只取第一个许可
    qDebug("cmd recv g2hDrivePermission, roadnum[0x%04x], endPointNum[%d]",
              permission.roadNum, permission.endPointNum);

    if(sEndNum != permission.endPointNum)
    {
        ModuleDir tempMd;
        tempMd.append(MD_VehicleInfo);

        QVariantList datalist;
        datalist.append(msg.msgId);
        datalist.append(static_cast<uint32_t>(permission.endPointNum));

        m_moduleDirs.append(tempMd);
        m_cmds.append(VI_G2HmiInfo);
        m_datas.append(QVariant::fromValue(datalist));

        sEndNum = permission.endPointNum;
    }

    return 0;
}

int  HmiCmdPackage::g2hResWorkOnStatuDeal(Jt808MsgST &msg)
{
    if(msg.bodyLen != G2H_RES_WORKON_STATU_LEN) {
        qWarning("g2hResWorkOnStatus length error,true[%ld],recv[%d]\n", G2H_RES_WORKON_STATU_LEN, msg.bodyLen);
        return -1;
    }

//    char arr[1024];
//    int arrLen;
//    arrLen = h2gComAckDeal(arr, static_cast<short>(G2H_RES_WORKON_STATU_CMD));

    struct pubVehicleResWorkStatuST statu;
    memmove(reinterpret_cast<char *>(&statu), msg.body, msg.bodyLen);
    qDebug("cmd recv g2hResWorkOnStatu, statu[%d], ret[%d]\n", statu.id, statu.ret);

    return 0;
}

//H->G
int  HmiCmdPackage::hmiEncodeComAck(uint8_t *buf, short cmd)
{
    Jt808MsgST msg;
    msg.msgId = H2G_COM_ACK_CMD;
    msg.bodyLen = H2G_COM_ACK_LEN;
    memmove(msg.body, reinterpret_cast<char *>(&cmd), H2G_COM_ACK_LEN);
    return JT808::enCodeMsg(JT808::defaultMsg, msg, buf);
}

int  HmiCmdPackage::hmiEncodeReqWorkOpt(uint8_t *buf, struct pubVehicleReqWorkOpt0F21ST &opt)
{
    Jt808MsgST msg;
    msg.msgId = H2G_REQ_WORK_OPT0F21_CMD;
    msg.bodyLen = H2G_REQ_WORK_OPT0F21_LEN;
    memmove(msg.body, reinterpret_cast<char *>(&opt), msg.bodyLen);
    return JT808::enCodeMsg(JT808::defaultMsg, msg, buf);
}
