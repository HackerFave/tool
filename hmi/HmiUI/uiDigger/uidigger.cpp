#include "uidigger.h"
#include "ui_uidigger.h"
#include "HmiFunction/log.h"
uiDigger::uiDigger(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::uiDigger)
{
    ui->setupUi(this);

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

//    workChoice = new WorkStateChoice;
//    QGridLayout *workChoiceGrid = new QGridLayout;
//    workChoiceGrid->addWidget(workChoice);
//    ui->centralStackedWidget->widget(3)->setLayout(workChoiceGrid);

    abnormalChoice = new AbnormalDigger;
    QGridLayout *abnormalChoiceGrid = new QGridLayout;
    abnormalChoiceGrid->addWidget(abnormalChoice);
    ui->centralAbnormalPage->setLayout(abnormalChoiceGrid);

    fixSet = new FixSetDigger;
    QGridLayout *fixSetGrid = new QGridLayout;
    fixSetGrid->addWidget(fixSet);
    ui->centralFixSetPage->setLayout(fixSetGrid);

    hmicmd = new CmdDealClass;
    ParaParsing* para = ParaParsing::instance();
    hmicmd->udpInit(para->paraSt.pub.localIp, para->paraSt.pub.localPort, para->paraSt.pub.remoteIp, para->paraSt.pub.remotePort);

    memset((char*)&localImeiName, 0, sizeof(localImeiName));

    titleTimer = new QTimer;
    titleTimer->setInterval(1000);
    titleTimer->start();

    messageEndTimer = new QTimer;
    blinkTimer = new QTimer;
    blinkTimer->setInterval(500);

    collectionTimer = new QTimer;
    collectionTimer->setInterval(1000);

    aroundVehicleUpdateTimer = new QTimer;
    aroundVehicleUpdateTimer->setInterval(5000);
    aroundVehicleUpdateTimer->start();

    ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);

    ui->editionTable->setItem(0, 1 , new QTableWidgetItem);
    ui->editionTable->setItem(1, 1 , new QTableWidgetItem);
    ui->editionTable->setItem(2, 1 , new QTableWidgetItem);
    ui->editionTable->setItem(3, 1 , new QTableWidgetItem);
    ui->editionTable->setItem(4, 1 , new QTableWidgetItem);
    ui->editionTable->setItem(5, 1 , new QTableWidgetItem);
    ui->editionTable->item(5,1)->setText(QString("V%1.%2.%3").arg(QString::number(DEV_COMPAT_VERSION, 10), 2, QLatin1Char('0')).arg(QString::number(DEV_MAIN_VERSION, 10), 2, QLatin1Char('0'))
                                         .arg(QString::number(DEV_SUB_VERSION, 10), 2, QLatin1Char('0')));

    currentStopNum = 1;
    areaside = 0x01;
    sidefixvalue = 270;

    connect(this, SIGNAL(h2gSetWorkStatuSignal(pubVehicleWorkStatuST)), hmicmd, SLOT(h2gSetWorkStatuSlots(pubVehicleWorkStatuST))); //设置运用状态
    connect(this, SIGNAL(h2gUpFileSignal(fileUpST)), hmicmd, SLOT(h2gUpFileSlots(fileUpST))); //文件上传,fileDir不用部分为0
    connect(this, SIGNAL(h2gShutDownSignal()), hmicmd, SLOT(h2gShutDownSlots()));  //发送网关关机命令
    connect(this, SIGNAL(h2gReqWorkOptSignal(pubVehicleReqWorkOptST)), hmicmd, SLOT(h2gReqWorkOptSlots(pubVehicleReqWorkOptST))); //相应现场调度
    connect(this, SIGNAL(h2gSetLoadSignal(char)), hmicmd, SLOT(h2gSetLoadSlots(char))); //设置物料类型
    connect(this, SIGNAL(h2gReqMaterSignal(short)), hmicmd, SLOT(h2gReqMaterSlots(short))); //设置物料类型
    connect(this, SIGNAL(h2gResWorkOptSignal(pubVehicleResWorkOptST)), hmicmd, SLOT(h2gResWorkOptSlots(pubVehicleResWorkOptST)));//发送驶入驶出命令

    connect(hmicmd, SIGNAL(g2hPosSignal(struct pubVehiclePosST)), this, SLOT(g2hPosSlot(struct pubVehiclePosST)));  //网关发送hmi位置信息
    connect(hmicmd, SIGNAL(g2hReqWorkOnStatuSignal(struct pubVehicleWorkStatuST)), this, SLOT(g2hReqWorkOnStatuSlot(struct pubVehicleWorkStatuST))); //平台发送网关运营状态变更
    connect(hmicmd, SIGNAL(g2hResWorkOnStatuSignal(struct pubVehicleResWorkStatuST)), this, SLOT(g2hResWorkOnStatuSlot(struct pubVehicleResWorkStatuST))); //收到平台运营状态变更确认
    connect(hmicmd, SIGNAL(g2hReqWorkOptSignal(struct pubVehicleReqWorkOptST)), this, SLOT(g2hReqWorkOptSlot(struct pubVehicleReqWorkOptST))); //网关请求现场作业
    connect(hmicmd, SIGNAL(g2hReqWorkRealOptSignal(struct pubVehicleReqWorkOptST)), this, SLOT(g2hReqWorkRealOptSlot(struct pubVehicleReqWorkOptST))); //网关请求现场作业
    connect(hmicmd, SIGNAL(g2hResWorkOptSignal(struct pubVehicleResWorkOptST)), this, SLOT(g2hResWorkOptSlot(struct pubVehicleResWorkOptST))); //辅助车辆（网关已收到矿卡现场作业），矿卡（接收到辅助车辆相应作业信息）
    connect(hmicmd, SIGNAL(g2hCollWarnningSignal(struct pubVehicleCollWarnningST)), this, SLOT(g2hCollWarnningSlot(struct pubVehicleCollWarnningST))); //预警
    connect(hmicmd, SIGNAL(g2hDownMapFileSignal(int, QString)), this, SLOT(g2hDownMapFileSlot(int, QString))); //接收网关下载地图命令
    connect(hmicmd, SIGNAL(g2hVersionSignal(struct versionST)), this, SLOT(g2hVersionSlot(struct versionST))); //版本号
    connect(hmicmd, SIGNAL(g2hDownTaskFileSignal(bool, QString, QStringList)), this, SLOT(g2hDownTaskFileSlot(bool, QString, QStringList))); //下载文件
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

    connect(titleTimer, SIGNAL(timeout()), this, SLOT(titleTimerSlot()));
    connect(messageEndTimer, SIGNAL(timeout()), this, SLOT(messageEndTimerSlot()));
    connect(blinkTimer, SIGNAL(timeout()), this, SLOT(blinkTimerSlot()));
    connect(aroundVehicleUpdateTimer, SIGNAL(timeout()), this, SLOT(aroundVehicleUpdateTimerSlot()));
    connect(collectionTimer, SIGNAL(timeout()), this, SLOT(collectionTimerSlot()));
    connect(hmicmd,SIGNAL(gatewayOfflineSignal(bool)),this,SLOT(g2hGatewayOffline(bool)));
    connect(commonUIFunction::instance(),SIGNAL(h2gSendHmiVerSignal(versionST)),hmicmd,SLOT(h2gSendHmiVerSlots(versionST)));

    connect(this,SIGNAL(addDbInfoSignal(QString)),map,SLOT(addDbInfo(QString)));
    connect(this,SIGNAL(initViewSignal(QMatrix)),map,SLOT(initView(QMatrix)));

    commonUIFunction::instance()->setHmiCMD(this->hmicmd);
    emit addDbInfoSignal(ParaParsing::instance()->paraSt.pub.mapPath+"/HDMap.db");
    //map->addDbInfo("/storage/mapinfo/HDMap.db");
    emit initViewSignal(QMatrix(0.25, 0, 0, 0.25, 0, 0));
}

