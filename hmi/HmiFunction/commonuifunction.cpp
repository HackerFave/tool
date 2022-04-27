#include "commonuifunction.h"
commonUIFunction* commonUIFunction::handle = NULL;
QMutex commonUIFunction::commonMutex;
commonUIFunction::commonUIFunction()
{
//    connect(this,SIGNAL(h2gSendHmiVerSignal(versionST)),this->hmicmd,SLOT(h2gSendHmiVerSlots(versionST)));


    //QTimer::singleShot(4000, this, SLOT(h2gSendHmiVerCMD()));
}


commonUIFunction *commonUIFunction::instance()
{
    if(handle == NULL) {
        QMutexLocker locker(&commonMutex);
        if(handle == NULL) {
            handle = new commonUIFunction();
        }
    }
    return handle;
}


void commonUIFunction::g2hVersionSlot(__IO QTableWidget *editionTable,__I struct versionST versiondataSt,__O union versionUN *versiondata) //版本号
{
    uint8_t verArray[4];
    memmove((char*)versiondata, (char*)&versiondataSt, sizeof(struct versionST));

    //更新版本号显示

    if(versiondata->data.versionflag & 0x04)//ICU感知版本
    {
        memmove(verArray,&versiondata->data.icu,4);
        editionTable->item(0,1)->setText(QString("V%1.").arg(QString::number(versiondata->data.termialType, 16).toUpper(), 2, QLatin1Char('0')).right(4)+\
                                            QString("%1.").arg(QString::number(versiondata->data.vehicleType, 16).toUpper(), 2, QLatin1Char('0')).right(3) + \
                                            QString("03.") +\

                                            QString("%1.").arg(QString::number(verArray[2], 16).toUpper(), 2, QLatin1Char('0')).right(3) + \
                                            QString("%1.").arg(QString::number(verArray[1], 16).toUpper(), 2, QLatin1Char('0')).right(3) + \
                                            QString("%1").arg(QString::number(verArray[0], 16).toUpper(), 2, QLatin1Char('0')).right(2));
    }

    if(versiondata->data.versionflag & 0x01)//CCU主控版本
    {
        memmove(verArray,&versiondata->data.ccu,4);
        editionTable->item(1,1)->setText(QString("V%1.").arg(QString::number(versiondata->data.termialType, 16).toUpper(), 2, QLatin1Char('0')).right(4)+\
                                            QString("%1.").arg(QString::number(versiondata->data.vehicleType, 16).toUpper(), 2, QLatin1Char('0')).right(3) + \
                                            QString("01.") +\

                                         QString("%1.").arg(QString::number(verArray[2], 16).toUpper(), 2, QLatin1Char('0')).right(3) + \
                                         QString("%1.").arg(QString::number(verArray[1], 16).toUpper(), 2, QLatin1Char('0')).right(3) + \
                                         QString("%1").arg(QString::number(verArray[0], 16).toUpper(), 2, QLatin1Char('0')).right(2));
    }
    if(versiondata->data.versionflag & 0x08)//VCU线控版本
    {
        memmove(verArray,&versiondata->data.vcu,4);
        editionTable->item(2,1)->setText(QString("V%1.").arg(QString::number(versiondata->data.termialType, 16).toUpper(), 2, QLatin1Char('0')).right(4)+\
                                            QString("%1.").arg(QString::number(versiondata->data.vehicleType, 16).toUpper(), 2, QLatin1Char('0')).right(3) + \
                                            QString("05.") +\

                                         QString("%1.").arg(QString::number(verArray[2], 16).toUpper(), 2, QLatin1Char('0')).right(3) + \
                                         QString("%1.").arg(QString::number(verArray[1], 16).toUpper(), 2, QLatin1Char('0')).right(3) + \
                                         QString("%1").arg(QString::number(verArray[0], 16).toUpper(), 2, QLatin1Char('0')).right(2));
    }

    if(versiondata->data.versionflag & 0x20)//CGU网关版本
    {
        memmove(verArray,&versiondata->data.cgu,4);
        editionTable->item(3,1)->setText(QString("V%1.").arg(QString::number(versiondata->data.termialType, 16).toUpper(), 2, QLatin1Char('0')).right(4)+\
                                            QString("%1.").arg(QString::number(versiondata->data.vehicleType, 16).toUpper(), 2, QLatin1Char('0')).right(3) + \
                                            QString("04.") +\

                                         QString("%1.").arg(QString::number(verArray[2], 16).toUpper(), 2, QLatin1Char('0')).right(3) + \
                                         QString("%1.").arg(QString::number(verArray[1], 16).toUpper(), 2, QLatin1Char('0')).right(3) + \
                                         QString("%1").arg(QString::number(verArray[0], 16).toUpper(), 2, QLatin1Char('0')).right(2));
    }

    if(versiondata->data.versionflag & 0x10)//HMI版本
    {
        memmove(verArray,&versiondata->data.hmi,4);
        editionTable->item(4,1)->setText(QString("V%1.").arg(QString::number(versiondata->data.termialType, 16).toUpper(), 2, QLatin1Char('0')).right(4)+\
                                            QString("%1.").arg(QString::number(versiondata->data.vehicleType, 16).toUpper(), 2, QLatin1Char('0')).right(3) + \
                                            QString("06.") +\

                                         QString("%1.").arg(QString::number(verArray[2], 16).toUpper(), 2, QLatin1Char('0')).right(3) + \
                                         QString("%1.").arg(QString::number(verArray[1], 16).toUpper(), 2, QLatin1Char('0')).right(3) + \
                                         QString("%1").arg(QString::number(verArray[0], 16).toUpper(), 2, QLatin1Char('0')).right(2));
    }

}

