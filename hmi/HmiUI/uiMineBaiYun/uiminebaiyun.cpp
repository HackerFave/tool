#include "uiminebaiyun.h"
#include "ui_uiminebaiyun.h"

uiMineBaiyun::uiMineBaiyun(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::uiMineBaiyun)
{
    ui->setupUi(this);
    preWarnCentralWorkStatePage=NULL;
    preWarnControlStackedWidget=NULL;

    map = new HmiMap;
    QGridLayout *mapGrid = new QGridLayout;
    mapGrid->addWidget(map);
    ui->centralMapViewPage->setLayout(mapGrid);

    delayChoice = new DelayChoice;
    QGridLayout *delayChoiceGrid = new QGridLayout;
    delayChoiceGrid->addWidget(delayChoice);
    ui->centralDelayPage->setLayout(delayChoiceGrid);

    keyBoard = new KeyBoard;
    QGridLayout *keyBoardGrid = new QGridLayout;
    keyBoardGrid->addWidget(keyBoard);
    ui->centralLoginOutPage->setLayout(keyBoardGrid);

    workChoice = new WorkStateChoice;
    QGridLayout *workChoiceGrid = new QGridLayout;
    workChoiceGrid->addWidget(workChoice);
    ui->centralWorkStatePage->setLayout(workChoiceGrid);

    faultMineChoice = new FaultMineChoice;
    QGridLayout *faultMineChoiceGrid = new QGridLayout;
    faultMineChoiceGrid->addWidget(faultMineChoice);
    ui->centralFaultPage->setLayout(faultMineChoiceGrid);

    hmicmd = new CmdDealClass;
    ParaParsing* para = ParaParsing::instance();
    hmicmd->udpInit(para->paraSt.pub.localIp, para->paraSt.pub.localPort, para->paraSt.pub.remoteIp, para->paraSt.pub.remotePort);
//new add 没有效果取消,加了后signal传递会有问题，不同线程间传递信号，避免方法后续再说
//    QThread *hmicmd_thread = new QThread;
//    hmicmd->moveToThread(hmicmd_thread);
//    hmicmd_thread->start();
//new add end
    memset((char*)&localImeiName, 0, sizeof(localImeiName));

    titleTimer = new QTimer;
    titleTimer->setInterval(1000);
    titleTimer->start();

    messageEndTimer = new QTimer;
    blinkTimer = new QTimer;
    blinkTimer->setInterval(500);

    speedBlinkTimer = new QTimer;
    speedBlinkTimer->setInterval(250);

    collectionTimer = new QTimer;
    collectionTimer->setInterval(50);

    aroundVehicleUpdateTimer = new QTimer;
    aroundVehicleUpdateTimer->setInterval(5000);
    aroundVehicleUpdateTimer->start();

    ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);

    ui->runDistLabel->hide();
    ui->reversDistLabel->hide();

    ui->editionTable->setItem(0, 1 , new QTableWidgetItem);
    ui->editionTable->setItem(1, 1 , new QTableWidgetItem);
    ui->editionTable->setItem(2, 1 , new QTableWidgetItem);
    ui->editionTable->setItem(3, 1 , new QTableWidgetItem);
    ui->editionTable->setItem(4, 1 , new QTableWidgetItem);
    ui->editionTable->setItem(5, 1 , new QTableWidgetItem);
    ui->editionTable->item(5,1)->setText(QString("V%1.%2.%3").arg(QString::number(DEV_COMPAT_VERSION, 10), 2, QLatin1Char('0')).arg(QString::number(DEV_MAIN_VERSION, 10), 2, QLatin1Char('0'))
                                         .arg(QString::number(DEV_SUB_VERSION, 10), 2, QLatin1Char('0')));

    ui->curSpeedLabel->setStyleSheet("color:white;\
                                      font:18pt, \"Microsoft YaHei\";");
    ui->speedUnitLabel->setStyleSheet("color:white;\
                                      font:10pt \"Microsoft YaHei\";");

    connect(this, SIGNAL(h2gSetWorkStatuSignal(pubVehicleWorkStatuST)), hmicmd, SLOT(h2gSetWorkStatuSlots(pubVehicleWorkStatuST))); //设置运用状态
    connect(this, SIGNAL(h2gUpFileSignal(fileUpST)), hmicmd, SLOT(h2gUpFileSlots(fileUpST))); //文件上传,fileDir不用部分为0
    connect(this, SIGNAL(h2gShutDownSignal()), hmicmd, SLOT(h2gShutDownSlots()));  //发送网关关机命令
    connect(this, SIGNAL(h2gReqWorkOptSignal(pubVehicleReqWorkOptST)), hmicmd, SLOT(h2gReqWorkOptSlots(pubVehicleReqWorkOptST))); //相应现场调度
    connect(this, SIGNAL(h2gSetLoadSignal(char)), hmicmd, SLOT(h2gSetLoadSlots(char))); //设置物料类型
    connect(this, SIGNAL(h2gReqMaterSignal(short)), hmicmd, SLOT(h2gReqMaterSlots(short))); //设置物料类型
    connect(this, SIGNAL(h2gClearVehicleErrorSiganl(pubClearVehicleErrorST)),hmicmd,SLOT(h2gClearVehicleError(pubClearVehicleErrorST)));//清除主控错误

    connect(hmicmd, SIGNAL(g2hPosSignal(struct pubVehiclePosST)), this, SLOT(g2hPosSlot(struct pubVehiclePosST)));  //网关发送hmi位置信息
    connect(hmicmd, SIGNAL(g2hReqWorkOnStatuSignal(struct pubVehicleWorkStatuST)), this, SLOT(g2hReqWorkOnStatuSlot(struct pubVehicleWorkStatuST))); //平台发送网关运营状态变更
    connect(hmicmd, SIGNAL(g2hResWorkOnStatuSignal(struct pubVehicleResWorkStatuST)), this, SLOT(g2hResWorkOnStatuSlot(struct pubVehicleResWorkStatuST))); //收到平台运营状态变更确认
    connect(hmicmd, SIGNAL(g2hReqWorkOptSignal(struct pubVehicleReqWorkOptST)), this, SLOT(g2hReqWorkOptSlot(struct pubVehicleReqWorkOptST))); //网关请求现场作业
    connect(hmicmd, SIGNAL(g2hReqWorkRealOptSignal(struct pubVehicleReqWorkOptST)), this, SLOT(g2hReqWorkRealOptSlot(struct pubVehicleReqWorkOptST))); //网关请求现场作业
    connect(hmicmd, SIGNAL(g2hResWorkOptSignal(struct pubVehicleResWorkOptST)), this, SLOT(g2hResWorkOptSlot(struct pubVehicleResWorkOptST))); //辅助车辆（网关已收到矿卡现场作业），矿卡（接收到辅助车辆相应作业信息）
    connect(hmicmd, SIGNAL(g2hCollWarnningSignal(struct pubVehicleCollWarnningST)), this, SLOT(g2hCollWarnningSlot(struct pubVehicleCollWarnningST))); //预警
    connect(hmicmd, SIGNAL(g2hDownMapFileSignal(int, QString)), this, SLOT(g2hDownMapFileSlot(int, QString))); //接收网关下载地图命令
    connect(hmicmd, SIGNAL(g2hVersionSignal(struct versionST)), this, SLOT(g2hVersionSlot(struct versionST))); //版本号
    connect(hmicmd, SIGNAL(g2hDownTaskFileSignal(bool, QString,QStringList)), this, SLOT(g2hDownTaskFileSlot(bool, QString,QStringList))); //下载文件
    connect(hmicmd, SIGNAL(g2hStartUpSignal(struct startUpST)), this, SLOT(g2hStartUpSlot(struct startUpST))); //网关发送开机命令
    connect(hmicmd, SIGNAL(g2hShutDownAckSignal()), this, SLOT(g2hShutDownAckSlot())); //网关关机应答
    connect(hmicmd, SIGNAL(g2hDevNameSignal(struct pubVehicleDevNameST)), this, SLOT(g2hDevNameSlot(struct pubVehicleDevNameST))); //网关检权获取名称后发送给hmi
    connect(hmicmd, SIGNAL(g2hMatAckSignal(short)), this, SLOT(g2hMatAckSlot(short))); //物料变更
    connect(hmicmd, SIGNAL(g2hReqTaskSignal(struct pubVehicleReqTaskST )), this, SLOT(g2hReqTaskSlot(struct pubVehicleReqTaskST ))); //请求任务
    connect(hmicmd, SIGNAL(g2hResTaskSignal(struct pubVehicleResTaskST )), this, SLOT(g2hResTaskSlot(struct pubVehicleResTaskST ))); //接收平台响应任务
    connect(hmicmd, SIGNAL(g2hReqRoadSignal(struct pubVehicleReqRoadST )), this, SLOT(g2hReqRoadSlot(struct pubVehicleReqRoadST ))); //申请路权
    connect(hmicmd, SIGNAL(g2hResRoadSignal(struct pubVehicleResRoadST )), this, SLOT(g2hResRoadSlot(struct pubVehicleResRoadST ))); //平台回复路权申请
    connect(hmicmd, SIGNAL(g2hFreeRoadSignal(struct pubVehicleReqRoadST )), this, SLOT(g2hFreeRoadSlot(struct pubVehicleReqRoadST ))); //平台下发释放路权
    connect(hmicmd, SIGNAL(g2hTemporaryTaskSignal(char )), this, SLOT( g2hTemporaryTaskSlot(char ))); //平台下发临时调度作业
    connect(hmicmd, SIGNAL(g2hMasterWarnningSignal(struct pubVehicleMasterWarnningST)), this, SLOT( g2hMasterWarnningSlot(struct pubVehicleMasterWarnningST))); //主控发送预警
    connect(hmicmd, SIGNAL(g2hInsertWarnningSignal(int)), this, SLOT(insertWarnningSlots(int)));
    connect(hmicmd, SIGNAL(g2hRemoveWarnningSignal(int)), this, SLOT(removeWarnningSlots(int)));
    connect(hmicmd, SIGNAL(g2hDownPermissionFileSignal(bool,QStringList,quint32)), this, SLOT(g2hDownPermissionFileSlot(bool,QStringList,quint32))); //下载任务许可文件
    connect(hmicmd, SIGNAL(g2hDrivePermissionSignal(struct pubDrivePermissionListST )), this, SLOT(g2hResPermissionSlot(struct pubDrivePermissionListST ))); //接收平台响应任务
    connect(titleTimer, SIGNAL(timeout()), this, SLOT(titleTimerSlot()));
    connect(messageEndTimer, SIGNAL(timeout()), this, SLOT(messageEndTimerSlot()));
    connect(blinkTimer, SIGNAL(timeout()), this, SLOT(blinkTimerSlot()));
    connect(speedBlinkTimer, SIGNAL(timeout()), this, SLOT(speedBlinkTimerSlot()));
    connect(aroundVehicleUpdateTimer, SIGNAL(timeout()), this, SLOT(aroundVehicleUpdateTimerSlot()));
    connect(collectionTimer, SIGNAL(timeout()), this, SLOT(collectionTimerSlot()));

    connect(hmicmd,SIGNAL(gatewayOfflineSignal(bool)),this,SLOT(g2hGatewayOffline(bool)));
    connect(commonUIFunction::instance(),SIGNAL(h2gSendHmiVerSignal(versionST)),hmicmd,SLOT(h2gSendHmiVerSlots(versionST)));

    connect(this,SIGNAL(addDbInfoSignal(QString)),map,SLOT(addDbInfo(QString)));
    connect(this,SIGNAL(initViewSignal(QMatrix)),map,SLOT(initView(QMatrix)));
    connect(this,SIGNAL(curPosSignal(latlngheading)),map,SLOT(calcFinishedTask(latlngheading)));

    commonUIFunction::instance()->setHmiCMD(this->hmicmd);
    emit addDbInfoSignal(ParaParsing::instance()->paraSt.pub.mapPath+"/HDMap.db");
    //map->addDbInfo("/storage/mapinfo/HDMap.db");
    emit initViewSignal(QMatrix(0.25, 0, 0, 0.25, 0, 0));
}

