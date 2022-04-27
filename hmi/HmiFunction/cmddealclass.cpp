#include "cmddealclass.h"
#include "jt808.h"
#include "systemcmd.h"
#include "paraparsing.h"
#include "filemanager.h"
#include "log.h"

#include "posinfo.h"
#include<cstring>
#pragma pack(1)
struct paraTypeST
{
    uint32_t ID;
    uint8_t  len;
    char data;
};
struct resParaST
{
    uint16_t index;
    uint8_t num;
    struct paraTypeST data;
};
#pragma pack()

CmdDealClass::CmdDealClass()
{

    LOG_DEBUG("cmd deal class start,HMI Version:%02d,%02d.%02d",DEV_COMPAT_VERSION,DEV_MAIN_VERSION,DEV_SUB_VERSION);

    linkLastTime = QDateTime::currentMSecsSinceEpoch();
    statuTimer = new QTimer(this);
    statuTimer->setInterval(STATU_TIMER_INTERVAL);
    connect(statuTimer, SIGNAL(timeout()), this, SLOT(statuSlots()));
    statuTimer->start();

    heartTimer = new QTimer(this);
    heartTimer->setInterval(HEART_TIMER_INTERVAL);
    connect(heartTimer, SIGNAL(timeout()), this, SLOT(heartSlots()));
    heartTimer->start();



    QTimer::singleShot(5000, this, SLOT(h2gCheckParaCmdSlot()));

}

void CmdDealClass::udpRecvMsgDeal(QByteArray array)
{
    cmdRecvArray.append(array);
    while(1) {
        int ret = cmdRecvArray.indexOf("\r\n");
        if(ret < 0) {
            if(cmdRecvArray.size() > ARRAY_ERR_BUFSIZE) {
                cmdRecvArray.clear();
            }
            break;
        }

        QByteArray tmp = cmdRecvArray.left(ret+2);

        if(tmp.size() > 6) {
           // qDebug()<<"子类接收报文数据："<<array<<"TC-----test";
            cmdAnalyzeDeal(tmp.data(), tmp.size());
        }
        cmdRecvArray = cmdRecvArray.right(cmdRecvArray.size()-ret-2);
    }
}

int CmdDealClass::cmdAnalyzeDeal(char *buf, int len)
{
    jt808MsgSt msg;
    if(Jt808::decode(buf, len, msg) < 0) {
        return -1;
    }
    linkLastTime = QDateTime::currentMSecsSinceEpoch();
   // qDebug()<<"子类解析协议地址："<<buf<<len<<msg.id<<"TC-----test";
    switch(msg.id)
    {
        case G2H_COM_ACK_CMD:           return g2hComAckDeal(msg);
        case G2H_POS_CMD:               return g2hPosDeal(msg);
        case G2H_REQ_WORKON_STATU_CMD:  return g2hReqWorkOnStatusDeal(msg);
        case G2H_RES_WORKON_STATU_CMD:  return g2hResWorkOnStatuDeal(msg);
        case G2H_REQ_WORK_OPT_CMD:      return g2hReqWorkOptDeal(msg);
        case G2H_RES_WORK_OPT_CMD:      return g2hResWorkOptDeal(msg);
        case G2H_RES_WORK_L_OPT_CMD:    return g2hResWorkLOptDeal(msg);
        case G2H_COLL_WARNNING_CMD:     return g2hCollWarningDeal(msg);
//        case G2H_DOWN_MAP_CMD:          return g2hDownMapDeal(msg);
        case G2H_VERSION_CMD:           return g2hVersionDeal(msg);
        case G2H_DOWN_FILE_CMD:         return g2hDownFileDeal(msg);
        case G2H_START_UP_CMD:          return g2hStartUpDeal(msg);
        case G2H_SHUT_DOWN_ACK_CMD:     return g2hShutDownAckDeal(msg);
        case G2H_DEV_NAME_CMD:          return g2hDevNameDeal(msg);
        case G2H_MAT_ACK_CMD:           return g2hMatAckDeal(msg);
        case G2H_REQ_TASK_CMD:          return g2hReqTaskDeal(msg);
        case G2H_RES_TASK_CMD:          return g2hResTaskDeal(msg);
        case G2H_REQ_ROAD_CMD:          return g2hReqRoadDeal(msg);
        case G2H_RES_ROAD_CMD:          return g2hResRoadDeal(msg);
        case G2H_FREE_ROAD_CMD:         return g2hFreeRoadDeal(msg);
        case G2H_TEMPORARY_TASK_CMD:    return g2hTemporaryTaskDeal(msg);
        case G2H_MASTER_WARNNING_CMD:   return g2hMasterWarnningDeal(msg);
        case G2H_PARA_QUERY_CMD:        return g2hParaQueryDeal(msg);
        case G2H_PARA_NOTIFY_CMD:        return g2hParaNotifyDeal(msg);
        case G2H_UP_FILE_ACK_CMD:       return g2hUpFileAckDeal(msg);
        case G2H_DRIVE_PERMISSION_CMD:  return g2hDrivePermissionDeal(msg);
        case G2H_AREA_BOUNDARY_CMD:     return g2hAreaBoundaryDeal(msg);
        default: LOG_DEBUG("[0x%04x] no id\n", msg.id);     return -1;
    }
}

//G->H
int  CmdDealClass::g2hComAckDeal(jt808MsgSt msg)
{
    if(msg.bodyLen != G2H_COM_ACK_LEN) {
        LOG_DEBUG("g2hComAck length error,true[%d],recv[%d]\n", G2H_COM_ACK_LEN, msg.bodyLen);
        return -1;
    }

    unsigned short subId;
    memmove(&subId, msg.body, msg.bodyLen);
    LOG_DEBUG("cmd recv g2hComAck, subId[0x%04x]", subId);

    switch(subId) {
    case H2G_VERSION_CMD:
    case H2G_SET_WORK_STATU_CMD:
    case H2G_UP_FILE_CMD:
    case H2G_START_UP_ACK_CMD:
    case H2G_SHUT_DOWN_CMD:
    case H2G_REQ_MATER_CMD:
    case H2G_RES_WORK_OPT_CMD:
    case H2G_REQ_WORK_OPT_CMD:
    case H2G_SET_LOAD_CMD:
    case H2G_CHECK_PARA_CMD:
    case G2H_CLEAR_VEHICLE_ERROR_CMD:
        removeReSendMsg(subId);
        break;
    }
    return 0;
}