void commonUIFunction::g2hReqTaskSlot(__IO QTextBrowser *messageBrowser, __I pubVehicleReqTaskST reqTask)
{
    //头部状态更新、信息窗口显示请求任务
    QString str;
    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));

    str.append("正在申请");
    switch(reqTask.taskType)
    {
        case 0x01:
        {
            str.append("装载任务");
            break;
        }
        case 0x02:
        {
            str.append("卸载任务");
            break;
        }
        case 0x03:
        {
            str.append("收车任务");
            break;
        }
        case 0x04:
        {
            str.append("电铲移动任务");
            break;
        }
        default:
        {
            str.append("未知任务");
            break;
        }
    }

    messageShow(messageBrowser,str);
}

void commonUIFunction::g2hGatewayOffline(QTextBrowser *messageBrowser, bool status)
{
    QString str;
    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));
    static bool beforeStatus=0;
    if(beforeStatus!=status)
    {
        beforeStatus=status;
        if(status)
        {
            str+="网关已离线";
        }else
        {
            str+="网关已上线";
            h2gSendHmiVerCMD();
        }
        messageShow(messageBrowser,str);
    }
}

void commonUIFunction::h2gSendHmiVerCMD()
{
    struct versionST hmiVersion;
    uint8_t verArray[4]={0};
    memset(&hmiVersion,0,sizeof(hmiVersion));
    hmiVersion.versionflag=1<<4;
    hmiVersion.vehicleType=ParaParsing::instance()->paraSt.pub.vehicleType;
    hmiVersion.vehicleNum=ParaParsing::instance()->paraSt.pub.vehicleType;
    verArray[0]=DEV_SUB_VERSION;//ParaParsing::instance()->paraSt.pub.subVer;
    verArray[1]=DEV_MAIN_VERSION;//ParaParsing::instance()->paraSt.pub.mainVer;
    verArray[2]=DEV_COMPAT_VERSION;//ParaParsing::instance()->paraSt.pub.compatibleVer;
    memmove(&hmiVersion.hmi,verArray,4);
    //imei
    //车辆型号

    emit h2gSendHmiVerSignal(hmiVersion);
}

void commonUIFunction::setHmiCMD(__I CmdDealClass *hmicmd)
{
    this->hmicmd=hmicmd;
}

void commonUIFunction::g2hTemporaryTaskSlot(__IO QTextBrowser *messageBrowser, __I char type)
{
    //信息窗口显示收到平台下发临时调度作业
    QString str;
    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));
    if(type == 0x01)
    {
        str.append("收到平台临时任务变更->装载任务");
    }
    else if( type == 0x02)
    {
        str.append("收到平台临时任务变更->卸载任务");
    }
    else if( type == 0x03)
    {
        str.append("收到平台临时任务变更->收车任务");
    }
    else if( type == 0x04)
    {
        str.append("收到平台临时任务变更->电铲移动任务");
    }
    else
    {
        str.append(QString("收到平台临时任务变更->未知任务:%1").arg(QString::number(type)));
    }

    messageShow(messageBrowser,str);
}