uiMineBaiyun::~uiMineBaiyun()
{
    delete ui;
}


void uiMineBaiyun::on_workSetBtn_clicked()
{
    preWarnCentralWorkStatePage=NULL;
    preWarnControlStackedWidget=NULL;
    ui->controlStackedWidget->setCurrentWidget(ui->controlWorkSetPage);//去作业设置页面
    ui->centralStackedWidget->setCurrentWidget(ui->centralWorkStatePage);

    workChoice->clearChoice();
}

void uiMineBaiyun::on_stateSetBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlStateSetPage);//去状态设置页面
}

void uiMineBaiyun::on_loginOutBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlLoginOutPage);//去往登录设置页面
    ui->centralStackedWidget->setCurrentWidget(ui->centralLoginOutPage);//去往ID输入页面
}

void uiMineBaiyun::on_otherSetBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlOtherSetPage);//去往其他设置页面
}

void uiMineBaiyun::on_loginOutBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);//返回首界面
    ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);//返回主显示页面
}

void uiMineBaiyun::on_loginOutOkBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);//返回首界面
    ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);//返回主显示页面
}

void uiMineBaiyun::on_outWorkBackBtn_clicked()
{
    ui->centralStackedWidget->setCurrentWidget(ui->centralWorkStatePage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlWorkSetPage);//返回首界面
}

void uiMineBaiyun::on_stateSetBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);//返回首界面
}

//状态设置确认按下
void uiMineBaiyun::on_workSetOkBtn_clicked()
{
    if(workChoice->buttonGroup->checkedButton() != NULL)
    {
        switch(workChoice->buttonGroup->checkedId())
        {
            case 1://停靠完成
            {
                struct pubVehicleReqWorkOptST opt;
                memmove(opt.localimei, localImeiName.localimei, 15);
                memmove(opt.localname, localImeiName.localname, 20);
                memmove(opt.remoteimei, workStateInfo.remoteimei, 15);
                memmove(opt.remotename, workStateInfo.remotename,20);
                opt.taskType = workStateInfo.taskType;
                opt.cmdType = PUB_WORK_OPT_FINISH_TASK;
                opt.utcTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

                emit h2gReqWorkOptSignal(opt);
                break;
            }
            case 2://载货状态设置
            {
                ui->controlStackedWidget->setCurrentWidget(ui->controlLoadStatePage);
                ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
                break;
            }
            case 3://加油设置
            {
                ui->controlStackedWidget->setCurrentWidget(ui->controlAddOilCheckPage);
                ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
                break;
            }
            case 4://采集路麠设置
            {
                ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionPage);
                ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
                break;
            }
            case 5://退出排队设置
            {
                ui->controlStackedWidget->setCurrentWidget(ui->controlOutWorkCheckPage);//去往退出排队确认页面
                ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
                break;
            }
            case 6://静态测试
            {
                break;
            }
            case 7://动态测试
            {
                break;
            }
            case 8://清除故障
            {
                struct pubClearVehicleErrorST err;
                err.errorType=0x01;
                memset(err.errorFlag,0,16);
                emit h2gClearVehicleErrorSiganl(err);
                if(preWarnCentralWorkStatePage==NULL)
                    preWarnCentralWorkStatePage=ui->centralMapViewPage;
                if(preWarnControlStackedWidget==NULL)
                    preWarnControlStackedWidget=ui->controlMainPage;
                ui->centralStackedWidget->setCurrentWidget(preWarnCentralWorkStatePage);
                ui->controlStackedWidget->setCurrentWidget(preWarnControlStackedWidget);//返回首界面
                preWarnCentralWorkStatePage=NULL;
                preWarnControlStackedWidget=NULL;
                break;
            }
            default:
                break;
        }

        workChoice->clearChoice();
    }
}

//状态设置返回按下
void uiMineBaiyun::on_workSetBackBtn_clicked()
{
    if(preWarnControlStackedWidget!=NULL)
        return;
    ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);//返回首界面
}

void uiMineBaiyun::on_faultBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlStateSetPage);//返回故障页面
    ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);//返回主显示页面
}