uiDigger::~uiDigger()
{
    delete ui;
}

void uiDigger::on_stateSetBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlStateSetPage);//去状态设置页面
}

void uiDigger::on_loginOutBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlLoginOutPage);//去往登录设置页面
    ui->centralStackedWidget->setCurrentWidget(ui->centralLoginOutPage);//去往ID输入页面
}

void uiDigger::on_otherSetBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlOtherSetPage);//去往其他设置页面
}

void uiDigger::on_loginOutBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);//返回首界面
    ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);//返回主显示页面
}

void uiDigger::on_loginOutOkBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);//返回首界面
    ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);//返回主显示页面
}

void uiDigger::on_outWorkBackBtn_clicked()
{
    ui->centralStackedWidget->setCurrentWidget(ui->centralWorkStatePage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlUnLoadWorkSetPage);//返回首界面
}

void uiDigger::on_stateSetBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);//返回首界面
}

//状态设置返回按下
void uiDigger::on_workSetBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlDiggerWorkPage);//返回首界面
}

void uiDigger::on_faultBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlStateSetPage);//返回故障页面
    ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);//返回主显示页面
}

void uiDigger::on_faultOkBtn_clicked()
{
    //获取错误号并发送
    pubVehicleWorkStatuST statu;

//    if(faultChoice->buttonGroup->checkedButton())
    {
        statu.statuType = PUB_VEHICLE_FAULT_WORK_STATU;
/*
        switch(faultChoice->buttonGroup->checkedId())
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
*/
        emit h2gSetWorkStatuSignal(statu);

        ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);//返回主界面
        ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);//返回主显示页面
    }
}

//载货状态返回
void uiDigger::on_loadStateBackBtn_clicked()
{
    ui->centralStackedWidget->setCurrentWidget(ui->centralWorkStatePage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlUnLoadWorkSetPage);//返回状态设置页面
}

//加油返回
void uiDigger::on_addOilBackBtn_clicked()
{
    ui->centralStackedWidget->setCurrentWidget(ui->centralWorkStatePage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlUnLoadWorkSetPage);//返回状态设置页面
}

//加油确定
void uiDigger::on_addOilOkBtn_clicked()
{
    //发送加油信号
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);//返回主界面
}

void uiDigger::on_outWorkOkBtn_clicked()
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

    ui->controlStackedWidget->setCurrentWidget(0);//返回主界面
}

void uiDigger::on_faultBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlFaultCheckPage);//去往故障确定页面
    ui->centralStackedWidget->setCurrentWidget(ui->centralFaultPage);//去往故障选择页面

//    faultChoice->clearChoice();
}

//空载按键按下
void uiDigger::on_emptyLoadBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlEmptyLoadCheckPage);
}

//半载按键按下
void uiDigger::on_halfLoadBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlHaldLoadCheckPage);
}

//满载按键按下
void uiDigger::on_fullLoadBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlFullLoadCheckPage);
}

//就绪状态设置按键按下
void uiDigger::on_readyBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlReadyCheckPage);
}

//就绪状态设置确定按键按下
void uiDigger::on_readyOkBtn_clicked()
{
//发送就绪申请
    struct pubVehicleWorkStatuST statu;

    memset((char*)&statu, 0, sizeof(struct pubVehicleWorkStatuST));
    statu.statuType = PUB_VEHICLE_READY_WORK_STATU;

    emit h2gSetWorkStatuSignal(statu);

    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
}

//就绪申请页面返回按键按下
void uiDigger::on_readyBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlStateSetPage);//返回首界面
}

//跳转至延时页面
void uiDigger::on_delayBtn_clicked()
{
    delayChoice->clearChoice();

    ui->centralStackedWidget->setCurrentWidget(ui->centralDelayPage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlDelayPage);
}