void commonUIFunction::g2hResTaskSlot(__IO QTextBrowser *messageBrowser,
                                      __IO QLabel *titleDestLabel,
                                      __I pubVehicleResTaskST resTask,
                                      __O struct pubVehicleResTaskST &taskInfo)
{
    //信息窗口显示接受平台响应任务
    QString str;
    QString strTaskType;

    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));

    str.append("申请");

    if(resTask.taskType == 0x01)
    {
        str.append("装载任务");
        strTaskType.append("装载");
    }
    else if(resTask.taskType == 0x02)
    {
        str.append("卸载任务");
        strTaskType.append("卸载");
    }
    else if(resTask.taskType == 0x03)
    {
        str.append("收车任务");
        strTaskType.append("收车");
    }
    else if(resTask.taskType == 0x04)
    {
        str.append("电铲移动任务");
        strTaskType.append("电铲移动");
    }

    if(resTask.result == 0x01)
    {
        str.append("成功");
        str.append(QString(",任务文件:") + QString(resTask.taskFile));

        taskInfo = resTask;
        titleDestLabel->setText(QString("目的地:") + QString(resTask.remotename) + QString("-") + strTaskType);
    }
    else if(resTask.result == 0x02)
    {
        str.append("失败-调度目标规划");
        titleDestLabel->setText("");
    }
    else if(resTask.result == 0x03)
    {
        str.append("失败-目标路径规划");
        titleDestLabel->setText("");
    }

    messageShow(messageBrowser,str);
}
void commonUIFunction::g2hDownPermissionFileSlot(bool flag, QStringList permissionList,quint32 endPointNum,
                                                 QTextBrowser *messageBrowser,
                                                 HmiMap *map)
{
    QString str;
    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));

    if(flag)
    {
        str.append("获取行车许可成功");
        str.append(QString("%1%2").arg(",许可文件:").arg(permissionList.at(0).split("/").last()));


        if(!permissionList.isEmpty())
        {
            map->addPermissionInfo(permissionList,endPointNum);
        }
    }
    else
    {
        str.append("获取行车许可失败");
    }

    messageShow(messageBrowser,str);
}

void commonUIFunction::messageShow(QTextBrowser *messageBrowser, const QString &str)
{
    messageBrowser->insertPlainText(str+"\r\n");
    messageBrowser->moveCursor(QTextCursor::Start,QTextCursor::MoveAnchor);
}
void commonUIFunction::g2hResWorkOnStatuSlot(QTextBrowser *messageBrowser,struct pubVehicleResWorkStatuST cloudReq) //收到平台运营状态变更确认
{
//在信息窗口显示运营状态变更确认
    QString str;

    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));
    str.append("申请");
    switch (cloudReq.id)
    {
        case 0x0f01:
        {
            str.append("运营状态变更");
            break;
        }
        default:
        {
            str.append("未知命令");
            break;
        }
    }

    if(cloudReq.ret == 0x00)
    {
        str.append("成功");
    }
    else
    {
        str.append("失败");
    }

    messageShow(messageBrowser,str);
}
void commonUIFunction::on_brightAdjustBtn_clicked(QStackedWidget *controlStackedWidget,QWidget *controlBrightPage)
{
    //按键标签提示：调节亮度->息屏
#if 0
    controlStackedWidget->setCurrentWidget(controlBrightPage);
#else
    hal_set_lcd_status(1);
    QWidget *screenProtect=new ScreenSaver;
    if(screenProtect)
    {
        screenProtect->show();
        screenProtect->showFullScreen();
    }
#endif
}