void uiMineBaiyun::on_faultOkBtn_clicked()
{
    //获取错误号并发送
    if(faultMineChoice->buttonGroup->checkedButton())
    {
        pubVehicleWorkStatuST statu;

        statu.statuType = PUB_VEHICLE_FAULT_WORK_STATU;
        switch(faultMineChoice->buttonGroup->checkedId())
        {
            case 1:
            {
                statu.fault = PUB_FAULT_OTHER;
                break;
            }
            default:
            {
                break;
            }
        }
        emit h2gSetWorkStatuSignal(statu);

        ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);//返回主界面
        ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);//返回主显示页面
    }

}

//载货状态返回
void uiMineBaiyun::on_loadStateBackBtn_clicked()
{
    ui->centralStackedWidget->setCurrentWidget(ui->centralWorkStatePage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlWorkSetPage);//返回状态设置页面
}

//加油返回
void uiMineBaiyun::on_addOilBackBtn_clicked()
{
    ui->centralStackedWidget->setCurrentWidget(ui->centralWorkStatePage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlWorkSetPage);//返回状态设置页面
}

//加油确定
void uiMineBaiyun::on_addOilOkBtn_clicked()
{
    //发送加油信号
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);//返回主界面
}

void uiMineBaiyun::on_outWorkOkBtn_clicked()
{
    //发送退出排队信号
    struct pubVehicleReqWorkOptST reqWork;
    PosInfo* pos = PosInfo::getHandle();

    memmove(reqWork.localimei, localImeiName.localimei, 15);
    memmove(reqWork.remoteimei, taskInfo.remoteimei, 15);
    reqWork.taskType = taskInfo.taskType;
    reqWork.cmdType = PUB_WORK_OPT_CLEAR_TASK;
    reqWork.utcTime = pos->getUtcTime();
    memmove(reqWork.localname, localImeiName.localname, 20);
    memmove(reqWork.remotename, taskInfo.remotename, 20);

    emit h2gReqWorkOptSignal(reqWork);

    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);//返回主界面
}

void uiMineBaiyun::on_faultBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlFaultCheckPage);//去往故障确定页面
    ui->centralStackedWidget->setCurrentWidget(ui->centralFaultPage);
}

//空载按键按下
void uiMineBaiyun::on_emptyLoadBaiyunBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlEmptyLoadCheckPage);
}

//半载按键按下
void uiMineBaiyun::on_fullunLoadBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlFullunLoadCheckPage);
}

//异常处理按键按下
void uiMineBaiyun::on_loadDiffBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlLoadDiffCheckPage);
}


void uiMineBaiyun::on_halfunLoadBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlHalfunLoadCheckPage);
}

void uiMineBaiyun::on_emptyunLoadBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlEmptyunLoadCheckPage);
}

void uiMineBaiyun::on_emptyunLoadOkBtn_clicked()
{
    emit h2gSetLoadSignal(PUB_WORK_STATE_EMPTYUNLOAD_TYPE);

    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
}

void uiMineBaiyun::on_emptyunLoadBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlLoadDiffCheckPage);
}

void uiMineBaiyun::on_halfunLoadOkBtn_clicked()
{
    emit h2gSetLoadSignal(PUB_WORK_STATE_HALFUNLOAD_TYPE);

    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
}

void uiMineBaiyun::on_halfunLoadBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlLoadDiffCheckPage);
}


//就绪状态设置按键按下
void uiMineBaiyun::on_readyBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlReadyCheckPage);
}

//就绪状态设置确定按键按下
void uiMineBaiyun::on_readyOkBtn_clicked()
{
//发送就绪申请
    struct pubVehicleWorkStatuST statu;

    memset((char*)&statu, 0, sizeof(struct pubVehicleWorkStatuST));
    statu.statuType = PUB_VEHICLE_READY_WORK_STATU;

    emit h2gSetWorkStatuSignal(statu);

    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
}

//就绪申请页面返回按键按下
void uiMineBaiyun::on_readyBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlStateSetPage);//返回首界面
}

//跳转至延时页面
void uiMineBaiyun::on_delayBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlDelayPage);
    ui->centralStackedWidget->setCurrentWidget(ui->centralDelayPage);//去往故障选择页面

    delayChoice->clearChoice();
}

//延时10分钟按键按下
void uiMineBaiyun::on_delay10Btn_clicked()
{
//发送延时10分钟信号
    if(delayChoice->buttonGroup->checkedButton())
    {
        pubVehicleWorkStatuST statu;
        memset((char*)&statu, 0, sizeof(struct pubVehicleWorkStatuST));
        statu.statuType = PUB_VEHICLE_DEALY_WORK_STATU;

        switch(delayChoice->buttonGroup->checkedId())
        {
            case 1:
            {
                statu.fault = PUB_DELAY_EXCAVATOR_CHANGE;
                break;
            }
            case 2:
            {
                statu.fault = PUB_DELAY_EXCAVATOR_FOOD;
                break;
            }
            case 3:
            {
                statu.fault = PUB_DELAY_EXCAVATOR_PUSH;
                break;
            }
            case 4:
            {
                statu.fault = PUB_DELAY_EXCAVATOR_AVOIDBLAST;
                break;
            }
            case 5:
            {
                statu.fault = PUB_DELAY_EXCAVATOR_SWEEP;
                break;
            }
            case 6:
            {
                statu.fault = PUB_DELAY_EXCAVATOR_CARGO;
                break;
            }
            case 7:
            {
                statu.fault = PUB_DELAY_EXCAVATOR_DISPLACE;
                break;
            }
            case 8:
            {
                statu.fault = PUB_DELAY_EXCAVATOR_DIG;
                break;
            }
            case 9:
            {
                statu.fault = PUB_DELAY_EXCAVATOR_POWER;
                break;
            }
            default:
            {
                break;
            }
        }
        statu.delayTim = PUB_DELAY_TIME_10MIN;
        emit h2gSetWorkStatuSignal(statu);

        ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
        ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
    }
}

//延时30分钟按键按下
void uiMineBaiyun::on_delay30Btn_clicked()
{
//发送延时30分钟信号
    if(delayChoice->buttonGroup->checkedButton())
    {
        pubVehicleWorkStatuST statu;
        memset((char*)&statu, 0, sizeof(struct pubVehicleWorkStatuST));
        statu.statuType = PUB_VEHICLE_DEALY_WORK_STATU;

        switch(delayChoice->buttonGroup->checkedId())
        {
            case 1:
            {
                statu.fault = PUB_DELAY_EXCAVATOR_CHANGE;
                break;
            }
            case 2:
            {
                statu.fault = PUB_DELAY_EXCAVATOR_FOOD;
                break;
            }
            case 3:
            {
                statu.fault = PUB_DELAY_EXCAVATOR_PUSH;
                break;
            }
            case 4:
            {
                statu.fault = PUB_DELAY_EXCAVATOR_AVOIDBLAST;
                break;
            }
            case 5:
            {
                statu.fault = PUB_DELAY_EXCAVATOR_SWEEP;
                break;
            }
            case 6:
            {
                statu.fault = PUB_DELAY_EXCAVATOR_CARGO;
                break;
            }
            case 7:
            {
                statu.fault = PUB_DELAY_EXCAVATOR_DISPLACE;
                break;
            }
            case 8:
            {
                statu.fault = PUB_DELAY_EXCAVATOR_DIG;
                break;
            }
            case 9:
            {
                statu.fault = PUB_DELAY_EXCAVATOR_POWER;
                break;
            }
            default:
            {
                break;
            }
        }
        statu.delayTim = PUB_DELAY_TIME_30MIN;
        emit h2gSetWorkStatuSignal(statu);

        ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
        ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
    }
}