int  CmdDealClass::g2hPosDeal(jt808MsgSt msg)
{
    if(msg.bodyLen != G2H_POS_LEN) {
        LOG_DEBUG("g2hPos length error,true[%d],recv[%d]\n", G2H_POS_LEN, msg.bodyLen);
        return -1;
    }

    struct pubVehiclePosST pos;
    memmove(reinterpret_cast<char *>(&pos), msg.body, msg.bodyLen);
    LOG_DEBUG("cmd recv g2hPos");

    emit g2hPosSignal(pos);
    //set system time xuzk2
    static uint8_t once=0;
    if(once==0)
    {
        once=1;
        QDateTime nowTime=QDateTime::fromTime_t(pos.utcTim/1000);
        nowTime=nowTime.addSecs(3600*8);
        QString strTime=nowTime.toString("yyyy-MM-dd hh:mm:ss");
        QString strCMD="date -s \""+strTime+"\"";
        system(strCMD.toLatin1().data());
        system("hwclock -w");
        qDebug()<<"设置时间:"<<nowTime;
    }
    //set system time end
	warnninOpt(msg.body, 16);

    return 0;
}

int  CmdDealClass::g2hReqWorkOnStatusDeal(jt808MsgSt msg)
{
    if(msg.bodyLen != G2H_REQ_WORKON_STATU_LEN) {
        LOG_DEBUG("g2hReqWorkOnStatus length error,true[%d],recv[%d]\n", G2H_REQ_WORKON_STATU_LEN, msg.bodyLen);
        return -1;
    }

    char arr[1024];
    int arrLen;
    arrLen = h2gComAckDeal(arr, static_cast<short>(G2H_REQ_WORKON_STATU_CMD));
    udpSendMsg(arr, arrLen);

    struct pubVehicleWorkStatuST statu;
    memmove(reinterpret_cast<char *>(&statu), msg.body, msg.bodyLen);
    LOG_DEBUG("cmd recv g2hReqWorkOnStatus, statu[%d], fault[%d], delayTime[%d] ", statu.statuType, statu.fault, statu.delayTim);
    emit g2hReqWorkOnStatuSignal(statu);

    return 0;
}

int  CmdDealClass::g2hResWorkOnStatuDeal(jt808MsgSt msg)
{
    if(msg.bodyLen != G2H_RES_WORKON_STATU_LEN) {
        LOG_DEBUG("g2hResWorkOnStatus length error,true[%d],recv[%d]\n", G2H_RES_WORKON_STATU_LEN, msg.bodyLen);
        return -1;
    }

    char arr[1024];
    int arrLen;
    arrLen = h2gComAckDeal(arr, static_cast<short>(G2H_RES_WORKON_STATU_CMD));
    udpSendMsg(arr, arrLen);

    struct pubVehicleResWorkStatuST statu;
    memmove(reinterpret_cast<char *>(&statu), msg.body, msg.bodyLen);
    LOG_DEBUG("cmd recv g2hResWorkOnStatu, statu[%d], ret[%d]\n", statu.id, statu.ret);
    emit g2hResWorkOnStatuSignal(statu);

    return 0;
}

int  CmdDealClass::g2hReqWorkOptDeal(jt808MsgSt msg)//0305
{
    if(msg.bodyLen != G2H_REQ_WORK_OPT_LEN) {
        LOG_DEBUG("g2hReqWorkOpt length error,true[%d],recv[%d]\n", G2H_REQ_WORK_OPT_LEN, msg.bodyLen);
        return -1;
    }

    char arr[1024];
    int arrLen;
    arrLen = h2gComAckDeal(arr, G2H_REQ_WORK_OPT_CMD);
    udpSendMsg(arr, arrLen);

    struct pubVehicleReqWorkOptST work;
    memset((char*)(&work), 0, sizeof(struct pubVehicleReqWorkOptST));
    memmove(reinterpret_cast<char *>(&work), msg.body, msg.bodyLen);
    LOG_DEBUG("cmd recv g2hReqWorkOpt, taskType[0x%02x], cmdType[0x%02x], localimei[%s], remoteimei[%s], tim[%lld]\n",
              work.taskType, work.cmdType, QByteArray(work.localimei,15).data(),
              QByteArray(work.remoteimei, 15).data(), work.utcTime);
    emit g2hReqWorkOptSignal(work);

    return 0;
}

int  CmdDealClass::g2hResWorkOptDeal(jt808MsgSt msg)//0306
{
    if(msg.bodyLen != G2H_RES_WORK_OPT_LEN) {
        LOG_DEBUG("g2hResWorkOpt length error,true[%d],recv[%d]\n", G2H_RES_WORK_OPT_LEN, msg.bodyLen);
        return -1;
    }

    char arr[1024];
    int arrLen;
    arrLen = h2gComAckDeal(arr, G2H_RES_WORK_OPT_CMD);
    udpSendMsg(arr, arrLen);

    struct pubVehicleReqWorkOptST work;
    memset((char*)(&work), 0, sizeof(struct pubVehicleReqWorkOptST));
    memmove(reinterpret_cast<char *>(&work), msg.body, msg.bodyLen);
    LOG_DEBUG("cmd recv g2hResWorkOpt, taskType[0x%02x], cmdType[0x%02x], localimei[%s], remoteimei[%s], tim[%lld]",
              work.taskType, work.cmdType, QByteArray(work.localimei,15).data(), QByteArray(work.remoteimei,15).data(), work.utcTime);
    emit g2hReqWorkRealOptSignal(work);

    return 0;
}