//延时10分钟按键按下
void uiDigger::on_delay10Btn_clicked()
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
void uiDigger::on_delay30Btn_clicked()
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
void uiDigger::on_delay60Btn_clicked()
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
void uiDigger::on_delayBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlStateSetPage);
    ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
}

void uiDigger::on_voiceAdjustBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlVoicePage);
}

void uiDigger::on_brightAdjustBtn_clicked()
{
    commonUIFunction::instance()->on_brightAdjustBtn_clicked(ui->controlStackedWidget,
                                                             ui->controlBrightPage);
}

void uiDigger::on_editionBtn_clicked()
{
    commonUIFunction::instance()->h2gSendHmiVerCMD();
    ui->controlStackedWidget->setCurrentWidget(ui->controlEditionPage);
}

void uiDigger::on_otherSetBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);//返回首界面
}

void uiDigger::on_brightMinusBtn_clicked()
{
//亮度调低
}

void uiDigger::on_brightPlusBtn_clicked()
{
//亮度调高
}

void uiDigger::on_brightBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlOtherSetPage);
}

void uiDigger::on_editionBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlOtherSetPage);
}

void uiDigger::on_voiceBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlOtherSetPage);
}

void uiDigger::on_emptyOkBtn_clicked()
{
    //发送设置空载信号

    emit h2gSetLoadSignal(PUB_WORK_STATE_EMPTYLOAD_TYPE);

    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
}

void uiDigger::on_emptyBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlLoadStatePage);
}

void uiDigger::on_halfOkBtn_clicked()
{
    //发送设置半载信号
    emit h2gSetLoadSignal(PUB_WORK_STATE_HALFLOAD_TYPE);

    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
}

void uiDigger::on_halfBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlLoadStatePage);
}

void uiDigger::on_fullOkBtn_clicked()
{
    //发送设置满载信号
    emit h2gSetLoadSignal(PUB_WORK_STATE_FULLLOAD_TYPE);

    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
}

void uiDigger::on_fullBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlLoadStatePage);
}

void uiDigger::g2hPosSlot(struct pubVehiclePosST info)  //网关发送hmi位置信息
{
    PosInfo* pos = PosInfo::getHandle();
    pos->setPosInfo(info);
}

void uiDigger::g2hReqWorkOnStatuSlot(struct pubVehicleWorkStatuST workStatu) //平台发送网关运营状态变更
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

void uiDigger::g2hResWorkOnStatuSlot(struct pubVehicleResWorkStatuST cloudReq) //收到平台运营状态变更确认
{
    commonUIFunction::instance()->g2hResWorkOnStatuSlot(ui->messageBrowser,cloudReq);

}

void uiDigger::g2hReqWorkOptSlot(struct pubVehicleReqWorkOptST reqWork) //网关请求现场作业
{
    commonUIFunction::instance()->g2hReqWorkOptSlot(ui->messageBrowser,reqWork);
    mineInfoTimerSlot();
}

void uiDigger::g2hReqWorkRealOptSlot(struct pubVehicleReqWorkOptST reqWork) //网关请求现场作业
{
    commonUIFunction::instance()->g2hReqWorkRealOptSlot(ui->messageBrowser,reqWork);
    mineInfoTimerSlot();

}

void uiDigger::g2hResWorkOptSlot(struct pubVehicleResWorkOptST opt) //辅助车辆（网关已收到矿卡现场作业），矿卡（接收到辅助车辆相应作业信息）
{
    workStateInfo = opt;
}

void uiDigger::g2hCollWarnningSlot(struct pubVehicleCollWarnningST otherVehicle) //预警-碰撞检测
{
//预警显示
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

void uiDigger::g2hDownMapFileSlot(int flag, QString path) //接收网关下载地图命令
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

void uiDigger::g2hVersionSlot(struct versionST versiondataSt) //版本号
{
    commonUIFunction::instance()->g2hVersionSlot(ui->editionTable,versiondataSt,&versiondata);
}

void uiDigger::g2hDownTaskFileSlot(bool flag, QString path, QStringList taskList) //下载文件
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

void uiDigger::g2hStartUpSlot(struct startUpST) //网关发送开机命令
{
//信息窗口显示，网关开机
    QString str;
    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :") + QString("收到网关开机信号"));
    commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
}

void uiDigger::g2hShutDownAckSlot() //网关关机应答
{
//信息窗口显示，网管关机
    QString str;
    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :") + QString("收到网关关机信号"));
    commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
}

void uiDigger::g2hDevNameSlot(struct pubVehicleDevNameST imeiName) //网关检权获取名称后发送给hmi
{
    localImeiName = imeiName;
    quint64 width=real2pix*ParaParsing::instance()->paraSt.pub.vehicleWidth;
    quint64 length=real2pix*ParaParsing::instance()->paraSt.pub.vehicleLength;
    if(width<10)width=10;
    if(length<10)length=10;

    map->setSelfVehicle(DIGGER, QString::fromUtf8(imeiName.localname), QSize(width, length));
}

void uiDigger::g2hMatAckSlot(short mater) //物料变更
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

void uiDigger::g2hReqTaskSlot(struct pubVehicleReqTaskST reqTask) //请求任务
{
    commonUIFunction::instance()->g2hReqTaskSlot(ui->messageBrowser,reqTask);

}

void uiDigger::g2hResTaskSlot(struct pubVehicleResTaskST resTask) //接收平台响应任务
{
    commonUIFunction::instance()->g2hResTaskSlot(ui->messageBrowser,ui->titleDestLabel,resTask,taskInfo);

}

void uiDigger::g2hReqRoadSlot(struct pubVehicleReqRoadST reqRoad) //申请路权
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

void uiDigger::g2hResRoadSlot(struct pubVehicleResRoadST resRoad) //平台回复路权申请
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

void uiDigger::g2hFreeRoadSlot(struct pubVehicleReqRoadST freeRoad) //平台下发释放路权
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

void uiDigger::g2hTemporaryTaskSlot(char type) //平台下发临时调度作业
{
    commonUIFunction::instance()->g2hTemporaryTaskSlot(ui->messageBrowser,type);
}

void uiDigger::g2hMasterWarnningSlot(struct pubVehicleMasterWarnningST materWarning) //主控发送预警
{
    PosInfo* pos = PosInfo::getHandle();
    char gearState = pos->getGearState();

    if((workStateInfo.cmdType != PUB_WORK_OPT_RECOVER_TASK) && (workStateInfo.cmdType != PUB_WORK_OPT_IN_TASK) && (gearState == 0x01))//D
    {
//        ui->runDistLabel->show();
//        ui->runDistLabel->setText(QString("%1%2").arg(QString::number(materWarning.disy, 10, 0)).arg("M"));
    }
    else if((workStateInfo.cmdType == PUB_WORK_OPT_IN_TASK) && (gearState == 0x03))//R
    {
//        ui->reversDistLabel->show();
//        ui->reversDistLabel->setText(QString("%1%2").arg(QString::number(materWarning.disy, 10, 0)).arg("M"));
    }
}

void uiDigger::g2hGatewayOffline(bool status)
{
    commonUIFunction::instance()->g2hGatewayOffline(ui->messageBrowser,status);
}

void uiDigger::insertWarnningSlots(int num)
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

void uiDigger::removeWarnningSlots(int num)
{
    try
    {
        QString str;
        str.append(QDateTime::currentDateTime().toString("hh:mm:ss :")).append("移除").append(warningMap.at(num).first.data());
        commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
    }
    catch(const std::exception)
    {

    }
}

void uiDigger::titleTimerSlot()//界面头信息更新显示
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

    char gpsState;
    struct latlngheading curPos;
    pos->getCurPos(&curPos);

    gpsState = pos->getGpsState();

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
                map->setSelfVehicle(DIGGER, QString::fromUtf8(localImeiName.localname), QSize(width, length));
            }

            
        }

        if(map->realTimeItem!=NULL)
            map->realTimeItem->stackBefore(map->selfVehicle);
        map->updateVehiclePos(curPos);
        map->scene->setSceneRect(map->scene->itemsBoundingRect());
        map->view->centerOn(map->selfVehicle->typeItem);
    }

}