//延时60分钟按键按下
void uiMineBaiyun::on_delay60Btn_clicked()
{
//发送延时60分钟信号
    if(delayChoice->buttonGroup->checkedButton())
    {
        pubVehicleWorkStatuST statu;
        memset((char*)&statu, 0, sizeof(struct pubVehicleWorkStatuST));
        statu.statuType = PUB_VEHICLE_DEALY_WORK_STATU;

        switch(delayChoice->buttonGroup->checkedId())
        {
            case 1:
            {
                statu.fault = PUB_DELAY_EXCAVATOR_CHANGE;
                break;
            }
            case 2:
            {
                statu.fault = PUB_DELAY_EXCAVATOR_FOOD;
                break;
            }
            case 3:
            {
                statu.fault = PUB_DELAY_EXCAVATOR_PUSH;
                break;
            }
            case 4:
            {
                statu.fault = PUB_DELAY_EXCAVATOR_AVOIDBLAST;
                break;
            }
            case 5:
            {
                statu.fault = PUB_DELAY_EXCAVATOR_SWEEP;
                break;
            }
            case 6:
            {
                statu.fault = PUB_DELAY_EXCAVATOR_CARGO;
                break;
            }
            case 7:
            {
                statu.fault = PUB_DELAY_EXCAVATOR_DISPLACE;
                break;
            }
            case 8:
            {
                statu.fault = PUB_DELAY_EXCAVATOR_DIG;
                break;
            }
            case 9:
            {
                statu.fault = PUB_DELAY_EXCAVATOR_POWER;
                break;
            }
            default:
            {
                break;
            }
        }
        statu.delayTim = PUB_DELAY_TIME_60MIN;
        emit h2gSetWorkStatuSignal(statu);

        ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
        ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
    }
}

//延时页面返回
void uiMineBaiyun::on_delayBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlStateSetPage);
    ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
}

void uiMineBaiyun::on_voiceAdjustBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlVoicePage);
}

void uiMineBaiyun::on_brightAdjustBtn_clicked()
{
    commonUIFunction::instance()->on_brightAdjustBtn_clicked(ui->controlStackedWidget,
                                                             ui->controlBrightPage);
}

void uiMineBaiyun::on_editionBtn_clicked()
{
    commonUIFunction::instance()->h2gSendHmiVerCMD();
    ui->controlStackedWidget->setCurrentWidget(ui->controlEditionPage);
}

void uiMineBaiyun::on_otherSetBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);//返回首界面
}

void uiMineBaiyun::on_brightMinusBtn_clicked()
{
//亮度调低
}

void uiMineBaiyun::on_brightPlusBtn_clicked()
{
//亮度调高
}

void uiMineBaiyun::on_brightBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlOtherSetPage);
}

void uiMineBaiyun::on_editionBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlOtherSetPage);
}

void uiMineBaiyun::on_voiceBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlOtherSetPage);
}

void uiMineBaiyun::on_emptyOkBtn_clicked()
{
    //发送设置空载信号

    emit h2gSetLoadSignal(PUB_WORK_STATE_EMPTYLOAD_TYPE);

    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
}

void uiMineBaiyun::on_emptyBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlLoadStatePage);
}

void uiMineBaiyun::on_fullunLoadOkBtn_clicked()
{
    emit h2gSetLoadSignal(PUB_WORK_STATE_FULLLOAD_TYPE);

    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
}
/*
void uiMineBaiyun::on_halfOkBtn_clicked()
{
    //发送设置半载信号
    emit h2gSetLoadSignal(PUB_WORK_STATE_HALFLOAD_TYPE);

    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
}
*/

void uiMineBaiyun::on_halfBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlLoadStatePage);
}

/*
void uiMineBaiyun::on_fullOkBtn_clicked()
{
    //发送设置满载信号
    emit h2gSetLoadSignal(PUB_WORK_STATE_FULLLOAD_TYPE);

    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
}
*/

void uiMineBaiyun::on_fullBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlLoadStatePage);
}

void uiMineBaiyun::g2hPosSlot(struct pubVehiclePosST info)  //网关发送hmi位置信息
{
    PosInfo* pos = PosInfo::getHandle();
    pos->setPosInfo(info);
}

void uiMineBaiyun::g2hReqWorkOnStatuSlot(struct pubVehicleWorkStatuST workStatu) //平台发送网关运营状态变更
{
//在信息窗口显示运营状态变更情况
    QString str;
    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));

    str.append("运营状态变更为");

    switch(workStatu.statuType)
    {
        case PUB_VEHICLE_READY_WORK_STATU:
        {
            str.append("就绪");
            break;
        }
        case PUB_VEHICLE_DEALY_WORK_STATU:
        {
            str.append("延时");
            break;
        }
        case PUB_VEHICLE_FAULT_WORK_STATU:
        {
            str.append("故障");
            break;
        }
        case PUB_VEHICLE_BACKUP_WORK_STATU:
        {
            str.append("备用");
            break;
        }
        default:
        {
            str.append("未知");
            break;
        }
    }

    commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
}

void uiMineBaiyun::g2hResWorkOnStatuSlot(struct pubVehicleResWorkStatuST cloudReq) //收到平台运营状态变更确认
{
    commonUIFunction::instance()->g2hResWorkOnStatuSlot(ui->messageBrowser,cloudReq);

}

void uiMineBaiyun::g2hReqWorkOptSlot(struct pubVehicleReqWorkOptST reqWork) //网关请求现场作业
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
        str.append(QString("发送驶入申请"));
    }
    else if(reqWork.cmdType == PUB_WORK_OPT_OUT_TASK)
    {
        str.append(QString("发送可驶离"));
    }
    else if(reqWork.cmdType == PUB_WORK_OPT_FINISH_TASK)
    {
        str.append(QString("发送停靠完成"));
    }
    else if(reqWork.cmdType == PUB_WORK_OPT_CLEAR_TASK)
    {
        str.append(QString("发送退出排队"));
    }
    else if(reqWork.cmdType == PUB_WORK_OPT_UNLOAD_TASK)
    {
        str.append(QString("允许卸料"));
    }
    else
    {
        str.append(QString("发送未知%1").arg(QString::number(reqWork.cmdType)));
    }

    commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
}

void uiMineBaiyun::g2hReqWorkRealOptSlot(struct pubVehicleReqWorkOptST reqWork) //网关请求现场作业
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
        str.append(QString("发送驶入申请"));
    }
    else if(reqWork.cmdType == PUB_WORK_OPT_OUT_TASK)
    {
        str.append(QString("发送可驶离"));
    }
    else if(reqWork.cmdType == PUB_WORK_OPT_FINISH_TASK)
    {
        str.append(QString("发送停靠完成"));
    }
    else if(reqWork.cmdType == PUB_WORK_OPT_CLEAR_TASK)
    {
        str.append(QString("发送退出排队"));
    }
    else
    {
        str.append(QString("发送未知%1").arg(QString::number(reqWork.cmdType)));
    }

    commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
}

void uiMineBaiyun::g2hResWorkOptSlot(struct pubVehicleResWorkOptST opt) //辅助车辆（网关已收到矿卡现场作业），矿卡（接收到辅助车辆相应作业信息）
{
    workStateInfo = opt;
    commonUIFunction::instance()->g2hResWorkOptSlot(ui->messageBrowser,opt);
}

void uiMineBaiyun::g2hCollWarnningSlot(struct pubVehicleCollWarnningST otherVehicle) //预警-碰撞检测
{
//预警显示
    if(otherVehicle.collType==0x01)//前向碰撞预警
    {
        ui->runDistLabel->show();
        ui->runDistLabel->setText(QString("%1%2").arg(QString::number(otherVehicle.dVehicleDis, 10, 0)).arg("M"));
        ui->stateInfoWidget->setStyleSheet("#stateInfoWidget{border-image: url(:/image/resource/stateMineRunColl.png);}");
    }
    if(aroundVehicleList.contains(otherVehicle))
    {
        int place = aroundVehicleList.indexOf(otherVehicle);
        aroundVehicleList.replace(place, otherVehicle);
    }
    else
    {
        aroundVehicleList.append(otherVehicle);
    }
}

void uiMineBaiyun::g2hDownMapFileSlot(int flag, QString path) //接收网关下载地图命令
{
//调用现在地图功能，并在信息窗口显示下载地图
    QString str;
    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));

    LOG_DEBUG("g2hDownMapFileSlot flag[%d], path[%s]", flag, path.toStdString().data());
    if(flag != MapUpdateMode_FASLE)
    {
        LOG_DEBUG("%s", path.toStdString().data());

        str.append("获取地图成功");
        str.append(QString("%1%2").arg(",地图文件:").arg(path.split("/").last()));

        if(flag == MapUpdateMode_NEW_FILE)
        {
            emit addDbInfoSignal(ParaParsing::instance()->paraSt.pub.mapPath+"/HDMap.db");
            //map->addDbInfo("/storage/mapinfo/HDMap.db");
        }
    }
    else
    {
        str.append("获取地图失败");
    }

    commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
}