int  CmdDealClass::g2hResWorkLOptDeal(jt808MsgSt msg)
{
    if(msg.bodyLen != G2H_RES_WORK_L_OPT_LEN) {
        LOG_DEBUG("g2hResWorkOpt length error,true[%d],recv[%d]\n", G2H_RES_WORK_L_OPT_LEN, msg.bodyLen);
        return -1;
    }

    char arr[1024];
    int arrLen;
    arrLen = h2gComAckDeal(arr, G2H_RES_WORK_L_OPT_CMD);
    udpSendMsg(arr, arrLen);

    struct pubVehicleResWorkOptST work;
    memmove(reinterpret_cast<char *>(&work), msg.body, msg.bodyLen);
    LOG_DEBUG("cmd recv g2hResWorkOpt, taskType[0x%02x], cmdType[0x%02x], localimei[%s], remoteimei[%s], tim[%lld]",
              work.taskType, work.cmdType, QByteArray(work.localimei,15).data(), QByteArray(work.remoteimei,15).data(), work.utcTime);
    emit g2hResWorkOptSignal(work);

    return 0;
}


int  CmdDealClass::g2hCollWarningDeal(jt808MsgSt msg)
{
    if(msg.bodyLen != G2H_COLL_WARNNING_LEN) {
        LOG_DEBUG("g2hCollWarning length error,true[%d],recv[%d]\n", G2H_COLL_WARNNING_LEN, msg.bodyLen);
        return -1;
    }

    char arr[1024];
    int arrLen;
    arrLen = h2gComAckDeal(arr, static_cast<short>(G2H_COLL_WARNNING_CMD));
    udpSendMsg(arr, arrLen);

    struct pubVehicleCollWarnningST coll;
    memmove(reinterpret_cast<char *>(&coll), msg.body, msg.bodyLen);
    LOG_DEBUG("cmd recv g2hCollWarning");
    emit g2hCollWarnningSignal(coll);

    return 0;
}

int  CmdDealClass::g2hDownMapDeal(jt808MsgSt msg)
{
    QtConcurrent::run(this, &CmdDealClass::g2hDownMapThread, msg);
    return 0; 
}

void CmdDealClass::g2hDownMapThread(jt808MsgSt msg)
{
    if(msg.bodyLen < 10) {
        LOG_DEBUG("g2hDownMap length error,true[%d],recv[%d]\n", G2H_DOWN_MAP_LEN, msg.bodyLen);
        return;
    }

    char arr[1024];
    int arrLen;
    arrLen = h2gComAckDeal(arr, static_cast<short>(G2H_DOWN_MAP_CMD));
    udpSendMsg(arr, arrLen);

    struct pubVehicleDownMapST map;
    memset(reinterpret_cast<char *>(&map), 0, sizeof(map));
    memmove(reinterpret_cast<char *>(&map), msg.body, msg.bodyLen);
    LOG_DEBUG("cmd recv g2hDownMap, dir[%s], md5[%s]", map.fileDir, QByteArray(map.md5,32).data());

    QString filePath;
    FileManager::instance()->getMapMutex();
    int ret = FileManager::instance()->fileDownLoadDeal(QString("map/")+QString(map.fileDir),
                                                        map.md5,
                                                        QString(ParaParsing::instance()->paraSt.pub.mapPath)+"/",
                                                        1,
                                                        filePath);
    FileManager::instance()->releaseMapMutex();

    system("sync");
    LOG_DEBUG("g2hDownMapFileSignal,ret[%d],recv[%s]\n", ret, filePath.toStdString().data());
    emit g2hDownMapFileSignal(ret, filePath);

    return ;
}

int  CmdDealClass::g2hVersionDeal(jt808MsgSt msg)
{
    if(msg.bodyLen != G2H_VERSION_LEN) {
        LOG_DEBUG("g2hVersion length error,true[%d],recv[%d]\n", G2H_VERSION_LEN, msg.bodyLen);
        return -1;
    }

    char arr[1024];
    int arrLen;
    arrLen = h2gComAckDeal(arr, G2H_VERSION_CMD);
    udpSendMsg(arr, arrLen);

    struct versionST version;
    memmove(reinterpret_cast<char *>(&version), msg.body, msg.bodyLen);
    LOG_DEBUG("cmd recv version, flag[0x%02x], cgu[%d]", version.versionflag, version.cgu);
    emit g2hVersionSignal(version);

    return 0;
}

int  CmdDealClass::g2hDownFileDeal(jt808MsgSt msg)
{
    QtConcurrent::run(this, &CmdDealClass::g2hDownFileThread, msg);
    return 0;    
}

void CmdDealClass::g2hDownFileThread(jt808MsgSt msg)
{
    if(msg.bodyLen < 10) {
        LOG_DEBUG("g2hDownFile length error,true[%d],recv[%d]\n", G2H_DOWN_FILE_LEN, msg.bodyLen);
        return ;
    }

    char arr[1024];
    int arrLen;
    arrLen = h2gComAckDeal(arr, G2H_DOWN_FILE_CMD);
    udpSendMsg(arr, arrLen);

    struct fileDownST file;
    memset((char *)&file, 0, sizeof(file));
    memmove((char *)&file, msg.body, msg.bodyLen);
    LOG_DEBUG("cmd recv g2hDownFile, result[0x%02x], fileType[%d], fileDir[%s], fileMd5[%s]",
                file.result, file.fileType, file.fileDir, file.md5);

    if(file.result != 0x01) {
        return ;
    }

    QString filePath;
    FileManager::instance()->getMapMutex();
    int ret = FileManager::instance()->fileDownLoadDeal(QString("map/")+QString(file.fileDir),
                                                        file.md5,
                                                        QString(ParaParsing::instance()->paraSt.pub.mapPath)+"/",
                                                        1,
                                                        filePath);
    FileManager::instance()->releaseMapMutex();

    LOG_DEBUG("g2hDownMapFileSignal,ret[%d],recv[%s]\n", ret, filePath.toStdString().data());

    if(ret == MapUpdateMode_NEW_FILE)
    {
        FileManager* file = FileManager::instance();
        QString untarCmd;

        QDir mapdir;
        mapdir.setPath(ParaParsing::instance()->paraSt.pub.mapPath);

        if(!mapdir.exists())
        {
            mapdir.mkdir(ParaParsing::instance()->paraSt.pub.mapPath);
        }

        LOG_DEBUG("Map Download Success!");

        untarCmd.append(QString("tar -xvf %1 -C %2").arg(filePath.left(filePath.length() -3))
                        .arg(ParaParsing::instance()->paraSt.pub.mapPath));

        QString gzipStr;
        gzipStr.append(QString("gzip -dk %1").arg(filePath));

        QString rmStr;
        rmStr.append("rm -rf %1").arg(filePath.left(filePath.length() - 3));

        QString renameStr;
        renameStr.append(QString("mv %1 %2").arg(filePath.left(filePath.length() - 7))
                         .arg(ParaParsing::instance()->paraSt.pub.mapPath+"/HDMap.db"));

        //qDebug()<<QDateTime::currentDateTime();
        file->getMapMutex();
        system(gzipStr.toLatin1().data());
        system("sync");
        system(untarCmd.toLatin1().data());
        system("sync");
        system(rmStr.toLatin1().data());
        system(renameStr.toLatin1().data());
        system("sync");
        file->releaseMapMutex();
        //qDebug()<<QDateTime::currentDateTime();
        //map->addDbInfo("/storage/mapinfo/HDMap.db");
    }

    emit g2hDownMapFileSignal(ret, filePath);
    
    return ;
}