void uiDigger::messageEndTimerSlot()
{
    ui->messageBrowser->moveCursor(QTextCursor::Start);
}

//消息往上滑动
void uiDigger::on_messageUpBtn_clicked()
{
    ui->messageBrowser->moveCursor(QTextCursor::Up);
    messageEndTimer->start(2000);
}

//信息往下滑动
void uiDigger::on_messageDownBtn_clicked()
{
    ui->messageBrowser->moveCursor(QTextCursor::Down);
    messageEndTimer->start(2000);
}

void uiDigger::blinkTimerSlot()
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

void uiDigger::collectionTimerSlot()
{
    PosInfo* pos = PosInfo::getHandle();
    struct collectlatlngheading* collCurPos = new struct collectlatlngheading;
    pos->getCurPos(collCurPos);
    collCurPos->flag = false;
    collCurPos->height -= ParaParsing::instance()->paraSt.digger.gpsHeightofVehicle;

    map->addCollectionPoint(collCurPos);
    //map->view->update();
    //map->realTimeItem->update();

    QApplication::processEvents();
}

void uiDigger::mineInfoTimerSlot()
{
    ui->unLoadListInfoTable->setCurrentItem(NULL);
    ui->unLoadListInfoTable->clearContents();

    WorkListDeal* worklist = WorkListDeal::getHandle();

    worklist->listMutex->lock();
    for(int count = 0; count < worklist->inList->size(); count++)
    {
        ui->unLoadListInfoTable->setItem(count, 0,new QTableWidgetItem(worklist->inList->at(count).localname));
        ui->unLoadListInfoTable->setItem(count, 1,new QTableWidgetItem("无人"));

        QString strWorkState;
        switch(worklist->inList->at(count).workState)
        {
            case PUB_WORK_OPT_IN_TASK:               //网关相应主控现场作业任务调度-可驶入
            {
                strWorkState = QString::fromUtf8("申请驶入");
                break;
            }
            case PUB_WORK_OPT_FINISH_TASK:           //网关相应主控现场作业任务调度-停靠完成
            {
               strWorkState = QString::fromUtf8("停靠完成");
               break;
            }
            case PUB_WORK_OPT_STOP_TASK:             //网关相应主控现场作业任务调度-紧急停车任务
            {
               strWorkState = QString::fromUtf8("紧急停车");
               break;
            }
            case PUB_WORK_OPT_RECOVER_TASK:          //网关相应主控现场作业任务调度-停车后恢复正常行驶
            {
               strWorkState = QString::fromUtf8("恢复行车");
               break;
            }
            case PUB_WORK_OPT_MAN_TASK:              //网关相应主控现场作业任务调度-停车后人工接管
            {
               strWorkState = QString::fromUtf8("人工接管");
               break;
            }
            case PUB_WORK_OPT_RE_IN_TASK:            //网关相应主控现场作业任务调度-停车后重新退出重进
            {
               strWorkState = QString::fromUtf8("重新进入");
               break;
            }
            case PUB_WORK_OPT_UNLOAD_TASK:           //网关相应主控现场作业任务调度-允许卸料
            {
               strWorkState = QString::fromUtf8("正在卸料");
               break;
            }
            case PUB_WORK_OPT_RUN:
            {
                strWorkState = QString::fromUtf8("正在驶入");
                break;
            }
        }
        ui->unLoadListInfoTable->setItem(count, 2,new QTableWidgetItem(strWorkState));
    }
    worklist->listMutex->unlock();
}