void uiMineBaiyun::g2hVersionSlot(struct versionST versiondataSt) //版本号
{
    commonUIFunction::instance()->g2hVersionSlot(ui->editionTable,versiondataSt,&versiondata);
}

void uiMineBaiyun::g2hDownTaskFileSlot(bool flag, QString path,QStringList taskList) //下载文件
{
//调用更新任务文件，并载信息窗口显示下载任务文件
    QString str;
    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));

    if(flag)
    {
        str.append("获取任务文件成功");
        str.append(QString("%1%2").arg(",任务文件:").arg(path.split("/").last()));


		if(!taskList.isEmpty())
        {
            map->addTaskInfo(taskList);
        }
    }
    else
    {
        str.append("获取任务文件失败");
    }

    commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
}

void uiMineBaiyun::g2hDownPermissionFileSlot(bool flag, QStringList permissionList,quint32 endPointNum)
{

    commonUIFunction::instance()->g2hDownPermissionFileSlot(flag,  permissionList, endPointNum,
                                                            ui->messageBrowser,map);

}

void uiMineBaiyun::g2hStartUpSlot(struct startUpST opt) //网关发送开机命令
{
//信息窗口显示，网关开机
    QString str;

    if(opt.startFlag)
    {
        str.append(QDateTime::currentDateTime().toString("hh:mm:ss :") + QString("收到网关异常开机信号"));
    }
    else
    {
        str.append(QDateTime::currentDateTime().toString("hh:mm:ss :") + QString("收到网关正常开机信号"));
    }

    commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
}

void uiMineBaiyun::g2hShutDownAckSlot() //网关关机应答
{
//信息窗口显示，网管关机
    QString str;
    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :") + QString("收到网关关机信号"));
    commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
}

void uiMineBaiyun::g2hDevNameSlot(struct pubVehicleDevNameST imeiName) //网关检权获取名称后发送给hmi
{
    localImeiName = imeiName;
    quint64 width=real2pix*ParaParsing::instance()->paraSt.pub.vehicleWidth;
    quint64 length=real2pix*ParaParsing::instance()->paraSt.pub.vehicleLength;
    if(width<10)width=10;
    if(length<10)length=10;

    map->setSelfVehicle(MINELOADER, QString::fromUtf8(imeiName.localname), QSize(width, length));
}

void uiMineBaiyun::g2hMatAckSlot(short mater) //物料变更
{
//显示物料变更为何物
    QString str;
    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :") + QString("物料变更为"));

    switch(mater)
    {
        case PUB_MATER_CLOUD_ROCK:
        {
            str.append("白云岩");
            break;
        }
        case PUB_MATER_HIGH_ORE:
        {
            str.append("高磁矿");
            break;
        }
        case PUB_MATER_LOW_ORE:
        {
            str.append("低磁矿");
            break;
        }
        default:
        {
            str.append("未知");
            break;
        }
    }

    commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
}

void uiMineBaiyun::g2hReqTaskSlot(struct pubVehicleReqTaskST reqTask) //请求任务
{
    commonUIFunction::instance()->g2hReqTaskSlot(ui->messageBrowser,reqTask);
}

void uiMineBaiyun::g2hResTaskSlot(struct pubVehicleResTaskST resTask) //接收平台响应任务
{
    commonUIFunction::instance()->g2hResTaskSlot(ui->messageBrowser,ui->titleDestLabel,resTask,taskInfo);
}



void uiMineBaiyun::g2hResPermissionSlot(struct pubDrivePermissionListST resPermission) //接收平台响应任务
{

}

void uiMineBaiyun::g2hReqRoadSlot(struct pubVehicleReqRoadST reqRoad) //申请路权
{
//信息窗口显示申请路权
    QString str;

    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));
    str.append(QString::number(reqRoad.roadNum) + QString("号"));

    if(reqRoad.roadType== PUB_VEHICLE_ROAD_TYPE_CROSSING)
    {
        str.append("路口");
    }
    else if(reqRoad.roadType== PUB_VEHICLEROAD_TYPE_ONEWAY)
    {
        str.append("单行线");
    }

    if(reqRoad.roadDir == PUB_VEHICLEROAD_DIR_LOAD)
    {
        str.append("上行");
    }
    else if(reqRoad.roadDir == PUB_VEHICLEROAD_DOR_UNLOAD)
    {
        str.append("下行");
    }

    str.append("占用申请");

    commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
}

void uiMineBaiyun::g2hResRoadSlot(struct pubVehicleResRoadST resRoad) //平台回复路权申请
{
//信息窗口显示收到路权申请
    QString str;

    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));
    str.append(QString::number(resRoad.roadNum) + QString("号"));

    if(resRoad.roadType== PUB_VEHICLE_ROAD_TYPE_CROSSING)
    {
        str.append("路口");
    }
    else if(resRoad.roadType== PUB_VEHICLEROAD_TYPE_ONEWAY)
    {
        str.append("单行线");
    }

    if(resRoad.roadDir == PUB_VEHICLEROAD_DIR_LOAD)
    {
        str.append("上行");
    }
    else if(resRoad.roadDir == PUB_VEHICLEROAD_DOR_UNLOAD)
    {
        str.append("下行");
    }

    if(resRoad.result == 1)
    {
        str.append("占用成功");
    }
    else if(resRoad.result == 2)
    {
        str.append("占用失败");
    }
    else
    {
        str.append("占用情况未知");
    }

    commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
}

void uiMineBaiyun::g2hFreeRoadSlot(struct pubVehicleReqRoadST freeRoad) //平台下发释放路权
{
//信息窗口显示收到平台下发释放路权
    QString str;

    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));
    str.append(QString::number(freeRoad.roadNum) + QString("号"));

    if(freeRoad.roadType== PUB_VEHICLE_ROAD_TYPE_CROSSING)
    {
        str.append("路口");
    }
    else if(freeRoad.roadType== PUB_VEHICLEROAD_TYPE_ONEWAY)
    {
        str.append("单行线");
    }

    if(freeRoad.roadDir == PUB_VEHICLEROAD_DIR_LOAD)
    {
        str.append("上行");
    }
    else if(freeRoad.roadDir == PUB_VEHICLEROAD_DOR_UNLOAD)
    {
        str.append("下行");
    }

    str.append("释放路权");

    commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
}

void uiMineBaiyun::g2hTemporaryTaskSlot(char type) //平台下发临时调度作业
{
    commonUIFunction::instance()->g2hTemporaryTaskSlot(ui->messageBrowser,type);
}

void uiMineBaiyun::g2hMasterWarnningSlot(struct pubVehicleMasterWarnningST materWarning) //主控发送预警,障碍物
{
//    PosInfo* pos = PosInfo::getHandle();
//    char gearState = pos->getGearState();

//    if((workStateInfo.cmdType != PUB_WORK_OPT_RECOVER_TASK) && (workStateInfo.cmdType != PUB_WORK_OPT_IN_TASK) && (gearState == 0x01))//D
//    {
//        ui->runDistLabel->show();
//        ui->runDistLabel->setText(QString("%1%2").arg(QString::number(materWarning.disy, 10, 0)).arg("M"));
//        ui->stateInfoWidget->setStyleSheet("#stateInfoWidget{border-image: url(:/image/resource/stateMineRunOther.png);}");
//    }
//    else if((workStateInfo.cmdType == PUB_WORK_OPT_IN_TASK) && (gearState == 0x03))//R
    {
        ui->reversDistLabel->show();
        ui->reversDistLabel->setText(QString("%1%2").arg(QString::number(materWarning.disy, 10, 0)).arg("M"));
        ui->stateInfoWidget->setStyleSheet("#stateInfoWidget{border-image: url(:/image/resource/stateMineRunOther.png);}");
    }
}

void uiMineBaiyun::g2hGatewayOffline(bool status)
{
    commonUIFunction::instance()->g2hGatewayOffline(ui->messageBrowser,status);

}

void uiMineBaiyun::insertWarnningSlots(int num)
{
    try
    {
        QString str;
        str.append(QDateTime::currentDateTime().toString("hh:mm:ss :")).append("增加").append(warningMap.at(num).first.data());
        commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
    }
    catch(const std::exception)
    {

    }
}