int  CmdDealClass::g2hStartUpDeal(jt808MsgSt msg)
{
    if(msg.bodyLen != G2H_START_UP_LEN) {
        LOG_DEBUG("g2hStartUp length error,true[%d],recv[%d]\n", G2H_START_UP_LEN, msg.bodyLen);
        return -1;
    }

    char arr[1024];
    int arrLen;
    arrLen = h2gComAckDeal(arr, G2H_START_UP_CMD);
    udpSendMsg(arr, arrLen);

    arrLen = h2gStartUpAckDeal(arr);
    udpSendMsg(arr, arrLen);

    struct startUpST start;
    memmove(reinterpret_cast<char *>(&start), msg.body, msg.bodyLen);
    LOG_DEBUG("cmd recv g2hStartUp, length[%f], width[%f], height[%f], flag[0x%02x]",
                static_cast<double>(start.vehicleLength), static_cast<double>(start.vehicleWidth), static_cast<double>(start.vehicleHeight), start.startFlag);
    emit g2hStartUpSignal(start);

    return 0;
}

int  CmdDealClass::g2hShutDownAckDeal(jt808MsgSt msg)
{
    if(msg.bodyLen != G2H_SHUT_DOWN_ACK_LEN) {
        LOG_DEBUG("g2hShutDownAck length error,true[%d],recv[%d]\n", G2H_SHUT_DOWN_ACK_LEN, msg.bodyLen);
        return -1;
    }

    char arr[1024];
    int arrLen;
    arrLen = h2gComAckDeal(arr, G2H_SHUT_DOWN_ACK_CMD);
    udpSendMsg(arr, arrLen);

    emit g2hShutDownAckSignal();
    return 0;
}

int  CmdDealClass::g2hDevNameDeal(jt808MsgSt msg)
{
    if(msg.bodyLen != G2H_DEV_NAME_LEN) {
        LOG_DEBUG("g2hDevName length error,true[%d],recv[%d]\n", G2H_DEV_NAME_LEN, msg.bodyLen);
        return -1;
    }

    char arr[1024];
    int arrLen;
    arrLen = h2gComAckDeal(arr, static_cast<short>(G2H_DEV_NAME_CMD));
    udpSendMsg(arr, arrLen);

    struct pubVehicleDevNameST devName;
    memmove(reinterpret_cast<char *>(&devName), msg.body, msg.bodyLen);
    LOG_DEBUG("cmd recv 2hDevName, imei[%s], name[%s]", devName.localimei, devName.localname);
    emit g2hDevNameSignal(devName);

    return 0;
}

int  CmdDealClass::g2hMatAckDeal(jt808MsgSt msg)
{
    if(msg.bodyLen != G2H_MAT_ACK_LEN) {
        LOG_DEBUG("g2hMatAck length error,true[%d],recv[%d]\n", G2H_MAT_ACK_LEN, msg.bodyLen);
        return -1;
    }

    char arr[1024];
    int arrLen;
    arrLen = h2gComAckDeal(arr, static_cast<short>(G2H_MAT_ACK_CMD));
    udpSendMsg(arr, arrLen);

    short mater;
    memmove(reinterpret_cast<char *>(&mater), msg.body, msg.bodyLen);
    LOG_DEBUG("cmd g2hMatAck, mater[%d]", mater);
    emit g2hMatAckSignal(mater);

    return 0;
}

int  CmdDealClass::g2hReqTaskDeal(jt808MsgSt msg)
{
    if(msg.bodyLen != G2H_REQ_TASK_LEN) {
        LOG_DEBUG("g2hReqTask length error,true[%d],recv[%d]\n", G2H_REQ_TASK_LEN, msg.bodyLen);
        return -1;
    }

    char arr[1024];
    int arrLen;
    arrLen = h2gComAckDeal(arr, G2H_REQ_TASK_CMD);
    udpSendMsg(arr, arrLen);

    struct pubVehicleReqTaskST task;
    memmove(reinterpret_cast<char *>(&task), msg.body, msg.bodyLen);
    LOG_DEBUG("cmd recv g2hReqTask, taskType[0x%02x], mater[0x%02x]", task.taskType, task.mater);
    //HMI 2.1协议弃用
    //emit g2hReqTaskSignal(task);

    return 0;
}

int  CmdDealClass::g2hResTaskDeal(jt808MsgSt msg)
{
    qDebug("will run task thread");
    if(msg.bodyLen < 10) {
        LOG_DEBUG("g2hResTask length error,true[%d],recv[%d]\n", G2H_RES_TASK_LEN, msg.bodyLen);
    }

    char arr[1024];
    int arrLen;
    arrLen = h2gComAckDeal(arr, G2H_RES_TASK_CMD);
    udpSendMsg(arr, arrLen);

    struct pubVehicleResTaskST task;
    memset((char *)&task, 0, sizeof(task));
    memmove((char *)&task, msg.body, msg.bodyLen);
    LOG_DEBUG("cmd recv g2hResTask, result[0x%02x], taskType[0x%02x], file[%s], remoteimei[%s]",
              task.result, task.taskType, task.taskFile, QByteArray(task.remoteimei, 15).data());

    emit g2hResTaskSignal(task);

    QtConcurrent::run(this, &CmdDealClass::g2hResTaskThread, msg);

    return 0;
}