void uiDigger::aroundVehicleUpdateTimerSlot()
{
    map->updateVehicleList(aroundVehicleList);
    aroundVehicleList.clear();
}

void uiDigger::on_collectionBackBtn_clicked()
{
    ui->centralStackedWidget->setCurrentWidget(ui->centralWorkStatePage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlDiggerWorkPage);
}

void uiDigger::on_startCollectionBtn_clicked()
{
    ParaParsing* handle = ParaParsing::instance();
    fixSet->setFixValue(handle->paraSt.digger.boundaryFixValue);
    fixSet->setLength(handle->paraSt.digger.vehicleLength);
    fixSet->setWidth(handle->paraSt.digger.vehicleWidth);
    ui->centralStackedWidget->setCurrentWidget(ui->centralFixSetPage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionValuePage);
}

void uiDigger::on_updateDataBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlOldUpdateDataPage);
}

void uiDigger::on_leftBtn_clicked()
{
    ui->suspendRestartBtn->setChecked(false);
    map->removeCollection();
    areaside = 0x01;
    sidefixvalue = 270;
    struct collectlatlngheading *collpos=new struct collectlatlngheading;
    PosInfo::getHandle()->getCurPos(collpos);
    collpos->flag = false;
    collpos->height -= ParaParsing::instance()->paraSt.digger.gpsHeightofVehicle;
    map->addCollection(collpos);

    collectionTimer->start();
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionStatePage);
}

void uiDigger::on_rightBtn_clicked()
{
    ui->suspendRestartBtn->setChecked(false);
    map->removeCollection();
    areaside = 0x02;
    sidefixvalue = 90;
    struct collectlatlngheading *collpos=new struct collectlatlngheading;
    PosInfo::getHandle()->getCurPos(collpos);
    collpos->flag = false;
    collpos->heading -= ParaParsing::instance()->paraSt.digger.gpsHeightofVehicle;
    map->addCollection(collpos);

    collectionTimer->start();
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionStatePage);
}

void uiDigger::on_suspendRestartBtn_clicked()
{

    PosInfo* pos = PosInfo::getHandle();
    struct collectlatlngheading* collData = new struct collectlatlngheading;

    if(ui->suspendRestartBtn->isChecked())
    {
        ui->suspendRestartBtnLabel->setText("继续");
        pos->getCurPos(collData);
        collData->flag = true;
        collData->height -= ParaParsing::instance()->paraSt.digger.gpsHeightofVehicle;

        map->addCollectionPoint(collData);
        collectionTimer->stop();
    }
    else
    {
        ui->suspendRestartBtnLabel->setText("暂停");
        pos->getCurPos(collData);
        collData->flag = true;
        collData->height -= ParaParsing::instance()->paraSt.digger.gpsHeightofVehicle;

        map->addCollectionPoint(collData);
        collectionTimer->start();
    }
}

void uiDigger::on_stopBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionOverPage);
}

void uiDigger::on_collectionOverBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionStatePage);
}

void uiDigger::on_saveBtn_clicked()
{
    //生成边界文件lpx
    collectionTimer->stop();

    ParaParsing* parahandle = ParaParsing::instance();
    StopListDeal* handle = StopListDeal::getHandle();

    handle->clearStopList();

    addCollectPosDataFun(&map->realTimeItem->list);
    QApplication::processEvents();

    boundaryDataFixFun(&map->realTimeItem->list, sidefixvalue, parahandle->paraSt.digger.boundaryFixValue);
    QApplication::processEvents();

    QString filename = QDateTime::currentDateTime().toString("MM-dd-hh-mm-ss.lpx");
    generateBoundaryLpxFile(QString("%1/%2").arg(ParaParsing::instance()->paraSt.pub.boundaryPath).arg(filename),map->realTimeItem->list,areaside);

    handle->generateStopList(QString("%1/%2").arg(ParaParsing::instance()->paraSt.pub.boundaryPath).arg(filename), sidefixvalue,\
                             parahandle->paraSt.digger.vehicleWidth, parahandle->paraSt.digger.vehicleLength);
    QApplication::processEvents();

    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionUpdateDataPage);

}

void uiDigger::on_deleteBtn_clicked()
{
    collectionTimer->stop();
    map->removeCollection();
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionDeleteDataPage);
}

void uiDigger::on_updateDataOkBtn_clicked()
{
    //提交边界文件
    fileUpST roadFile;
    memset((char*)(&roadFile), 0, sizeof(fileUpST));

    QDir roadDir;
    roadDir.setPath(ParaParsing::instance()->paraSt.pub.boundaryPath);
    if(roadDir.exists())
    {
        QStringList filelist = roadDir.entryList(QDir::AllEntries | QDir::NoDot | QDir::NoDotDot, QDir::Time);
        if(filelist.size() > 0)
        {
            roadFile.fileType = areaType;
            roadFile.result = 0x01;
            QString filePath = QString("%1/%2").arg(ParaParsing::instance()->paraSt.pub.boundaryPath).arg(filelist.at(0));
            QString tftpPath = QString("%1/%2").arg(ParaParsing::instance()->paraSt.pub.boundaryPath.split("/").takeLast()).arg(filelist.at(0));
            FileManager* manager = FileManager::instance();
            QString md5 = manager->fileMd5(static_cast<const QString>(filePath));
            memmove(roadFile.md5, md5.toStdString().data(), md5.length());
            memmove(roadFile.fileDir, tftpPath.toStdString().data(), tftpPath.length());
            emit h2gUpFileSignal(roadFile);
        }
    }
    map->removeCollection();
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
}

void uiDigger::on_updateDataBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionOverPage);
}

void uiDigger::on_deleteDataOkBtn_clicked()
{
    //删除采集到的数据
    map->datathread->clearRtcCollectionList();
    map->removeCollection();
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
}

void uiDigger::on_deleteDataBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionOverPage);
}