void commonUIFunction::g2hResWorkOptSlot(QTextBrowser *messageBrowser,struct pubVehicleResWorkOptST opt) //辅助车辆（网关已收到矿卡现场作业），矿卡（接收到辅助车辆相应作业信息）
{
    QString str;
    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));

    str.append("[");
    str.append(QString::fromUtf8(QByteArray(opt.localname, 20)));
    str.append("]");
    str.append("向");
    str.append("[");
    str.append(QString::fromUtf8(QByteArray(opt.remotename, 20)));
    str.append("]");

    if(opt.cmdType == PUB_WORK_OPT_IN_TASK)
    {
        str.append(QString("发送可驶入"));
    }
    else if(opt.cmdType == PUB_WORK_OPT_OUT_TASK)
    {
        str.append(QString("发送可驶离"));
    }
    else if(opt.cmdType == PUB_WORK_OPT_FINISH_TASK)
    {
        str.append(QString("发送停靠完成"));
    }
    else if(opt.cmdType == PUB_WORK_OPT_STOP_TASK)
    {
        str.append(QString("发送紧急停车"));
    }
    else if(opt.cmdType == PUB_WORK_OPT_RECOVER_TASK)
    {
        str.append(QString("发送恢复行车"));
    }
    else if(opt.cmdType == PUB_WORK_OPT_MAN_TASK)
    {
        str.append(QString("发送人工接管"));
    }
    else if(opt.cmdType == PUB_WORK_OPT_RE_IN_TASK)
    {
        str.append(QString("发送退出重新申请"));
    }
    else if(opt.cmdType == PUB_WORK_OPT_UNLOAD_TASK)
    {
        str.append(QString("发送允许卸料"));
    }
    else if(opt.cmdType == PUB_WORK_OPT_CLEAR_TASK)
    {
        str.append(QString("发送退出排队"));
    }
    else
    {
        str.append(QString("发送未知%1").arg(QString::number(opt.cmdType)));
    }
    str.append(QString("%1,%2,%3").arg(opt.lat, 0, 'f', 7).arg(opt.lng, 0, 'f', 7).arg(opt.heading));

    messageShow(messageBrowser,str);
}
void commonUIFunction::g2hReqWorkRealOptSlot(QTextBrowser *messageBrowser,struct pubVehicleReqWorkOptST reqWork) //网关请求现场作业
{
    QString str;
    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));

    str.append("[");
    str.append(QString::fromUtf8(QByteArray(reqWork.localname, 20)));
    str.append("]");
    str.append("向");
    str.append("[");
    str.append(QString::fromUtf8(QByteArray(reqWork.remotename, 20)));
    str.append("]");

    if(reqWork.cmdType == PUB_WORK_OPT_IN_TASK)
    {
        str.append(QString("发送正在驶入"));

        WorkListDeal* worklist = WorkListDeal::getHandle();
        struct pubVehicleDevNameStateST remoteVehicle;
        memmove(remoteVehicle.localimei, reqWork.localimei, 15);
        memmove(remoteVehicle.localname, reqWork.localname, 20);
        remoteVehicle.workState = PUB_WORK_OPT_RUN;
        worklist->insertInList(remoteVehicle);
    }
    else if(reqWork.cmdType == PUB_WORK_OPT_OUT_TASK)
    {
        str.append(QString("发送正在驶离"));

        WorkListDeal* worklist = WorkListDeal::getHandle();
        struct pubVehicleDevNameStateST remoteVehicle;
        memmove(remoteVehicle.localimei, reqWork.localimei, 15);
        memmove(remoteVehicle.localname, reqWork.localname, 20);
        worklist->removeInList(remoteVehicle);
    }
    else if(reqWork.cmdType == PUB_WORK_OPT_FINISH_TASK)
    {
        str.append(QString("发送已停靠完成"));

        WorkListDeal* worklist = WorkListDeal::getHandle();
        struct pubVehicleDevNameStateST remoteVehicle;
        memmove(remoteVehicle.localimei, reqWork.localimei, 15);
        memmove(remoteVehicle.localname, reqWork.localname, 20);

        remoteVehicle.workState = PUB_WORK_OPT_FINISH_TASK;
        worklist->insertInList(remoteVehicle);//电铲是remove
    }
    else if(reqWork.cmdType == PUB_WORK_OPT_UNLOAD_TASK)//电铲没有这个
    {
        str.append(QString("发送正在卸料"));

        WorkListDeal* worklist = WorkListDeal::getHandle();
        struct pubVehicleDevNameStateST remoteVehicle;
        memmove(remoteVehicle.localimei, reqWork.localimei, 15);
        memmove(remoteVehicle.localname, reqWork.localname, 20);
        remoteVehicle.workState = PUB_WORK_OPT_UNLOAD_TASK;
        worklist->insertInList(remoteVehicle);
    }
    else if(reqWork.cmdType == PUB_WORK_OPT_CLEAR_TASK)
    {
        str.append(QString("发送退出排队"));

        WorkListDeal* worklist = WorkListDeal::getHandle();
        struct pubVehicleDevNameStateST remoteVehicle;
        memmove(remoteVehicle.localimei, reqWork.localimei, 15);
        memmove(remoteVehicle.localname, reqWork.localname, 20);
        remoteVehicle.workState = PUB_WORK_OPT_CLEAR_TASK;
        worklist->removeInList(remoteVehicle);
    }
    else if(reqWork.cmdType == PUB_WORK_OPT_STOP_TASK)
    {
        str.append(QString("发送紧急停车"));

        WorkListDeal* worklist = WorkListDeal::getHandle();
        struct pubVehicleDevNameStateST remoteVehicle;
        memmove(remoteVehicle.localimei, reqWork.localimei, 15);
        memmove(remoteVehicle.localname, reqWork.localname, 20);
        remoteVehicle.workState = PUB_WORK_OPT_STOP_TASK;
        worklist->insertInList(remoteVehicle);

    }
    else if(reqWork.cmdType == PUB_WORK_OPT_RECOVER_TASK)
    {
        str.append(QString("发送停车后恢复行驶"));
        WorkListDeal* worklist = WorkListDeal::getHandle();
        struct pubVehicleDevNameStateST remoteVehicle;
        memmove(remoteVehicle.localimei, reqWork.localimei, 15);
        memmove(remoteVehicle.localname, reqWork.localname, 20);
        remoteVehicle.workState = PUB_WORK_OPT_RECOVER_TASK;
        worklist->insertInList(remoteVehicle);

    }
    else if(reqWork.cmdType == PUB_WORK_OPT_MAN_TASK)
    {
        str.append(QString("发送停车后人工接管"));
        WorkListDeal* worklist = WorkListDeal::getHandle();
        struct pubVehicleDevNameStateST remoteVehicle;
        memmove(remoteVehicle.localimei, reqWork.localimei, 15);
        memmove(remoteVehicle.localname, reqWork.localname, 20);
        remoteVehicle.workState = PUB_WORK_OPT_MAN_TASK;
        worklist->insertInList(remoteVehicle);

    }
    else if(reqWork.cmdType == PUB_WORK_OPT_RE_IN_TASK)
    {
        str.append(QString("发送停车后退出重进"));
        WorkListDeal* worklist = WorkListDeal::getHandle();
        struct pubVehicleDevNameStateST remoteVehicle;
        memmove(remoteVehicle.localimei, reqWork.localimei, 15);
        memmove(remoteVehicle.localname, reqWork.localname, 20);
        remoteVehicle.workState = PUB_WORK_OPT_RE_IN_TASK;
        worklist->insertInList(remoteVehicle);

    }
    else
    {
        str.append(QString("发送未知%1").arg(QString::number(reqWork.cmdType)));
    }

    messageShow(messageBrowser,str);
}
void commonUIFunction::g2hReqWorkOptSlot(QTextBrowser *messageBrowser,struct pubVehicleReqWorkOptST reqWork) //网关请求现场作业
{
    QString str;
    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));

    str.append("[");
    str.append(QString::fromUtf8(QByteArray(reqWork.localname, 20)));
    str.append("]");
    str.append("向");
    str.append("[");
    str.append(QString::fromUtf8(QByteArray(reqWork.remotename, 20)));
    str.append("]");

    if(reqWork.cmdType == PUB_WORK_OPT_IN_TASK)
    {
        str.append(QString("发送申请驶入"));

        WorkListDeal* worklist = WorkListDeal::getHandle();
        struct pubVehicleDevNameStateST remoteVehicle;
        memmove(remoteVehicle.localimei, reqWork.localimei, 15);
        memmove(remoteVehicle.localname, reqWork.localname, 20);
        remoteVehicle.workState = PUB_WORK_OPT_IN_TASK;

        worklist->insertInList(remoteVehicle);
    }
    else if(reqWork.cmdType == PUB_WORK_OPT_OUT_TASK)
    {
        str.append(QString("发送正在驶离"));

        WorkListDeal* worklist = WorkListDeal::getHandle();
        struct pubVehicleDevNameStateST remoteVehicle;
        memmove(remoteVehicle.localimei, reqWork.localimei, 15);
        memmove(remoteVehicle.localname, reqWork.localname, 20);
        remoteVehicle.workState = PUB_WORK_OPT_OUT_TASK;
        worklist->removeInList(remoteVehicle);
    }
    else if(reqWork.cmdType == PUB_WORK_OPT_FINISH_TASK)
    {
        str.append(QString("发送已停靠完成"));

        WorkListDeal* worklist = WorkListDeal::getHandle();
        struct pubVehicleDevNameStateST remoteVehicle;
        memmove(remoteVehicle.localimei, reqWork.localimei, 15);
        memmove(remoteVehicle.localname, reqWork.localname, 20);
        remoteVehicle.workState = PUB_WORK_OPT_FINISH_TASK;

        if(ParaParsing::instance()->paraSt.pub.vehicleType==8)
            worklist->insertInList(remoteVehicle);
        else if(ParaParsing::instance()->paraSt.pub.vehicleType==7)
            worklist->removeInList(remoteVehicle);
        else
            worklist->insertInList(remoteVehicle);
    }
    else if(reqWork.cmdType == PUB_WORK_OPT_UNLOAD_TASK)
    {
        str.append(QString("发送申请卸料"));

        WorkListDeal* worklist = WorkListDeal::getHandle();
        struct pubVehicleDevNameStateST remoteVehicle;
        memmove(remoteVehicle.localimei, reqWork.localimei, 15);
        memmove(remoteVehicle.localname, reqWork.localname, 20);
        remoteVehicle.workState = PUB_WORK_OPT_UNLOAD_TASK;
        worklist->insertInList(remoteVehicle);
    }
    else if(reqWork.cmdType == PUB_WORK_OPT_CLEAR_TASK)
    {
        str.append(QString("发送已退出排队"));

        WorkListDeal* worklist = WorkListDeal::getHandle();
        struct pubVehicleDevNameStateST remoteVehicle;
        memmove(remoteVehicle.localimei, reqWork.localimei, 15);
        memmove(remoteVehicle.localname, reqWork.localname, 20);
        worklist->removeInList(remoteVehicle);
    }
    else if(reqWork.cmdType == PUB_WORK_OPT_STOP_TASK)
    {
        str.append(QString("紧急停车"));

        WorkListDeal* worklist = WorkListDeal::getHandle();
        struct pubVehicleDevNameStateST remoteVehicle;
        memmove(remoteVehicle.localimei, reqWork.localimei, 15);
        memmove(remoteVehicle.localname, reqWork.localname, 20);
        remoteVehicle.workState = PUB_WORK_OPT_STOP_TASK;
        worklist->insertInList(remoteVehicle);

    }
    else if(reqWork.cmdType == PUB_WORK_OPT_RECOVER_TASK)
    {
        str.append(QString("停车后恢复行驶"));
        WorkListDeal* worklist = WorkListDeal::getHandle();
        struct pubVehicleDevNameStateST remoteVehicle;
        memmove(remoteVehicle.localimei, reqWork.localimei, 15);
        memmove(remoteVehicle.localname, reqWork.localname, 20);
        remoteVehicle.workState = PUB_WORK_OPT_RECOVER_TASK;
        worklist->insertInList(remoteVehicle);

    }
    else if(reqWork.cmdType == PUB_WORK_OPT_MAN_TASK)
    {
        str.append(QString("停车后人工接管"));
        WorkListDeal* worklist = WorkListDeal::getHandle();
        struct pubVehicleDevNameStateST remoteVehicle;
        memmove(remoteVehicle.localimei, reqWork.localimei, 15);
        memmove(remoteVehicle.localname, reqWork.localname, 20);
        remoteVehicle.workState = PUB_WORK_OPT_MAN_TASK;
        worklist->insertInList(remoteVehicle);

    }
    else if(reqWork.cmdType == PUB_WORK_OPT_RE_IN_TASK)
    {
        str.append(QString("停车后退出重进"));
        WorkListDeal* worklist = WorkListDeal::getHandle();
        struct pubVehicleDevNameStateST remoteVehicle;
        memmove(remoteVehicle.localimei, reqWork.localimei, 15);
        memmove(remoteVehicle.localname, reqWork.localname, 20);
        remoteVehicle.workState = PUB_WORK_OPT_RE_IN_TASK;
        worklist->insertInList(remoteVehicle);

    }
    else
    {
        str.append(QString("操作未知:%1").arg(QString::number(reqWork.cmdType)));
    }

    messageShow(messageBrowser,str);

}