void CmdDealClass::g2hResTaskThread(jt808MsgSt msg)
{
    qDebug("in task respond thread");

    struct pubVehicleResTaskST task;
    memset((char *)&task, 0, sizeof(task));
    memmove((char *)&task, msg.body, msg.bodyLen);
    LOG_DEBUG("cmd recv g2hResTask, result[0x%02x], taskType[0x%02x], file[%s], remoteimei[%s]",
              task.result, task.taskType, task.taskFile, QByteArray(task.remoteimei, 15).data());

    if(task.result != 0x01)
    {
        return ;
    }

    QStringList taskList;
    taskList.clear();

    QString filePath;
    FileManager::instance()->getTaskMutex();
    bool ret = FileManager::instance()->fileDownLoadDeal(QString("task/")+QString(task.taskFile),
                                                         task.md5,
                                                         QString(ParaParsing::instance()->paraSt.pub.taskPath)+"/",
                                                         10,
                                                         filePath);
    FileManager::instance()->releaseTaskMutex();

    if(ret)
    {
        FileManager* file = FileManager::instance();
        QString untarCmd;

        QDir taskDir;
        taskDir.setPath(ParaParsing::instance()->paraSt.pub.taskPath);

        if(taskDir.exists())//存在目录
        {
            //移除目录中所有的文件夹
            FileManager::instance()->dirRemove(ParaParsing::instance()->paraSt.pub.taskPath,0);//删除所有目录
        }
        else//不存在创建目录
        {
            taskDir.mkdir(ParaParsing::instance()->paraSt.pub.taskPath);
        }

        QString childDir;
        childDir = QString(ParaParsing::instance()->paraSt.pub.taskPath+"%1").arg(QDateTime::currentDateTime().toString("/MM-dd_hh:mm:ss"));
        taskDir.mkdir(childDir);

        QString gzipStr;
        gzipStr.append(QString("gzip -dk %1").arg(filePath));

        untarCmd.append(QString("tar -xvf %1 -C %2").arg(filePath.left(filePath.length() - 3)).arg(childDir));

        QString rmStr;
        rmStr.append(QString("rm -rf %1").arg(filePath.left(filePath.length() - 3)));

        file->getTaskMutex();
        system(gzipStr.toLatin1().data());
        system(untarCmd.toLatin1().data());
        system(rmStr.toLatin1().data());
        file->releaseTaskMutex();

        taskList = file->anaysisTask(true, childDir);
    }

    emit g2hDownTaskFileSignal(ret, filePath, taskList);

    return ;
}

int  CmdDealClass::g2hReqRoadDeal(jt808MsgSt msg)
{
    if(msg.bodyLen != G2H_REQ_ROAD_LEN) {
        LOG_DEBUG("g2hReqRoad length error,true[%d],recv[%d]\n", G2H_REQ_ROAD_LEN, msg.bodyLen);
        return -1;
    }

    char arr[1024];
    int arrLen;
    arrLen = h2gComAckDeal(arr, G2H_REQ_ROAD_CMD);
    udpSendMsg(arr, arrLen);

    struct pubVehicleReqRoadST road;
    memmove(reinterpret_cast<char *>(&road), msg.body, msg.bodyLen);
    LOG_DEBUG("cmd recv g2hReqRoad, roadType[0x%02x], roadDir[0x%02x], roadNum[%d]", road.roadType, road.roadDir ,road.roadNum);
    //HMI2.1协议弃用
    //emit g2hReqRoadSignal(road);

    return 0;
}

int  CmdDealClass::g2hResRoadDeal(jt808MsgSt msg)
{
    if(msg.bodyLen != G2H_RES_ROAD_LEN) {
        LOG_DEBUG("g2hResRoad length error,true[%d],recv[%d]\n", G2H_RES_ROAD_LEN, msg.bodyLen);
        return -1;
    }

    char arr[1024];
    int arrLen;
    arrLen = h2gComAckDeal(arr, static_cast<short>(G2H_RES_ROAD_CMD));
    udpSendMsg(arr, arrLen);

    struct pubVehicleResRoadST road;
    memmove(reinterpret_cast<char *>(&road), msg.body, msg.bodyLen);
    LOG_DEBUG("cmd recv g2hResRoad, result[0x%02x], roadType[0x%02x], roadDir[0x%02x], roadNum[%d]", road.result, road.roadType, road.roadDir ,road.roadNum);
    //HMI2.1协议弃用
    //emit g2hResRoadSignal(road);

    return 0;
}

int  CmdDealClass::g2hFreeRoadDeal(jt808MsgSt msg)
{
    if(msg.bodyLen != G2H_FREE_ROAD_LEN) {
        LOG_DEBUG("g2hFreeRoad length error,true[%d],recv[%d]\n", G2H_FREE_ROAD_LEN, msg.bodyLen);
        return -1;
    }

    char arr[1024];
    int arrLen;
    arrLen = h2gComAckDeal(arr, G2H_FREE_ROAD_CMD);
    udpSendMsg(arr, arrLen);

    struct pubVehicleReqRoadST road;
    memmove(reinterpret_cast<char *>(&road), msg.body, msg.bodyLen);
    LOG_DEBUG("cmd recv g2hFreeRoad, roadType[0x%02x], roadDir[0x%02x], roadNum[%d]", road.roadType, road.roadDir ,road.roadNum);
    //HMI2.1协议弃用
    //emit g2hFreeRoadSignal(road);

    return 0;
}

int  CmdDealClass::g2hTemporaryTaskDeal(jt808MsgSt msg)
{
    if(msg.bodyLen != G2H_TEMPORARY_TASK_LEN) {
        LOG_DEBUG("g2hTemporaryTask length error,true[%d],recv[%d]\n", G2H_TEMPORARY_TASK_LEN, msg.bodyLen);
        return -1;
    }

    char arr[1024];
    int arrLen;
    arrLen = h2gComAckDeal(arr, static_cast<short>(G2H_TEMPORARY_TASK_CMD));
    udpSendMsg(arr, arrLen);

    char taskType;
    memmove(reinterpret_cast<char *>(&taskType), msg.body, msg.bodyLen);
    LOG_DEBUG("cmd recv g2hTemporaryTask, taskType[0x%02x]", taskType);
    emit g2hTemporaryTaskSignal(taskType);

    return 0;
}