void uiDigger::on_unLoadWorkSetBtn_clicked()
{
//    ui->controlStackedWidget->setCurrentWidget(ui->controlUnLoadWorkSetPage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlDiggerWorkPage);
}

void uiDigger::on_abnormalOkBtn_clicked()
{
    if(abnormalChoice->buttonGroup->checkedButton() != NULL)
    {
        WorkListDeal* worklist = WorkListDeal::getHandle();
        struct pubVehicleDevNameStateST* firstNode = new struct pubVehicleDevNameStateST;
        bool result = false;
        if(ui->unLoadListInfoTable->currentRow() >= 0)
        {
            memset(firstNode->localname, 0, 20);
            if(ui->unLoadListInfoTable->item(ui->unLoadListInfoTable->currentRow(), 0) != NULL)
            {
                QString name = ui->unLoadListInfoTable->item(ui->unLoadListInfoTable->currentRow(), 0)->text();
                memmove(firstNode->localname, name.toStdString().data(), 20);
            }
            result = worklist->getImeiInListByName(firstNode);
        }
        else
        {
            result = worklist->getFirstInList(firstNode);
        }

        if(result && (firstNode != NULL))
        {
            QString str;
            str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));
            switch(abnormalChoice->buttonGroup->checkedId())
            {
                case 1:
                {
                    //发送人工接管
                    str.append("触发人工接管");
                    struct pubVehicleResWorkOptST opt;
                    memmove(opt.localimei, localImeiName.localimei, 15);
                    memmove(opt.localname, localImeiName.localname, 20);
                    memmove(opt.remoteimei, firstNode->localimei, 15);
                    memmove(opt.remotename, firstNode->localname, 20);
                    opt.taskType = PUB_WORK_OPT_UNLOAD_TYPE;
                    opt.cmdType = PUB_WORK_OPT_MAN_TASK;
                    opt.loadStatu = PUB_WORK_STATE_FULLLOAD_TYPE;
                    opt.materType = PUB_MATER_LOW_ORE;
                    opt.utcTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

                    emit h2gResWorkOptSignal(opt);

                    firstNode->workState = PUB_WORK_OPT_MAN_TASK;
                    worklist->insertInList(*firstNode);

                    break;
                }
                case 2:
                {
                    //发送重新停靠
                    str.append("触发重新停靠");
                    struct pubVehicleResWorkOptST opt;
                    memmove(opt.localimei, localImeiName.localimei, 15);
                    memmove(opt.localname, localImeiName.localname, 20);
                    memmove(opt.remoteimei, firstNode->localimei, 15);
                    memmove(opt.remotename, firstNode->localname, 20);
                    opt.taskType = PUB_WORK_OPT_UNLOAD_TYPE;
                    opt.cmdType = PUB_WORK_OPT_RE_IN_TASK;
                    opt.loadStatu = PUB_WORK_STATE_FULLLOAD_TYPE;
                    opt.materType = PUB_MATER_LOW_ORE;
                    opt.utcTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

                    emit h2gResWorkOptSignal(opt);

                    firstNode->workState = PUB_WORK_OPT_RE_IN_TASK;
                    worklist->insertInList(*firstNode);

                    break;
                }
                case 3:
                {
                    //发送清除队列
                    str.append("触发清除队列");
                    worklist->removeFirstInList();
                    mineInfoTimerSlot();

                    struct pubVehicleDevNameStateST imeiremoteSt;
                    memmove((char*)(&imeiremoteSt), (char*)(firstNode), sizeof(struct pubVehicleDevNameStateST));

                    if(imeiStopNumMap.contains(imeiremoteSt))
                    {
                        int stopnum = imeiStopNumMap.value(imeiremoteSt);
                        stopNumImeiMap.remove(stopnum);
                        imeiStopNumMap.remove(imeiremoteSt);
                    }
                    break;
                }
                default:
                    break;
            }

            ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
            ui->controlStackedWidget->setCurrentWidget(ui->controlUnLoadWorkSetPage);
            commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
        }

        delete firstNode;
    }
}

void uiDigger::on_abnormalBtn_clicked()
{
    abnormalChoice->clearChoice();

    ui->controlStackedWidget->setCurrentWidget(ui->controlabnormalCheckPage);
    ui->centralStackedWidget->setCurrentWidget(ui->centralAbnormalPage);
}

void uiDigger::on_abnormalBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlUnLoadWorkSetPage);
    ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
}