void uiMineBaiyun::removeWarnningSlots(int num)
{
    try
    {
        QString str;
        str.append(QDateTime::currentDateTime().toString("hh:mm:ss :")).append("移除").append(warningMap.at(num).first.data());
        commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
        if(ui->controlStackedWidget->currentWidget()!=ui->controlAlalrmManPage)
        {
            preWarnControlStackedWidget=ui->controlStackedWidget->currentWidget();
            ui->controlStackedWidget->setCurrentWidget(ui->controlAlalrmManPage);
        }
    }
    catch(const std::exception)
    {

    }
}

void uiMineBaiyun::titleTimerSlot()//界面头信息更新显示
{
    ui->titleTimeLabel->setText(QDateTime::currentDateTime().toString("yyyy/MM/dd - hh:mm:ss"));

    PosInfo* pos = PosInfo::getHandle();
    pos->getWorkStatu();

    QString stateString;
    stateString.append("运营状态：");
    switch(pos->getWorkStatu())
    {
        case PUB_VEHICLE_READY_WORK_STATU:
        {
            stateString.append("就绪");
            break;
        }
        case PUB_VEHICLE_DEALY_WORK_STATU:
        {
            stateString.append("延时");
            break;
        }
        case PUB_VEHICLE_FAULT_WORK_STATU:
        {
            stateString.append("故障");
            break;
        }
        case PUB_VEHICLE_BACKUP_WORK_STATU:
        {
            stateString.append("备用");
            break;
        }
        case PUB_VEHICLE_NA_WORK_STATU:
        {
            stateString.append("就绪");
            break;
        }
        default:
        {
            stateString.append("未知");
            break;
        }
    }

    if((pos->getWorkStatu() == PUB_VEHICLE_DEALY_WORK_STATU))
    {
        switch(pos->getDelay())
        {
            case PUB_FAULT_PWR:
            {
                stateString.append(" - 电机故障");
                break;
            }
            case PUB_FAULT_CABLE:
            {
                stateString.append(" - 电缆故障");
                break;
            }
            case PUB_FAULT_CABLE_BROKEN:
            {
                stateString.append(" - 断钢缆");
                break;
            }
            case PUB_FAULT_OTHER:
            {
                stateString.append(" - 其他故障");
                break;
            }
            case PUB_DELAY_STOP:
            {
                stateString.append(" - 暂停派遣");
                break;
            }
            case PUB_DELAY_SCHEDULE_FALUT:
            {
                stateString.append(" - 调度失败");
                break;
            }
            case PUB_DELAY_ERROR_ROAD:
            {
                stateString.append(" - 无效道路");
                break;
            }
            case PUB_DELAY_AVOIDBLAST:
            {
                stateString.append(" - 避炮");
                break;
            }
            case PUB_DELAY_OIL:
            {
                stateString.append(" - 停车加油");
                break;
            }
            case PUB_DELAY_REMOTE:
            {
                stateString.append(" - 遥控停车");
                break;
            }
            case PUB_DELAY_EMERGENCY:
            {
                stateString.append(" - 紧急制动");
                break;
            }
            case PUB_DELAY_OVER:
            {
                stateString.append(" - 任务终止");
                break;
            }
            case PUB_DELAY_CHANGE:
            {
                stateString.append(" - 障碍物停车");
                break;
            }
            case PUB_DELAY_COLD:
            {
                stateString.append(" - 车辆冷却");
                break;
            }
            case PUB_DELAY_HITCH:
            {
                stateString.append(" - 短时故障");
                break;
            }
            case PUB_DELAY_WAIT:
            {
                stateString.append(" - 等待路权");
                break;
            }
//            case PUB_DELAY_EXCAVATOR_CHECK:
//            {
//                stateString.append(" - 点检");
//                break;
//            }
//            case PUB_DELAY_EXCAVATOR_CHANGE:
//            {
//                stateString.append(" - 交接班");
//                break;
//            }
//            case PUB_DELAY_EXCAVATOR_FOOD:
//            {
//                stateString.append(" - 班中餐");
//                break;
//            }
            case PUB_DELAY_EXCAVATOR_PUSH:
            {
                stateString.append(" - 推铲根");
                break;
            }
            case PUB_DELAY_EXCAVATOR_AVOIDBLAST:
            {
                stateString.append(" - 避炮");
                break;
            }
            case PUB_DELAY_EXCAVATOR_SHORT:
            {
                stateString.append(" - 短时故障");
                break;
            }
            case PUB_DELAY_EXCAVATOR_SWEEP:
            {
                stateString.append(" - 扫货");
                break;
            }
            case PUB_DELAY_EXCAVATOR_CARGO:
            {
                stateString.append(" - 倒货");
                break;
            }
            case PUB_DELAY_EXCAVATOR_DISPLACE:
            {
                stateString.append(" - 移位");
                break;
            }
            case PUB_DELAY_EXCAVATOR_DIG:
            {
                stateString.append(" - 挖根底");
                break;
            }
            case PUB_DELAY_EXCAVATOR_POWER:
            {
                stateString.append(" - 停电");
                break;
            }
            case PUB_DELAY_EXCAVATOR_DROP:
            {
                stateString.append(" - 掉牙尖");
                break;
            }
            case PUB_DELAY_EXCAVATOR_PUMP:
            {
                stateString.append(" - 挖泵坑");
                break;
            }
            case PUB_DELAY_EXCAVATOR_CABLE:
            {
                stateString.append(" - 倒电缆");
                break;
            }
            default:
            {
                break;
            }
        }
    }

    if((pos->getWorkStatu() == PUB_VEHICLE_FAULT_WORK_STATU))
    {
        switch(pos->getDelay())
        {
            case PUB_FAULT_PWR:
            {
                stateString.append(" - 电机故障");
                break;
            }
            case PUB_FAULT_CABLE:
            {
                stateString.append(" - 电缆故障");
                break;
            }
            case PUB_FAULT_CABLE_BROKEN:
            {
                stateString.append(" - 断钢缆");
                break;
            }
            case PUB_FAULT_OTHER:
            {
                stateString.append(" - 其他故障");
                break;
            }
            case PUB_DELAY_STOP:
            {
                stateString.append(" - 暂停派遣");
                break;
            }
            case PUB_DELAY_SCHEDULE_FALUT:
            {
                stateString.append(" - 调度失败");
                break;
            }
            case PUB_DELAY_ERROR_ROAD:
            {
                stateString.append(" - 无效道路");
                break;
            }
            case PUB_DELAY_AVOIDBLAST:
            {
                stateString.append(" - 避炮");
                break;
            }
            case PUB_DELAY_OIL:
            {
                stateString.append(" - 停车加油");
                break;
            }
            case PUB_DELAY_REMOTE:
            {
                stateString.append(" - 遥控停车");
                break;
            }
            case PUB_DELAY_EMERGENCY:
            {
                stateString.append(" - 紧急制动");
                break;
            }
            case PUB_DELAY_OVER:
            {
                stateString.append(" - 任务终止");
                break;
            }
            case PUB_DELAY_CHANGE:
            {
                stateString.append(" - 障碍物停车");
                break;
            }
            case PUB_DELAY_COLD:
            {
                stateString.append(" - 车辆冷却");
                break;
            }
            case PUB_DELAY_HITCH:
            {
                stateString.append(" - 短时故障");
                break;
            }
            case PUB_DELAY_WAIT:
            {
                stateString.append(" - 等待路权");
                break;
            }
//            case PUB_DELAY_EXCAVATOR_CHECK:
//            {
//                stateString.append(" - 点检");
//                break;
//            }
//            case PUB_DELAY_EXCAVATOR_CHANGE:
//            {
//                stateString.append(" - 交接班");
//                break;
//            }
//            case PUB_DELAY_EXCAVATOR_FOOD:
//            {
//                stateString.append(" - 班中餐");
//                break;
//            }
            case PUB_DELAY_EXCAVATOR_PUSH:
            {
                stateString.append(" - 推铲根");
                break;
            }
            case PUB_DELAY_EXCAVATOR_AVOIDBLAST:
            {
                stateString.append(" - 避炮");
                break;
            }
            case PUB_DELAY_EXCAVATOR_SHORT:
            {
                stateString.append(" - 短时故障");
                break;
            }
            case PUB_DELAY_EXCAVATOR_SWEEP:
            {
                stateString.append(" - 扫货");
                break;
            }
            case PUB_DELAY_EXCAVATOR_CARGO:
            {
                stateString.append(" - 倒货");
                break;
            }
            case PUB_DELAY_EXCAVATOR_DISPLACE:
            {
                stateString.append(" - 移位");
                break;
            }
            case PUB_DELAY_EXCAVATOR_DIG:
            {
                stateString.append(" - 挖根底");
                break;
            }
            case PUB_DELAY_EXCAVATOR_POWER:
            {
                stateString.append(" - 停电");
                break;
            }
            case PUB_DELAY_EXCAVATOR_DROP:
            {
                stateString.append(" - 掉牙尖");
                break;
            }
            case PUB_DELAY_EXCAVATOR_PUMP:
            {
                stateString.append(" - 挖泵坑");
                break;
            }
            case PUB_DELAY_EXCAVATOR_CABLE:
            {
                stateString.append(" - 倒电缆");
                break;
            }
            default:
            {
                break;
            }
        }
    }

    ui->titleStateLabel->setText(stateString);

    ui->titleNameLabel->setText(QString("设备ID:") +  QString(QByteArray(localImeiName.localname, sizeof(localImeiName.localname))));

    char rssi = pos->getRssi();

    bool warnV2N=pos->getWarnningBit(v2n);


    if(rssi < 10)
    {
        if(warnV2N)//云平台连接故障，没有信号
            ui->titleSignalLabel->setStyleSheet("border-image:url(:/image/resource/noConnect.png);");
        else//云平台连接正常，但是没有rssi信号
            ui->titleSignalLabel->setStyleSheet("border-image:url(:/image/resource/signal0.png);");
    }
    else if((rssi >= 10) && (rssi < 13))
    {
        if(warnV2N)//云平台连接故障，有信号
            ui->titleSignalLabel->setStyleSheet("border-image:url(:/image/resource/noConnectSignal.png);");
        else//云平台连接正常，有rssi信号
            ui->titleSignalLabel->setStyleSheet("border-image: url(:/image/resource/signal1.png);");
    }
    else if((rssi >= 13) && (rssi < 16))
    {
        if(warnV2N)//云平台连接故障，有信号
            ui->titleSignalLabel->setStyleSheet("border-image:url(:/image/resource/noConnectSignal.png);");
        else//云平台连接正常，有rssi信号
            ui->titleSignalLabel->setStyleSheet("border-image: url(:/image/resource/signal2.png);");
    }
    else if((rssi >= 16) && (rssi < 19))
    {
        if(warnV2N)//云平台连接故障，有信号
            ui->titleSignalLabel->setStyleSheet("border-image:url(:/image/resource/noConnectSignal.png);");
        else//云平台连接正常，有rssi信号
            ui->titleSignalLabel->setStyleSheet("border-image: url(:/image/resource/signal3.png);");
    }
    else if((rssi >= 20) && (rssi < 25))
    {
        if(warnV2N)//云平台连接故障，有信号
            ui->titleSignalLabel->setStyleSheet("border-image:url(:/image/resource/noConnectSignal.png);");
        else//云平台连接正常，有rssi信号
            ui->titleSignalLabel->setStyleSheet("border-image: url(:/image/resource/signal4.png);");
    }
    else if(rssi >= 25)
    {
        if(warnV2N)//云平台连接故障，有信号
            ui->titleSignalLabel->setStyleSheet("border-image:url(:/image/resource/noConnectSignal.png);");
        else//云平台连接正常，有rssi信号
            ui->titleSignalLabel->setStyleSheet("border-image: url(:/image/resource/signal5.png);");
    }

    ui->runDistLabel->hide();
    ui->reversDistLabel->hide();

    char gearState;
    char engineState;
    char aiState;
    char gpsState;
    struct latlngheading curPos;

    float maxSpeed;
    float speed;

    aiState = pos->getAiState();
    gearState = pos->getGearState();
    engineState = pos->getEngineState();
    gpsState = pos->getGpsState();
    maxSpeed = pos->getMaxSpeed();
    speed = pos->getSpeed();
    pos->getCurPos(&curPos);

    if(aiState == 0x01)//Ai
    {
        ui->mineADLabel->setStyleSheet("border-image: url(:/image/resource/ADable.png);");
    }
    else//~Ai
    {
        ui->mineADLabel->setStyleSheet("border-image: url(:/image/resource/ADdisable.png);");
    }

    if(gearState == 0x00)//P
    {
        ui->mineGearLabel->setStyleSheet("border-image: url(:/image/resource/P.png);");
    }
    else if(gearState == 0x01)//D
    {
        ui->mineGearLabel->setStyleSheet("border-image: url(:/image/resource/D.png);");
    }
    else if(gearState == 0x02)//N
    {
        ui->mineGearLabel->setStyleSheet("border-image: url(:/image/resource/N.png);");
    }
    else if(gearState == 0x03)//R
    {
        ui->mineGearLabel->setStyleSheet("border-image: url(:/image/resource/R.png);");
    }

    if(engineState == 0x01)//牵引M
    {
        ui->mineEngineLabel->setStyleSheet("border-image: url(:/image/resource/M.png);");
    }
    else if(engineState == 0x02)//制动B
    {
        ui->mineEngineLabel->setStyleSheet("border-image: url(:/image/resource/B.png);");
    }
    else//惰性NULL
    {
        ui->mineEngineLabel->setStyleSheet("border-image: url(:/image/resource/Null.png);");
    }

    ui->curSpeedLabel->setText(QString::number(speed, 'd', 0));
    ui->maxSpeedLabel->setText(QString::number(maxSpeed, 'd', 0));
    if(speed >= maxSpeed)
    {
        if(!speedBlinkTimer->isActive())
        {
            speedBlinkTimer->start();
        }
    }
    else
    {
        speedBlinkTimer->stop();
        ui->curSpeedLabel->setStyleSheet("color:white;\
                                         font:18pt \"Microsoft YaHei\";");
        ui->speedUnitLabel->setStyleSheet("color:white;\
                                          font:10pt \"Microsoft YaHei\";");
    }

    if((workStateInfo.taskType == PUB_WORK_OPT_LOAD_TYPE) && (workStateInfo.cmdType == PUB_WORK_OPT_RECOVER_TASK))//装载停靠
    {
        ui->stateInfoWidget->setStyleSheet("#stateInfoWidget{border-image: url(:/image/resource/stateLoadWorking.png);}");
    }
    else if((workStateInfo.taskType == PUB_WORK_OPT_UNLOAD_TYPE) && (workStateInfo.cmdType == PUB_WORK_OPT_RECOVER_TASK))//卸载停靠
    {
        ui->stateInfoWidget->setStyleSheet("#stateInfoWidget{border-image: url(:/image/resource/stateUnloadWorking.png);}");
    }
    else if(workStateInfo.cmdType == PUB_WORK_OPT_OUT_TASK && ui->runDistLabel->isHidden())//可驶离，往前走,前方无车
    {
        ui->stateInfoWidget->setStyleSheet("#stateInfoWidget{border-image: url(:/image/resource/stateMineRun.png);}");
    }
    else if(workStateInfo.cmdType == PUB_WORK_OPT_OUT_TASK && !ui->runDistLabel->isHidden())//可驶离，往前走,前方有车
    {
        ui->stateInfoWidget->setStyleSheet("#stateInfoWidget{border-image: url(:/image/resource/stateMineRunColl.png);}");
    }
    else if(workStateInfo.cmdType == PUB_WORK_OPT_IN_TASK)//可停靠，往后倒
    {
        ui->stateInfoWidget->setStyleSheet("#stateInfoWidget{border-image: url(:/image/resource/stateUnloadWorkRun.png);}");
    }
    else
    {
        ui->stateInfoWidget->setStyleSheet("#stateInfoWidget{border-image: url(:/image/resource/stateMineRun.png);}");
    }

    map->updateGpsState(gpsState);

    if((curPos.lat > 0.0000001) && (curPos.lat < 360) && (curPos.lng > 0.0000001) && (curPos.lng < 360))
    {
        if(!map->scene->items().contains(map->selfVehicle))
        {
            if(map->selfVehicle == NULL)
            {
                quint64 width=real2pix*ParaParsing::instance()->paraSt.pub.vehicleWidth;
                quint64 length=real2pix*ParaParsing::instance()->paraSt.pub.vehicleLength;
                if(width<10)width=10;
                if(length<10)length=10;
                map->setSelfVehicle(MINELOADER, QString::fromUtf8(localImeiName.localname), QSize(width, length));
            }

            if(pos->getLoadState())
            {
                map->selfVehicle->setType(MINELOADER,0);
            }
            else
            {
                map->selfVehicle->setType(MINEUNLOADER,0);
            }

            
        }

        if((map->permissionItemGroup!=NULL)&&(map->taskItemGroup!=NULL))
        {
            map->taskItemGroup->stackBefore(map->permissionItemGroup);
        }
        if(map->taskItemGroup!=NULL)
            map->taskItemGroup->stackBefore(map->selfVehicle);
        if(map->permissionItemGroup!=NULL)
            map->permissionItemGroup->stackBefore(map->selfVehicle);
        //更新任务路径已走过的点
        emit curPosSignal(curPos);
        map->updateVehiclePos(curPos);
        map->scene->setSceneRect(map->scene->itemsBoundingRect());
        map->view->centerOn(map->selfVehicle->typeItem);
    }

}