int  CmdDealClass::g2hMasterWarnningDeal(jt808MsgSt msg)
{
    if(msg.bodyLen != G2H_MASTER_WARNNING_LEN) {
        LOG_DEBUG("g2hMasterWarnning length error,true[%d],recv[%d]\n", G2H_MASTER_WARNNING_LEN, msg.bodyLen);
        return -1;
    }

    char arr[1024];
    int arrLen;
    arrLen = h2gComAckDeal(arr, G2H_MASTER_WARNNING_CMD);
    udpSendMsg(arr, arrLen);

    struct pubVehicleMasterWarnningST warn;

    memmove(reinterpret_cast<char *>(&warn), msg.body, msg.bodyLen);
    LOG_DEBUG("cmd recv g2hMasterWarnning, taskType[0x%02x], disx[%f], disy[%f]",
              warn.warnningType, static_cast<double>(warn.disx), static_cast<double>(warn.disy));
    qDebug()<<"子类发送解析后数据到客户端hmi显示："<<warn.speedx<<msg.id<<"TC-----test";
    emit g2hMasterWarnningSignal(warn);
    return 0;
}

int CmdDealClass::g2hDrivePermissionDeal(jt808MsgST msg)
{
    u8 len=msg.body[0];
    if(msg.bodyLen != len*sizeof(struct pubDrivePermissionListST)+1) {
        LOG_DEBUG("g2hDrivePermissionDeal length error,true[%d],recv[%d]\n", len*sizeof(struct pubDrivePermissionListST)+1, msg.bodyLen);
        return -1;
    }

    char arr[1024];
    int arrLen;
    arrLen = h2gComAckDeal(arr, G2H_DRIVE_PERMISSION_CMD);
    udpSendMsg(arr, arrLen);

//    emit g2hDrivePermissionSignal(permission);
    QtConcurrent::run(this, &CmdDealClass::g2hDrivePermissionThread, msg);
    return 0;
}

void CmdDealClass::g2hDrivePermissionThread(jt808MsgST msg)
{

    struct pubDrivePermissionListST permission;
    memmove(reinterpret_cast<char *>(&permission), msg.body+1, sizeof(struct pubDrivePermissionListST));//只取第一个许可
    LOG_DEBUG("cmd recv g2hDrivePermission, roadnum[0x%04x], endPointNum[%d]",
              permission.roadNum, permission.endPointNum);

    QStringList permissionList;
    permissionList.clear();

    QString filePath;
    FileManager::instance()->getPermissionMutex();
    bool ret = FileManager::instance()->fileDownLoadDeal(QString("permit/")+QString(permission.fileName),
                                                         permission.MD5,
                                                         QString(ParaParsing::instance()->paraSt.pub.permissionPath)+"/",
                                                         3, filePath);
    FileManager::instance()->releasePermissionMutex();

    if(ret)
    {
        FileManager* file = FileManager::instance();
        QString untarCmd;

        QDir permissionDir;
        permissionDir.setPath(QString(ParaParsing::instance()->paraSt.pub.permissionPath)+"/");

        if(permissionDir.exists())//存在目录
        {
            //移除目录中所有的文件夹
            FileManager::instance()->dirRemove(ParaParsing::instance()->paraSt.pub.permissionPath,0);//删除所有目录
        }
        else//不存在创建目录
        {
            permissionDir.mkdir(QString(ParaParsing::instance()->paraSt.pub.permissionPath)+"/");
        }

        QString childDir;
        childDir = QString(ParaParsing::instance()->paraSt.pub.permissionPath+"%1").arg(QDateTime::currentDateTime().toString("/MM-dd_hh:mm:ss"));
        permissionDir.mkdir(childDir);

        QString gzipStr;
        gzipStr.append(QString("gzip -dk %1").arg(filePath));

        untarCmd.append(QString("tar -xvf %1 -C %2").arg(filePath.left(filePath.length() - 3)).arg(childDir));

        QString rmStr;
        rmStr.append(QString("rm -rf %1").arg(filePath.left(filePath.length() - 3)));

        file->getPermissionMutex();
        system(gzipStr.toLatin1().data());
        system(untarCmd.toLatin1().data());
        system(rmStr.toLatin1().data());
        file->releasePermissionMutex();

        permissionList = file->anaysisPermission(true, childDir);
    }

    emit g2hDownPermissionFileSignal(ret, permissionList,permission.endPointNum);

    return ;

}

int CmdDealClass::g2hAreaBoundaryDeal(jt808MsgST msg)
{
    if(msg.bodyLen != G2H_AREA_BOUNDARY_LEN) {
        LOG_DEBUG("g2hDrivePermissionDeal length error,true[%d],recv[%d]\n", G2H_AREA_BOUNDARY_LEN, msg.bodyLen);
        return -1;
    }

    char arr[1024];
    int arrLen;
    arrLen = h2gComAckDeal(arr, G2H_AREA_BOUNDARY_CMD);
    udpSendMsg(arr, arrLen);

    //区域边界下发收到后处理

    return 0;
}

int  CmdDealClass::g2hParaQueryDeal(jt808MsgSt msg)
{
    char arr[1024];
    int arrLen;
    (void)(msg);//去除msg警告
    arrLen = h2gComAckDeal(arr, G2H_PARA_QUERY_CMD);
    udpSendMsg(arr, arrLen);



    return 0;
}

int  CmdDealClass::g2hParaNotifyDeal(jt808MsgSt msg)
{
    char arr[1024];
    int arrLen;
    (void)(msg);//去除msg警告
    arrLen = h2gComAckDeal(arr, G2H_PARA_NOTIFY_CMD);
    udpSendMsg(arr, arrLen);

    return 0;
}

int  CmdDealClass::g2hUpFileAckDeal(jt808MsgSt msg)
{
    char arr[1024];
    int arrLen;
    if(msg.bodyLen != G2H_UP_FILE_ACK_LEN) {
        LOG_DEBUG("g2hUpFileAck length error,true[%d],recv[%d]\n", G2H_UP_FILE_ACK_LEN, msg.bodyLen);
        return -1;
    }

    char ret;
    memmove((char *)&ret, msg.body, msg.bodyLen);
    LOG_DEBUG("cmd recv g2hUpFileAck, ret[0x%02x]", ret);

    arrLen = h2gComAckDeal(arr, G2H_UP_FILE_ACK_CMD);
    udpSendMsg(arr, arrLen);


    return 0;
}