void uiDigger::on_unLoadRunWorkBtn_clicked()
{
    WorkListDeal* worklist = WorkListDeal::getHandle();
    struct pubVehicleDevNameStateST* firstNode = new struct pubVehicleDevNameStateST;
    bool result = false;
    QString str;
    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));

    if(ui->unLoadListInfoTable->currentRow() >= 0)
    {
        memset(firstNode->localname, 0, 20);
        if(ui->unLoadListInfoTable->item(ui->unLoadListInfoTable->currentRow(), 0) != NULL)
        {
            QString name = ui->unLoadListInfoTable->item(ui->unLoadListInfoTable->currentRow(), 0)->text();
            memmove(firstNode->localname, name.toStdString().data(), 20);
        }
        result = worklist->getImeiInListByName(firstNode);
    }
    else
    {
        result = worklist->getFirstInList(firstNode);
    }
    if(!result  || (firstNode == NULL))
    {
        return ;
    }

    //发送允许驶入
    if((firstNode->workState == PUB_WORK_OPT_IN_TASK)//当前状态为申请驶入
            ||(firstNode->workState == PUB_WORK_OPT_RECOVER_TASK))//当前状态停车后恢复行驶
    {
        StopListDeal* handle = StopListDeal::getHandle();

        struct pubVehicleResWorkOptST opt;
        if(currentStopNum <= (handle->stopList.size()))
        {

            memmove(opt.localimei, localImeiName.localimei, 15);
            memmove(opt.localname, localImeiName.localname, 20);
            memmove(opt.remoteimei, firstNode->localimei, 15);
            memmove(opt.remotename, firstNode->localname, 20);
            opt.taskType = PUB_WORK_OPT_UNLOAD_TYPE;
            opt.cmdType = PUB_WORK_OPT_IN_TASK;
            opt.loadStatu = PUB_WORK_STATE_FULLLOAD_TYPE;
            opt.materType = PUB_MATER_LOW_ORE;
            opt.lat = handle->stopList.at(currentStopNum - 1)->lat;
            opt.lng = handle->stopList.at(currentStopNum - 1)->lng;
            opt.heading = handle->stopList.at(currentStopNum - 1)->heading;
            opt.utcTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

            emit h2gResWorkOptSignal(opt);

            str.append("触发允许驶入");
            commonUIFunction::instance()->messageShow(ui->messageBrowser,str);

            struct pubVehicleDevNameStateST selectidST;
            memmove((char*)(&selectidST), (char*)(firstNode), sizeof(struct pubVehicleDevNameStateST));

            if(handle->stopList.size() < 2)
            {
                currentStopNum = 1;
                stopNumImeiMap.insert(currentStopNum, selectidST);
                imeiStopNumMap.insert(selectidST, currentStopNum);
            }
            else
            {
                for(int count = 0; count < handle->stopList.size(); count++)
                {
                    currentStopNum += 2;
                    if((currentStopNum) > handle->stopList.size())
                    {
                        if(currentStopNum % 2)
                        {
                            currentStopNum = 2;
                        }
                        else
                        {
                            currentStopNum = 1;
                        }
                    }

                    if(stopNumImeiMap.contains(currentStopNum))
                    {
                        continue;
                    }
                    else//包含当前停靠号
                    {
                        if(imeiStopNumMap.contains(selectidST))
                        {
                            int oldstopnum = imeiStopNumMap.value(selectidST);
                            imeiStopNumMap.remove(selectidST);
                            stopNumImeiMap.remove(oldstopnum);
                        }

                        stopNumImeiMap.insert(currentStopNum, selectidST);
                        imeiStopNumMap.insert(selectidST, currentStopNum);

                        break;
                    }
                }
            }

        }
        else
        {
            commonUIFunction::instance()->messageShow(ui->messageBrowser,QString("%1%2").arg(QDateTime::currentDateTime().toString("hh:mm:ss :")).arg("无可用停靠点"));
        }
    }
    else//当前状态为非申请驶入
    {
        LOG_DEBUG("The FirstNode:[%s]-name[%s] workState is not OPT_IN_TASK,is:%d",
                  firstNode->localimei,
                  firstNode->localname,
                  firstNode->workState);
        str.append("[");
        str.append(QString::fromUtf8(QByteArray(firstNode->localname, 20)));
        str.append("] ");
        str.append("当前状态不可驶入");
        commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
    }

    delete firstNode;
}

//void uiDigger::on_unLoadOverOutBtn_clicked()
//{
//    WorkListDeal* worklist = WorkListDeal::getHandle();
//    struct pubVehicleDevNameStateST* firstNode = new struct pubVehicleDevNameStateST;
//    bool result = worklist->getFirstInList(firstNode);

//    if(!result  || (firstNode == NULL))
//    {
//        return ;
//    }

//    QString str;
//    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));

//    if(firstNode->workState == PUB_WORK_OPT_UNLOAD_TASK)
//    {
//        str.append("触发卸完驶离");

//        struct pubVehicleResWorkOptST opt;
//        memmove(opt.localimei, localImeiName.localimei, 15);
//        memmove(opt.localname, localImeiName.localname, 20);
//        memmove(opt.remoteimei, firstNode->localimei, 15);
//        memmove(opt.remotename, firstNode->localname, 20);
//        opt.taskType = PUB_WORK_OPT_UNLOAD_TYPE;
//        opt.cmdType = PUB_WORK_OPT_OUT_TASK;
//        opt.loadStatu = PUB_WORK_STATE_EMPTYLOAD_TYPE;
//        opt.utcTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

//        emit h2gResWorkOptSignal(opt);

//        worklist->removeInList(*firstNode);

//        commonUIFunction::instance()->messageShow(ui->messageBrowser,str);

//        struct pubVehicleDevNameStateST imeiremoteSt;
//        memmove((char*)(&imeiremoteSt), (char*)(firstNode), sizeof(struct pubVehicleDevNameStateST));

//        if(imeiStopNumMap.contains(imeiremoteSt))
//        {
//            int stopnum = imeiStopNumMap.value(imeiremoteSt);
//            stopNumImeiMap.remove(stopnum);
//            imeiStopNumMap.remove(imeiremoteSt);
//        }
//    }
//    else
//    {
//        LOG_DEBUG("The FirstNode:[%s]-name[%s] workState is not OPT_FINISH_TASK,is:%d",
//                  firstNode->localimei,
//                  firstNode->localname,
//                  firstNode->workState);
//        str.append("[");
//        str.append(QString::fromUtf8(QByteArray(firstNode->localname, 20)));
//        str.append("] ");
//        str.append("当前状态不可驶离");

//        commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
//    }

//    delete firstNode;
//}