void uiMineBaiyun::messageEndTimerSlot()
{
    ui->messageBrowser->moveCursor(QTextCursor::Start);
}

//消息往上滑动
void uiMineBaiyun::on_messageUpBtn_clicked()
{
    ui->messageBrowser->moveCursor(QTextCursor::Up);
    messageEndTimer->start(2000);
}

//信息往下滑动
void uiMineBaiyun::on_messageDownBtn_clicked()
{
    ui->messageBrowser->moveCursor(QTextCursor::Down);
    messageEndTimer->start(2000);
}

void uiMineBaiyun::blinkTimerSlot()
{
    if(blinkFlag)
    {
        blinkFlag = false;
        ui->alarmManInfoLabel->setStyleSheet("border-color:rgb(254, 28, 29);\
                                             border-width:2px;\
                                             border-style:solid;\
                                             background-color:rgb(60, 22, 45);\
                                             color:white;\
                                             font:14pt, \"Microsoft YaHei\";");
    }
    else
    {
        blinkFlag = true;
        ui->alarmManInfoLabel->setStyleSheet("border-color:rgb(254, 28, 29);\
                                             border-width:2px;\
                                             border-style:solid;\
                                             background-color:rgb(99, 23, 41);\
                                             color:white;\
                                             font:14pt, \"Microsoft YaHei\";");
    }
}