//H->G
int  CmdDealClass::h2gComAckDeal(char *buf, short cmd)
{
    jt808MsgSt msg;
    msg.id = H2G_COM_ACK_CMD;
    msg.bodyLen = H2G_COM_ACK_LEN;
    memmove(msg.body, reinterpret_cast<char *>(&cmd), H2G_COM_ACK_LEN);
    return Jt808::encode(buf, msg);
}

int  CmdDealClass::h2gHeartDeal(char *buf)
{
    jt808MsgSt msg;
    msg.id = H2G_HEART_CMD;
    msg.bodyLen = H2G_HEART_LEN;
    return Jt808::encode(buf, msg);
}

int  CmdDealClass::h2gVersionDeal(char *buf, struct versionST version)
{
    jt808MsgSt msg;
    msg.id = H2G_VERSION_CMD;
    msg.bodyLen = H2G_VERSION_LEN;
    memmove(msg.body, reinterpret_cast<char *>(&version), H2G_VERSION_LEN);
    return Jt808::encode(buf, msg);
}

int  CmdDealClass::h2gSetWorkStatuDeal(char *buf, struct pubVehicleWorkStatuST statu)
{
    jt808MsgSt msg;
    msg.id = H2G_SET_WORK_STATU_CMD;
    msg.bodyLen = H2G_SET_WORK_STATU_LEN;
    memmove(msg.body, reinterpret_cast<char *>(&statu), H2G_SET_WORK_STATU_LEN);
    return Jt808::encode(buf, msg);
}

int  CmdDealClass::h2gUpFileDeal(char *buf, fileUpST file)
{
    jt808MsgSt msg;
    msg.id = H2G_UP_FILE_CMD;
    msg.bodyLen = H2G_UP_FILE_LEN;//static_cast<quint16>(sizeof(file)-1024+strlen(file.fileDir));
    memmove(msg.body, reinterpret_cast<char *>(&file), msg.bodyLen);
    return Jt808::encode(buf, msg);
}

int  CmdDealClass::h2gStartUpAckDeal(char *buf)
{
    jt808MsgSt msg;
    msg.id = H2G_START_UP_ACK_CMD;
    msg.bodyLen = H2G_START_UP_ACK_LEN;
    memset(msg.body, 0, msg.bodyLen);
    return Jt808::encode(buf, msg);
}

int  CmdDealClass::h2gShutDownDeal(char *buf)
{
    jt808MsgSt msg;
    msg.id = H2G_SHUT_DOWN_CMD;
    msg.bodyLen = H2G_SHUT_DOWN_LEN;
    memset(msg.body, 0, msg.bodyLen);
    return Jt808::encode(buf, msg);
}

void CmdDealClass::h2gCheckParaCmdSlot()
{
    char arr[1024];
    int arrLen;
    jt808MsgSt msg;
    msg.id = H2G_CHECK_PARA_CMD;
    msg.bodyLen = H2G_CHECK_PARA_LEN;
    memset(msg.body, 0, msg.bodyLen);
    arrLen = Jt808::encode(arr, msg);

    udpSendMsg(arr, arrLen);
    addReSendMsg(H2G_CHECK_PARA_CMD, QByteArray(arr, arrLen));

}


int  CmdDealClass::h2gSetLoadDeal(char *buf, char load)
{
    jt808MsgSt msg;
    msg.id = H2G_SET_LOAD_CMD;
    msg.bodyLen = H2G_SET_LOAD_LEN;
    memmove(msg.body, &load, msg.bodyLen);
    return Jt808::encode(buf, msg);
}

int  CmdDealClass::h2gResWorkOptDeal(char *buf, struct pubVehicleResWorkOptST opt)
{
    jt808MsgSt msg;
    msg.id = H2G_RES_WORK_OPT_CMD;
    msg.bodyLen = H2G_RES_WORK_OPT_LEN;
    memmove(msg.body, reinterpret_cast<char *>(&opt), msg.bodyLen);
    return Jt808::encode(buf, msg);
}

int  CmdDealClass::h2gReqWorkOptDeal(char *buf, struct pubVehicleReqWorkOptST opt)
{
    jt808MsgSt msg;
    msg.id = H2G_REQ_WORK_OPT_CMD;
    msg.bodyLen = H2G_REQ_WORK_OPT_LEN;
    memmove(msg.body, reinterpret_cast<char *>(&opt), msg.bodyLen);
    return Jt808::encode(buf, msg);
}

int  CmdDealClass::h2gReqMaterDeal(char *buf, short mater)
{
    jt808MsgSt msg;
    msg.id = H2G_REQ_MATER_CMD;
    msg.bodyLen = H2G_REQ_MATER_LEN;
    memmove(msg.body, reinterpret_cast<char *>(&mater), msg.bodyLen);
    return Jt808::encode(buf, msg);
}

int  CmdDealClass::h2gCollectMapDeal(char *buf, char type)
{
    jt808MsgSt msg;
    msg.id = H2G_COLLECT_MAP_CMD;
    msg.bodyLen = H2G_COLLECT_MAP_LEN;
    memmove(msg.body, (char *)&type, msg.bodyLen);
    return Jt808::encode(buf, msg);
}

//slots function
void CmdDealClass::heartSlots()
{
    char arr[1024]={0};
    int arrLen;



    arrLen = h2gHeartDeal(arr);
    LOG_DEBUG("cmd send heart");
    udpSendMsg(arr, arrLen);
}