void uiDigger::on_emergenceBtn_clicked()
{
    WorkListDeal* worklist = WorkListDeal::getHandle();
    struct pubVehicleDevNameStateST* firstNode = new struct pubVehicleDevNameStateST;
    bool result = false;
    if(ui->unLoadListInfoTable->currentRow() >= 0)
    {
        memset(firstNode->localname, 0, 20);
        if(ui->unLoadListInfoTable->item(ui->unLoadListInfoTable->currentRow(), 0) != NULL)
        {
            QString name = ui->unLoadListInfoTable->item(ui->unLoadListInfoTable->currentRow(), 0)->text();
            memmove(firstNode->localname, name.toStdString().data(), 20);
        }
        result = worklist->getImeiInListByName(firstNode);
    }
    else
    {
        result = worklist->getFirstInList(firstNode);
    }

    if(result && (firstNode != NULL))
    {
        QString str;

        str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));

        if(ui->emergenceBtn->isChecked())
        {
            //发送紧急停车
            str.append("触发紧急停车");
            struct pubVehicleResWorkOptST opt;
            memmove(opt.localimei, localImeiName.localimei, 15);
            memmove(opt.localname, localImeiName.localname, 20);
            memmove(opt.remoteimei,firstNode->localimei, 15);
            memmove(opt.remotename, firstNode->localname, 20);
            opt.taskType = PUB_WORK_OPT_UNLOAD_TYPE;
            opt.cmdType = PUB_WORK_OPT_STOP_TASK;
            opt.loadStatu = PUB_WORK_STATE_FULLLOAD_TYPE;
            opt.materType = PUB_MATER_LOW_ORE;
            opt.utcTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

            emit h2gResWorkOptSignal(opt);

            firstNode->workState = PUB_WORK_OPT_STOP_TASK;
            worklist->insertInList(*firstNode);
        }
        else
        {
            //发送恢复行车
            str.append("触发恢复行车");
            struct pubVehicleResWorkOptST opt;
            memmove(opt.localimei, localImeiName.localimei, 15);
            memmove(opt.localname, localImeiName.localname, 20);
            memmove(opt.remoteimei, firstNode->localimei, 15);
            memmove(opt.remotename, firstNode->localname, 20);
            opt.taskType = PUB_WORK_OPT_UNLOAD_TYPE;
            opt.cmdType = PUB_WORK_OPT_RECOVER_TASK;
            opt.loadStatu = PUB_WORK_STATE_FULLLOAD_TYPE;
            opt.materType = PUB_MATER_LOW_ORE;
            opt.utcTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

            emit h2gResWorkOptSignal(opt);

            firstNode->workState = PUB_WORK_OPT_RECOVER_TASK;
            worklist->insertInList(*firstNode);
        }

        commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
    }
    else
    {
        ui->emergenceBtn->setChecked(!ui->emergenceBtn->isChecked());
    }

    delete firstNode;
}

void uiDigger::on_voiceMinusBtn_clicked()
{
    //音量调低
//    struct pubVehiclePosST info;
//    info.gLat = 40.1023023;
//    info.gLng = 116.0987327;
//    info.gHead = 80;
//    g2hPosSlot(info);
}

void uiDigger::on_voicePlusBtn_clicked()
{
    //音量调高
//    struct pubVehicleCollWarnningST otherVehicle;
//    otherVehicle.dVehicleLat = 40.1023112;
//    otherVehicle.dVehicleLng = 116.0987327;
//    otherVehicle.dVehicleHeading = 90;
//    memmove(otherVehicle.dName, QByteArray("Huang", 20), 20);
//    g2hCollWarnningSlot(otherVehicle);
}

void uiDigger::on_useCurBoundaryBtn_clicked()
{
    StopListDeal* handle = StopListDeal::getHandle();
    ParaParsing* parahandle = ParaParsing::instance();
    currentStopNum = 1;//初始化第一个停靠点

    if(handle->stopList.size() == 0)
    {
        QDir boundaryDir;
        boundaryDir.setPath(ParaParsing::instance()->paraSt.pub.boundaryPath);
        if(!boundaryDir.exists())
        {
            boundaryDir.mkdir(ParaParsing::instance()->paraSt.pub.boundaryPath);
        }

        QStringList boundaryFileList = boundaryDir.entryList(QDir::AllEntries | QDir::NoDot | QDir::NoDotDot, QDir::Time);
        if(boundaryFileList.size() == 0)
        {
            commonUIFunction::instance()->messageShow(ui->messageBrowser,QString("%1%2").arg(QDateTime::currentDateTime().toString("hh:mm:ss :")).arg("不存在可用边界"));
        }
        else
        {
            handle->generateStopList(QString("%1/%2").arg(ParaParsing::instance()->paraSt.pub.boundaryPath).arg(boundaryFileList.at(0)), sidefixvalue,\
                                     parahandle->paraSt.digger.vehicleWidth, parahandle->paraSt.digger.vehicleLength);
        }
    }

    if(handle->stopList.size() > 0)
    {
        ui->controlStackedWidget->setCurrentWidget(ui->controlUnLoadWorkSetPage);
        commonUIFunction::instance()->messageShow(ui->messageBrowser,QString("%1获取到%2个停靠点")
                                                  .arg(QDateTime::currentDateTime().toString("hh:mm:ss :"))
                                                  .arg(handle->stopList.size()));
        map->removeStopPoint(stopPointItem);
        stopPointItem=new StopPoints(&StopListDeal::getHandle()->stopList);
        map->addStopPoint(stopPointItem);

    }
    else
    {
        commonUIFunction::instance()->messageShow(ui->messageBrowser,QString("%1%2").arg(QDateTime::currentDateTime().toString("hh:mm:ss :")).arg("不存在停靠点"));
    }

}

void uiDigger::on_diggerWorkBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
}

void uiDigger::on_collectionBtn_clicked()
{
    areaType = 3;
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionPage);
}

void uiDigger::on_updateOldDataBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionPage);
}

void uiDigger::on_updateOldDataOkBtn_clicked()
{
    on_updateDataOkBtn_clicked();
}

void uiDigger::on_collectionValueOkBtn_clicked()
{
    ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlStopSidePage);

    ParaParsing* handle = ParaParsing::instance();
    handle->writedigger(fixSet->getFixValue(), fixSet->getLength(), fixSet->getWidth());
}

void uiDigger::on_collectionValueBackBtn_clicked()
{
    ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionPage);
}

void uiDigger::on_collectionUploadBtn_clicked()
{
    areaType = 2;
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionPage);
}