void uiMineBaiyun::speedBlinkTimerSlot()
{
    if(speedBlinkFlag)
    {
        speedBlinkFlag = false;
        ui->curSpeedLabel->setStyleSheet("color:red;\
                                          font:18pt, \"Microsoft YaHei\";");
        ui->speedUnitLabel->setStyleSheet("color:red;\
                                          font:10pt \"Microsoft YaHei\";");

    }
    else
    {
        speedBlinkFlag = true;
        ui->curSpeedLabel->setStyleSheet("color:white;\
                                          font:18pt, \"Microsoft YaHei\";");
        ui->speedUnitLabel->setStyleSheet("color:white;\
                                          font:10pt \"Microsoft YaHei\";");

    }
}

void uiMineBaiyun::collectionTimerSlot()
{
    PosInfo* pos = PosInfo::getHandle();
    struct latlngheading curPos;
    pos->getCurPos(&curPos);

    addPosDataFun(&collectionList, curPos);
}

void uiMineBaiyun::aroundVehicleUpdateTimerSlot()
{
    map->updateVehicleList(aroundVehicleList);
    aroundVehicleList.clear();
}

void uiMineBaiyun::on_collectionBackBtn_clicked()
{
    ui->centralStackedWidget->setCurrentWidget(ui->centralWorkStatePage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlWorkSetPage);
}

void uiMineBaiyun::on_startCollectionBtn_clicked()
{
    ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionStatePage);
    collectionTimer->start();
}

void uiMineBaiyun::on_updateDataBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionUpdateDataPage);
}

void uiMineBaiyun::on_leftBtn_clicked()
{
    ui->suspendRestartBtn->setChecked(false);
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionStatePage);
}

void uiMineBaiyun::on_rightBtn_clicked()
{
    ui->suspendRestartBtn->setChecked(false);
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionStatePage);
}

void uiMineBaiyun::on_suspendRestartBtn_clicked()
{
    PosInfo* pos = PosInfo::getHandle();
    struct latlngheading* data = new struct latlngheading;

    if(ui->suspendRestartBtn->isChecked())
    {
        ui->suspendRestartBtnLabel->setText("继续");
        pos->getCurPos(data);

        collectionList.append(data);
        collectionTimer->stop();
    }
    else
    {
        ui->suspendRestartBtnLabel->setText("暂停");
        pos->getCurPos(data);

        collectionList.append(data);
        collectionTimer->start();
    }
}

void uiMineBaiyun::on_stopBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionOverPage);
//    collectionTimer->stop();
}

void uiMineBaiyun::on_collectionOverBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionStatePage);
}

void uiMineBaiyun::on_saveBtn_clicked()
{
    //生成路径文件lpx
    collectionTimer->stop();
    generateRoadLpxFile(QString("%1/%2").arg(ParaParsing::instance()->paraSt.pub.roadPath).arg(QDateTime::currentDateTime().toString("MM-dd-hh-mm-ss.lpx")),collectionList);
    collectionList.clear();
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionUpdateDataPage);
}

void uiMineBaiyun::on_deleteBtn_clicked()
{
    collectionTimer->stop();
    collectionList.clear();
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionDeleteDataPage);
}

void uiMineBaiyun::on_updateDataOkBtn_clicked()
{
    //提交路径文件
    fileUpST roadFile;
    memset((char*)(&roadFile), 0, sizeof(fileUpST));

    QDir roadDir;
    //roadDir.setPath("/tftpServer");
    roadDir.setPath(ParaParsing::instance()->paraSt.pub.roadPath);
    //if(roadDir.exists("/tftpServer"))
    if(roadDir.exists())
    {
        QStringList filelist = roadDir.entryList(QDir::AllEntries | QDir::NoDot | QDir::NoDotDot, QDir::Time);
        if(filelist.size() > 0)
        {
            roadFile.fileType = 0x00;
            roadFile.result = 0x01;
            //QString filePath = QString("%1/%2").arg("/tftpServer").arg(filelist.at(0));
            QString filePath = QString("%1/%2").arg(ParaParsing::instance()->paraSt.pub.roadPath).arg(filelist.at(0));
            FileManager* manager = FileManager::instance();
            QString md5 = manager->fileMd5(static_cast<const QString>(filePath));
            memmove(roadFile.md5, md5.toStdString().data(), md5.length());
            memmove(roadFile.fileDir, filelist.at(0).toStdString().data(), filelist.at(0).length());
            qDebug()<<"roadFile:"<<roadFile.fileDir;
            emit h2gUpFileSignal(roadFile);
        }
    }
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
}

void uiMineBaiyun::on_updateDataBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionOverPage);
}

void uiMineBaiyun::on_deleteDataOkBtn_clicked()
{
    //删除采集到的数据
    collectionList.clear();
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
}

void uiMineBaiyun::on_deleteDataBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionOverPage);
}

void uiMineBaiyun::on_voiceMinusBtn_clicked()
{
//    g2hDownTaskFileSlot(true, "/storage/taskinfo/d0-s0-empty.tar.gz");
}

void uiMineBaiyun::on_voicePlusBtn_clicked()
{
//    g2hDownTaskFileSlot(true, "/storage/taskinfo/s0-d0-full.tar.gz");
}


void uiMineBaiyun::on_alarmManOkBtn_clicked()
{
    preWarnCentralWorkStatePage=ui->centralStackedWidget->currentWidget();
    ui->controlStackedWidget->setCurrentWidget(ui->controlWorkSetPage);
    ui->centralStackedWidget->setCurrentWidget(ui->centralWorkStatePage);
}