void CmdDealClass::statuSlots()
{
    static qint8 tick4clean=20;
    if(tick4clean>10)//延时10s等UI起来
    {
        tick4clean--;
        return;
    }
    qint64 curTime = QDateTime::currentMSecsSinceEpoch();
    if((curTime-linkLastTime) > HEART_TIMER_TIMEOUT)
    {
        LOG_DEBUG("statu link off");
        if(tick4clean==0)
        {
            //清除pos信息，界面恢复原始状态
            PosInfo* pos = PosInfo::getHandle();
            struct pubVehiclePosST info;
            std::memset(&info,0,sizeof(struct pubVehiclePosST));
            pos->setPosInfo(info);
            emit gatewayOfflineSignal(true);            
            tick4clean=5;
            //qDebug()<<"网关离线。。。";
        }else if(tick4clean>0)
            tick4clean--;
    }else
    {
        emit gatewayOfflineSignal(false);
        //qDebug()<<"网关在线";
        tick4clean=5;
    }
}
void CmdDealClass::h2gClearVehicleError(pubClearVehicleErrorST err)
{
    char buf[1024];
    uint32_t len;
    jt808MsgSt msg;
    msg.id = G2H_CLEAR_VEHICLE_ERROR_CMD;
    msg.bodyLen = G2H_CLEAR_VEHICLE_ERROR_LEN;
    memmove(msg.body, &err, msg.bodyLen);
    len = Jt808::encode(buf, msg);
    udpSendMsg(buf, len);
    addReSendMsg(G2H_CLEAR_VEHICLE_ERROR_CMD, QByteArray(buf, len));
}
void  CmdDealClass::h2gSetWorkStatuSlots(pubVehicleWorkStatuST statu)
{
    char arr[1024];
    int arrLen;

    arrLen = h2gSetWorkStatuDeal(arr, statu);
    udpSendMsg(arr, arrLen);
    addReSendMsg(H2G_SET_WORK_STATU_CMD, QByteArray(arr, arrLen));
}

void  CmdDealClass::h2gUpFileSlots(fileUpST file)
{
    char arr[1024];
    int arrLen;

    arrLen = h2gUpFileDeal(arr, file);
    udpSendMsg(arr, arrLen);
    addReSendMsg(H2G_UP_FILE_CMD, QByteArray(arr, arrLen));
}

void  CmdDealClass::h2gShutDownSlots()
{
    char arr[1024];
    int arrLen;

    arrLen = h2gShutDownDeal(arr);
    udpSendMsg(arr, arrLen);
    addReSendMsg(H2G_SHUT_DOWN_CMD, QByteArray(arr, arrLen));
}

void  CmdDealClass::h2gSetLoadSlots(char load)
{
    char arr[1024];
    int arrLen;

    arrLen = h2gSetLoadDeal(arr, load);
    udpSendMsg(arr, arrLen);
    addReSendMsg(H2G_SET_LOAD_CMD, QByteArray(arr, arrLen));
}

void  CmdDealClass::h2gResWorkOptSlots( pubVehicleResWorkOptST opt)
{
    char arr[1024];
    int arrLen;

    arrLen = h2gResWorkOptDeal(arr, opt);
    udpSendMsg(arr, arrLen);
    addReSendMsg(H2G_RES_WORK_OPT_CMD, QByteArray(arr, arrLen));
}

void  CmdDealClass::h2gReqWorkOptSlots(struct pubVehicleReqWorkOptST opt)
{
    char arr[1024];
    int arrLen;

    arrLen = h2gReqWorkOptDeal(arr, opt);
    udpSendMsg(arr, arrLen);
    addReSendMsg(H2G_REQ_WORK_OPT_CMD, QByteArray(arr, arrLen));
}

void  CmdDealClass::h2gReqMaterSlots(short mater)
{
    char arr[1024];
    int arrLen;

    arrLen = h2gReqMaterDeal(arr, mater);
    udpSendMsg(arr, arrLen);
    addReSendMsg(H2G_REQ_MATER_CMD, QByteArray(arr, arrLen));
}

void  CmdDealClass::h2gCollectMapDealSlots(char type)
{
    char arr[1024];
    int arrLen;

    arrLen = h2gCollectMapDeal(arr, type);
    udpSendMsg(arr, arrLen);
    addReSendMsg(H2G_COLLECT_MAP_CMD, QByteArray(arr, arrLen));
}

void CmdDealClass::h2gSendHmiVerSlots(struct versionST version)
{
    char arr[1024];
    int arrLen;
    jt808MsgSt msg;
    msg.id = H2G_VERSION_CMD;
    msg.bodyLen = H2G_VERSION_LEN;
    memmove(msg.body, &version, msg.bodyLen);
    arrLen = Jt808::encode(arr, msg);

    udpSendMsg(arr, arrLen);
    addReSendMsg(H2G_VERSION_CMD, QByteArray(arr, arrLen));
}
//功能函数
/**
 * @brief CmdDealClass::warnninOpt 功能报警
 * @param buf 报警位地址
 * @param len 报警数据长度
 */
void  CmdDealClass::warnninOpt(char *buf, int len)
{
    warnning warn;
    warn.loadFromRawData(buf,len);
    auto warnBitVector = warn.getWarningBit();

    std::vector<size_t> addWarn,removeWarn;

    std::set_difference(warnBitVector.begin(), warnBitVector.end(),
                        warnList.begin(), warnList.end(),
                        back_inserter(addWarn));

    std::set_difference(warnList.begin(), warnList.end(),
                    warnBitVector.begin(), warnBitVector.end(),
                        back_inserter(removeWarn));

    warnList = warnBitVector;

    for(auto &warnBit : addWarn)
    {
        LOG_DEBUG("insert warnning %d\n",warnBit);
        g2hInsertWarnningSignal(warnBit);
    }

    for(auto &warnBit : removeWarn)
    {
        LOG_DEBUG("remove warnning %d\n", warnBit);
        g2hRemoveWarnningSignal(warnBit);
    }

//    for(int i=0; i<len; i++) {
//        for(int j=0; j<8; j++) {
//            if(buf[i]&(0x01<<j)) {
//                if(warnInMap.contains(i*8+j) == false) {
//                    warnInMap.insert(i*8+j, warnningStr[i*8+j]);
//                    LOG_DEBUG("insert warnning %d\n", i*8+j);
//                    g2hInsertWarnningSignal(i*8+j);
//                }
//            } else {
//                if(warnInMap.contains(i*8+j) == true) {
//                    warnInMap.remove(i*8+j);
//                    LOG_DEBUG("remove warnning %d\n", i*8+j);
//                    g2hRemoveWarnningSignal(i*8+j);
//                }
//            }
//        }
//    }
}







