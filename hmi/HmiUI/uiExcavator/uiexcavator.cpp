#include "uiexcavator.h"
#include "ui_uiexcavator.h"
#include "HmiFunction/log.h"
#include "HmiFunction/paraparsing.h"

uiExcavator::uiExcavator(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::uiExcavator)
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

    abnormalChoice = new AbnormalExcavator;
    QGridLayout *abnormalChoiceGrid = new QGridLayout;
    abnormalChoiceGrid->addWidget(abnormalChoice);
    ui->centralAbnormalPage->setLayout(abnormalChoiceGrid);

    materChoice = new MaterChoice;
    QGridLayout *materChoiceGrid = new QGridLayout;
    materChoiceGrid->addWidget(materChoice);
    ui->centralMaterChoicePage->setLayout(materChoiceGrid);

    stopHeadingChoice = new StopHeadingChoice;
    QGridLayout* stopHeadingChoiceGrid = new QGridLayout;
    stopHeadingChoiceGrid->addWidget(stopHeadingChoice);
    ui->centralStopHeadingChoicePage->setLayout(stopHeadingChoiceGrid);

    hmicmd = new CmdDealClass;
    ParaParsing* para = ParaParsing::instance();
    hmicmd->udpInit(para->paraSt.pub.localIp, para->paraSt.pub.localPort, para->paraSt.pub.remoteIp, para->paraSt.pub.remotePort);

    memset((char*)&localImeiName, 0, sizeof(localImeiName));
    stopFixHeading = 0;

    curLoad = PUB_WORK_STATE_EMPTYLOAD_TYPE;
    curMater = PUB_MATER_NULL;

    titleTimer = new QTimer;
    titleTimer->setInterval(1000);
    titleTimer->start();

    messageEndTimer = new QTimer;
    blinkTimer = new QTimer;
    blinkTimer->setInterval(500);

    collectionTimer = new QTimer;
    collectionTimer->setInterval(50);

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
    connect(hmicmd,SIGNAL(gatewayOfflineSignal(bool)),this,SLOT(g2hGatewayOffline(bool)));
    connect(commonUIFunction::instance(),SIGNAL(h2gSendHmiVerSignal(versionST)),hmicmd,SLOT(h2gSendHmiVerSlots(versionST)));

    connect(this,SIGNAL(addDbInfoSignal(QString)),map,SLOT(addDbInfo(QString)));
    connect(this,SIGNAL(initViewSignal(QMatrix)),map,SLOT(initView(QMatrix)));

    commonUIFunction::instance()->setHmiCMD(this->hmicmd);
    emit addDbInfoSignal(ParaParsing::instance()->paraSt.pub.mapPath+"/HDMap.db");
    //map->addDbInfo("/storage/mapinfo/HDMap.db");
    emit initViewSignal(QMatrix(0.25, 0, 0, 0.25, 0, 0));
}

uiExcavator::~uiExcavator()
{
    delete ui;
}

void uiExcavator::on_stateSetBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlStateSetPage);//去状态设置页面
}

void uiExcavator::on_loginOutBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlLoginOutPage);//去往登录设置页面
    ui->centralStackedWidget->setCurrentWidget(ui->centralLoginOutPage);//去往ID输入页面
}

void uiExcavator::on_otherSetBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlOtherSetPage);//去往其他设置页面
}

void uiExcavator::on_loginOutBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);//返回首界面
    ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);//返回主显示页面
}

void uiExcavator::on_loginOutOkBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);//返回首界面
    ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);//返回主显示页面
}

void uiExcavator::on_outWorkBackBtn_clicked()
{
    ui->centralStackedWidget->setCurrentWidget(ui->centralWorkStatePage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlLoadWorkSetPage);//返回首界面
}

void uiExcavator::on_stateSetBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);//返回首界面
}

//状态设置返回按下
void uiExcavator::on_workSetBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlExcavatorWorkPage);//返回首界面
}

void uiExcavator::on_faultBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlStateSetPage);//返回故障页面
    ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);//返回主显示页面
}

void uiExcavator::on_faultOkBtn_clicked()
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
void uiExcavator::on_loadStateBackBtn_clicked()
{
    ui->centralStackedWidget->setCurrentWidget(ui->centralWorkStatePage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlLoadWorkSetPage);//返回状态设置页面
}

//加油返回
void uiExcavator::on_addOilBackBtn_clicked()
{
    ui->centralStackedWidget->setCurrentWidget(ui->centralWorkStatePage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlLoadWorkSetPage);//返回状态设置页面
}

//加油确定
void uiExcavator::on_addOilOkBtn_clicked()
{
    //发送加油信号
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);//返回主界面
}

void uiExcavator::on_outWorkOkBtn_clicked()
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

void uiExcavator::on_faultBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlFaultCheckPage);//去往故障确定页面
    ui->centralStackedWidget->setCurrentWidget(ui->centralFaultPage);//去往故障选择页面

//    faultChoice->clearChoice();
}

//空载按键按下
void uiExcavator::on_emptyLoadBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlEmptyLoadCheckPage);
    curLoad = PUB_WORK_STATE_EMPTYLOAD_TYPE;
}

//半载按键按下
void uiExcavator::on_halfLoadBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlHaldLoadCheckPage);
    curLoad = PUB_WORK_STATE_HALFLOAD_TYPE;
}

//满载按键按下
void uiExcavator::on_fullLoadBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlFullLoadCheckPage);
    curLoad = PUB_WORK_STATE_FULLLOAD_TYPE;
}

//就绪状态设置按键按下
void uiExcavator::on_readyBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlReadyCheckPage);
}

//就绪状态设置确定按键按下
void uiExcavator::on_readyOkBtn_clicked()
{
//发送就绪申请
    struct pubVehicleWorkStatuST statu;

    memset((char*)&statu, 0, sizeof(struct pubVehicleWorkStatuST));
    statu.statuType = PUB_VEHICLE_READY_WORK_STATU;

    emit h2gSetWorkStatuSignal(statu);

    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
}

//就绪申请页面返回按键按下
void uiExcavator::on_readyBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlStateSetPage);//返回首界面
}

//跳转至延时页面
void uiExcavator::on_delayBtn_clicked()
{
    delayChoice->clearChoice();

    ui->centralStackedWidget->setCurrentWidget(ui->centralDelayPage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlDelayPage);
}

//延时10分钟按键按下
void uiExcavator::on_delay10Btn_clicked()
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
void uiExcavator::on_delay30Btn_clicked()
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
void uiExcavator::on_delay60Btn_clicked()
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
void uiExcavator::on_delayBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlStateSetPage);
    ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
}

void uiExcavator::on_voiceAdjustBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlVoicePage);
}

void uiExcavator::on_brightAdjustBtn_clicked()
{
    commonUIFunction::instance()->on_brightAdjustBtn_clicked(ui->controlStackedWidget,
                                                             ui->controlBrightPage);
}

void uiExcavator::on_editionBtn_clicked()
{
    commonUIFunction::instance()->h2gSendHmiVerCMD();
    ui->controlStackedWidget->setCurrentWidget(ui->controlEditionPage);
}

void uiExcavator::on_otherSetBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);//返回首界面
}

void uiExcavator::on_brightMinusBtn_clicked()
{
//亮度调低
}

void uiExcavator::on_brightPlusBtn_clicked()
{
//亮度调高
}

void uiExcavator::on_brightBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlOtherSetPage);
}

void uiExcavator::on_editionBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlOtherSetPage);
}

void uiExcavator::on_voiceBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlOtherSetPage);
}

void uiExcavator::on_emptyOkBtn_clicked()
{
    //发送设置空载信号

    emit h2gSetLoadSignal(PUB_WORK_STATE_EMPTYLOAD_TYPE);

    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
}

void uiExcavator::on_emptyBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlLoadStatePage);
}

void uiExcavator::on_halfOkBtn_clicked()
{
    //发送设置半载信号
    emit h2gSetLoadSignal(PUB_WORK_STATE_HALFLOAD_TYPE);

    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
}

void uiExcavator::on_halfBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlLoadStatePage);
}

void uiExcavator::on_fullOkBtn_clicked()
{
    //发送设置满载信号
    emit h2gSetLoadSignal(PUB_WORK_STATE_FULLLOAD_TYPE);

    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
}

void uiExcavator::on_fullBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlLoadStatePage);
}

void uiExcavator::g2hPosSlot(struct pubVehiclePosST info)  //网关发送hmi位置信息
{
    PosInfo* pos = PosInfo::getHandle();
    pos->setPosInfo(info);
}

void uiExcavator::g2hReqWorkOnStatuSlot(struct pubVehicleWorkStatuST workStatu) //平台发送网关运营状态变更
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

void uiExcavator::g2hResWorkOnStatuSlot(struct pubVehicleResWorkStatuST cloudReq) //收到平台运营状态变更确认
{
    commonUIFunction::instance()->g2hResWorkOnStatuSlot(ui->messageBrowser,cloudReq);

}

void uiExcavator::g2hReqWorkOptSlot(struct pubVehicleReqWorkOptST reqWork) //网关请求现场作业
{
    commonUIFunction::instance()->g2hReqWorkOptSlot(ui->messageBrowser,reqWork);
    mineInfoTimerSlot();

}

void uiExcavator::g2hReqWorkRealOptSlot(struct pubVehicleReqWorkOptST reqWork) //网关请求现场作业
{
    commonUIFunction::instance()->g2hReqWorkRealOptSlot(ui->messageBrowser,reqWork);
    mineInfoTimerSlot();

}

void uiExcavator::g2hResWorkOptSlot(struct pubVehicleResWorkOptST opt) //辅助车辆（网关已收到矿卡现场作业），矿卡（接收到辅助车辆相应作业信息）
{
    workStateInfo = opt;
}

void uiExcavator::g2hCollWarnningSlot(struct pubVehicleCollWarnningST otherVehicle) //预警-碰撞检测
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

void uiExcavator::g2hDownMapFileSlot(int flag, QString path) //接收网关下载地图命令
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

void uiExcavator::g2hVersionSlot(struct versionST versiondataSt) //版本号
{
    commonUIFunction::instance()->g2hVersionSlot(ui->editionTable,versiondataSt,&versiondata);
}

void uiExcavator::g2hDownTaskFileSlot(bool flag, QString path, QStringList taskList) //下载文件
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

void uiExcavator::g2hStartUpSlot(struct startUpST) //网关发送开机命令
{
//信息窗口显示，网关开机
    QString str;
    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :") + QString("收到网关开机信号"));
    commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
}

void uiExcavator::g2hShutDownAckSlot() //网关关机应答
{
//信息窗口显示，网管关机
    QString str;
    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :") + QString("收到网关关机信号"));
    commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
}

void uiExcavator::g2hDevNameSlot(struct pubVehicleDevNameST imeiName) //网关检权获取名称后发送给hmi
{
    localImeiName = imeiName;

    quint64 width=real2pix*ParaParsing::instance()->paraSt.pub.vehicleWidth;
    quint64 length=real2pix*ParaParsing::instance()->paraSt.pub.vehicleLength;
    if(width<10)width=10;
    if(length<10)length=10;

    map->setSelfVehicle(EXCAVATOR, QString::fromUtf8(imeiName.localname), QSize(width, length));
}

void uiExcavator::g2hMatAckSlot(short mater) //物料变更
{
//显示物料变更为何物
    QString str;
    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :") + QString("物料变更为"));

    switch(mater)
    {
        case PUB_MATER_CLOUD_ROCK:
        {
            str.append("白云岩");
            curMater = PUB_MATER_CLOUD_ROCK;
            break;
        }
        case PUB_MATER_HIGH_ORE:
        {
            str.append("高磁矿");
            curMater = PUB_MATER_HIGH_ORE;
            break;
        }
        case PUB_MATER_LOW_ORE:
        {
            str.append("低磁矿");
            curMater = PUB_MATER_LOW_ORE;
            break;
        }
        default:
        {
            str.append("未知");
            curMater = PUB_MATER_NULL;
            break;
        }
    }

    commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
}

void uiExcavator::g2hReqTaskSlot(struct pubVehicleReqTaskST reqTask) //请求任务
{
    commonUIFunction::instance()->g2hReqTaskSlot(ui->messageBrowser,reqTask);

}

void uiExcavator::g2hResTaskSlot(struct pubVehicleResTaskST resTask) //接收平台响应任务
{
    commonUIFunction::instance()->g2hResTaskSlot(ui->messageBrowser,ui->titleDestLabel,resTask,taskInfo);

}

void uiExcavator::g2hReqRoadSlot(struct pubVehicleReqRoadST reqRoad) //申请路权
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

void uiExcavator::g2hResRoadSlot(struct pubVehicleResRoadST resRoad) //平台回复路权申请
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

void uiExcavator::g2hFreeRoadSlot(struct pubVehicleReqRoadST freeRoad) //平台下发释放路权
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

void uiExcavator::g2hTemporaryTaskSlot(char type) //平台下发临时调度作业
{
    commonUIFunction::instance()->g2hTemporaryTaskSlot(ui->messageBrowser,type);
}

void uiExcavator::g2hMasterWarnningSlot(struct pubVehicleMasterWarnningST materWarning) //主控发送预警
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

void uiExcavator::g2hGatewayOffline(bool status)
{
    commonUIFunction::instance()->g2hGatewayOffline(ui->messageBrowser,status);
}

void uiExcavator::insertWarnningSlots(int num)
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

void uiExcavator::removeWarnningSlots(int num)
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

void uiExcavator::titleTimerSlot()//界面头信息更新显示
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
                map->setSelfVehicle(EXCAVATOR, QString::fromUtf8(localImeiName.localname), QSize(width, length));
            }


            
        }

        map->updateVehiclePos(curPos);
        map->scene->setSceneRect(map->scene->itemsBoundingRect());
        map->view->centerOn(map->selfVehicle->typeItem);
    }

}

void uiExcavator::messageEndTimerSlot()
{
    ui->messageBrowser->moveCursor(QTextCursor::Start);
}

//消息往上滑动
void uiExcavator::on_messageUpBtn_clicked()
{
    ui->messageBrowser->moveCursor(QTextCursor::Up);
    messageEndTimer->start(2000);
}

//信息往下滑动
void uiExcavator::on_messageDownBtn_clicked()
{
    ui->messageBrowser->moveCursor(QTextCursor::Down);
    messageEndTimer->start(2000);
}

void uiExcavator::blinkTimerSlot()
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

void uiExcavator::collectionTimerSlot()
{
    PosInfo* pos = PosInfo::getHandle();
    struct latlngheading curPos;
    pos->getCurPos(&curPos);

//    addPosDataFun(&collectionList, curPos);
}

void uiExcavator::aroundVehicleUpdateTimerSlot()
{
    map->updateVehicleList(aroundVehicleList);
    aroundVehicleList.clear();
}

void uiExcavator::mineInfoTimerSlot()
{
    ui->LoadListInfoTable->setCurrentItem(NULL);
    ui->LoadListInfoTable->clearContents();

    WorkListDeal* worklist = WorkListDeal::getHandle();

    worklist->listMutex->lock();

    for(int count = 0; count < worklist->inList->size(); count++)
    {
        if(count+1 > ui->LoadListInfoTable->rowCount())
        {
            ui->LoadListInfoTable->insertRow(ui->LoadListInfoTable->rowCount());
        }

        ui->LoadListInfoTable->setItem(count, 0,new QTableWidgetItem(worklist->inList->at(count).localname));
        ui->LoadListInfoTable->setItem(count, 1,new QTableWidgetItem("无人"));

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
        ui->LoadListInfoTable->setItem(count, 2,new QTableWidgetItem(strWorkState));
    }
    worklist->listMutex->unlock();
}

void uiExcavator::on_collectionBackBtn_clicked()
{
    ui->centralStackedWidget->setCurrentWidget(ui->centralWorkStatePage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlLoadWorkSetPage);
}

void uiExcavator::on_startCollectionBtn_clicked()
{
    ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionStatePage);
}

void uiExcavator::on_updateDataBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionUpdateDataPage);
}

void uiExcavator::on_leftBtn_clicked()
{
    ui->suspendRestartBtn->setChecked(false);
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionStatePage);
}

void uiExcavator::on_rightBtn_clicked()
{
    ui->suspendRestartBtn->setChecked(false);
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionStatePage);
}

void uiExcavator::on_suspendRestartBtn_clicked()
{
    if(ui->suspendRestartBtn->isChecked())
    {
        ui->suspendRestartBtnLabel->setText("继续");
        PosInfo* pos = PosInfo::getHandle();
        struct latlngheading* data = new struct latlngheading;
//        collectionList.append(data);
        collectionTimer->start();
    }
    else
    {
        ui->suspendRestartBtnLabel->setText("暂停");
        collectionTimer->stop();
    }
}

void uiExcavator::on_stopBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionOverPage);
}

void uiExcavator::on_collectionOverBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionStatePage);
}

void uiExcavator::on_saveBtn_clicked()
{
    //生成边界文件lpx
//    generateLpxFile(QString("%1/%2").arg("/tftpServer/mapinfo").arg(QDateTime::currentDateTime().toString("MM-dd_hh:mm:ss")),collectionList);
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionUpdateDataPage);
}

void uiExcavator::on_deleteBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionDeleteDataPage);
}

void uiExcavator::on_updateDataOkBtn_clicked()
{
    //提交边界文件
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
}

void uiExcavator::on_updateDataBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionOverPage);
}

void uiExcavator::on_deleteDataOkBtn_clicked()
{
    //删除采集到的数据
    collectionList.clear();
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
}

void uiExcavator::on_deleteDataBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionOverPage);
}

void uiExcavator::on_loadWorkSetBtn_clicked()
{
//    if(ParaParsing::instance()->paraSt.pub.vehicleType==9)//永顺的，显示一键停靠
//    {
//        ui->changeMaterBtnLabel->setText("一键停靠");
//    }
    ui->controlStackedWidget->setCurrentWidget(ui->controlExcavatorWorkPage);
}

void uiExcavator::on_abnormalOkBtn_clicked()
{
    if(abnormalChoice->buttonGroup->checkedButton() != NULL)
    {
        WorkListDeal* worklist = WorkListDeal::getHandle();
        struct pubVehicleDevNameStateST* firstNode = new struct pubVehicleDevNameStateST;
        bool result = false;
        if(ui->LoadListInfoTable->currentRow() >= 0)
        {
            memset(firstNode->localname, 0, 20);
            if(ui->LoadListInfoTable->item(ui->LoadListInfoTable->currentRow(), 0) != NULL)
            {
                QString name = ui->LoadListInfoTable->item(ui->LoadListInfoTable->currentRow(), 0)->text();
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
                    opt.taskType = PUB_WORK_OPT_LOAD_TYPE;
                    opt.cmdType = PUB_WORK_OPT_MAN_TASK;
                    opt.loadStatu = curLoad;
                    opt.materType = curMater;
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
                    WorkListDeal* worklist = WorkListDeal::getHandle();
                    memmove(opt.remoteimei, worklist->getInList()->first().localimei, 15);
                    memmove(opt.remotename, worklist->getInList()->first().localname, 20);
                    opt.taskType = PUB_WORK_OPT_UNLOAD_TYPE;
                    opt.cmdType = PUB_WORK_OPT_RE_IN_TASK;
                    opt.loadStatu = curLoad;
                    opt.materType = curMater;
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
                    break;
                }
                case 4:
                {
                    //半载驶离

                    if(firstNode->workState == PUB_WORK_OPT_FINISH_TASK)
                    {
                        str.append("触发半载驶离");

                        struct pubVehicleResWorkOptST opt;
                        memmove(opt.localimei, localImeiName.localimei, 15);
                        memmove(opt.localname, localImeiName.localname, 20);
                        memmove(opt.remoteimei, firstNode->localimei, 15);
                        memmove(opt.remotename, firstNode->localname, 20);
                        opt.taskType = PUB_WORK_OPT_LOAD_TYPE;
                        opt.cmdType = PUB_WORK_OPT_OUT_TASK;
                        opt.loadStatu = PUB_WORK_STATE_HALFLOAD_TYPE;
                        opt.materType = curMater;
                        opt.utcTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

                        emit h2gResWorkOptSignal(opt);

                        worklist->removeInList(*firstNode);
                    }
                    else
                    {
                        LOG_DEBUG("The FirstNode:[%s]-name[%s] workState is not OPT_FINISH_TASK,is:%d",
                                  firstNode->localimei,
                                  firstNode->localname,
                                  firstNode->workState);
                        str.append("[");
                        str.append(QString::fromUtf8(QByteArray(firstNode->localname, 20)));
                        str.append("] ");
                        str.append("当前状态不可驶离");
                    }

                    break;
                }
                case 5:
                {
                    //空载驶离
                    if(firstNode->workState == PUB_WORK_OPT_FINISH_TASK)
                    {
                        str.append("触发空载驶离");

                        struct pubVehicleResWorkOptST opt;
                        memmove(opt.localimei, localImeiName.localimei, 15);
                        memmove(opt.localname, localImeiName.localname, 20);
                        memmove(opt.remoteimei, firstNode->localimei, 15);
                        memmove(opt.remotename, firstNode->localname, 20);
                        opt.taskType = PUB_WORK_OPT_LOAD_TYPE;
                        opt.cmdType = PUB_WORK_OPT_OUT_TASK;
                        opt.loadStatu = PUB_WORK_STATE_EMPTYLOAD_TYPE;
                        opt.materType = curMater;
                        opt.utcTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

                        emit h2gResWorkOptSignal(opt);

                        worklist->removeInList(*firstNode);
                    }
                    else
                    {
                        LOG_DEBUG("The FirstNode:[%s]-name[%s] workState is not OPT_FINISH_TASK,is:%d",
                                  firstNode->localimei,
                                  firstNode->localname,
                                  firstNode->workState);
                        str.append("[");
                        str.append(QString::fromUtf8(QByteArray(firstNode->localname, 20)));
                        str.append("] ");
                        str.append("当前状态不可驶离");
                    }
                    break;
                }
                default:
                    break;
            }

            ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
            ui->controlStackedWidget->setCurrentWidget(ui->controlLoadWorkSetPage);
            commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
        }

        delete  firstNode;
    }
}

void uiExcavator::on_abnormalBtn_clicked()
{
    abnormalChoice->clearChoice();

    ui->controlStackedWidget->setCurrentWidget(ui->controlabnormalCheckPage);
    ui->centralStackedWidget->setCurrentWidget(ui->centralAbnormalPage);
}

void uiExcavator::on_abnormalBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlLoadWorkSetPage);
    ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
}

void uiExcavator::on_loadOverOutBtn_clicked()
{
    QString str;
    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));
    WorkListDeal* worklist = WorkListDeal::getHandle();
    struct pubVehicleDevNameStateST* firstNode = new struct pubVehicleDevNameStateST;
    bool result = false;
    if(ui->LoadListInfoTable->currentRow() >= 0)
    {
        memset(firstNode->localname, 0, 20);
        if(ui->LoadListInfoTable->item(ui->LoadListInfoTable->currentRow(), 0) != NULL)
        {
            QString name = ui->LoadListInfoTable->item(ui->LoadListInfoTable->currentRow(), 0)->text();
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

    if(firstNode->workState == PUB_WORK_OPT_FINISH_TASK)
    {
        str.append("触发装完驶离");

        struct pubVehicleResWorkOptST opt;
        memmove(opt.localimei, localImeiName.localimei, 15);
        memmove(opt.localname, localImeiName.localname, 20);
        memmove(opt.remoteimei, firstNode->localimei, 15);
        memmove(opt.remotename, firstNode->localname, 20);
        opt.taskType = PUB_WORK_OPT_LOAD_TYPE;
        opt.cmdType = PUB_WORK_OPT_OUT_TASK;
        opt.loadStatu = PUB_WORK_STATE_FULLLOAD_TYPE;
        opt.materType = curMater;
        opt.utcTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

        emit h2gResWorkOptSignal(opt);

        worklist->removeInList(*firstNode);
    }
    else
    {
        LOG_DEBUG("The FirstNode:[%s]-name[%s] workState is not OPT_FINISH_TASK,is:%d",
                  firstNode->localimei,
                  firstNode->localname,
                  firstNode->workState);
        str.append("[");
        str.append(QString::fromUtf8(QByteArray(firstNode->localname, 20)));
        str.append("] ");
        str.append("当前状态不可驶离");
    }
    commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
    delete  firstNode;
}

void uiExcavator::on_emergenceBtn_clicked()
{
    WorkListDeal* worklist = WorkListDeal::getHandle();
    struct pubVehicleDevNameStateST* firstNode = new struct pubVehicleDevNameStateST;
    bool result = false;
    if(ui->LoadListInfoTable->currentRow() >= 0)
    {
        memset(firstNode->localname, 0, 20);
        if(ui->LoadListInfoTable->item(ui->LoadListInfoTable->currentRow(), 0) != NULL)
        {
            QString name = ui->LoadListInfoTable->item(ui->LoadListInfoTable->currentRow(), 0)->text();
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
            memmove(opt.remoteimei, firstNode->localimei, 15);
            memmove(opt.remotename, firstNode->localname, 20);
            opt.taskType = PUB_WORK_OPT_LOAD_TYPE;
            opt.cmdType = PUB_WORK_OPT_STOP_TASK;
            opt.loadStatu = curLoad;
            opt.materType = curMater;
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
            opt.taskType = PUB_WORK_OPT_LOAD_TYPE;
            opt.cmdType = PUB_WORK_OPT_RECOVER_TASK;
            opt.loadStatu = curLoad;
            opt.materType = curMater;

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

void uiExcavator::on_changeStopBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlExcavatorHeadingFixWarningPage);
}

void uiExcavator::on_startWorkBtn_clicked()
{
    ParaParsing* para = ParaParsing::instance();
    if(para->paraSt.pub.vehicleType==2)//通用电铲
    {
        ui->quickStopBtn->hide();
        ui->quickStopBtnLabel->hide();
        //ui->verticalLayout_96->hide();
        if(curMater == PUB_MATER_NULL)//不存在物料
        {
            QString str;
            str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));
            str.append("未选择物料,请选择物料开始作业");
            commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
        }
        else if(!stopCalClass::stopOk)//指点停靠
        {
            QString str;
            str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));
            str.append("不存在合理停靠点，请选择停靠航向");
            commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
        }
        else //if(para->paraSt.excavator.fixedPoint == 0)//定点停靠
        {
            ui->controlStackedWidget->setCurrentWidget(ui->controlLoadWorkSetPage);
            QString str;
            str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));
            if (para->paraSt.excavator.fixedPoint)
            {
                str.append("定点停靠");
            }
            else
            {
                str.append("指点停靠");
            }
            str.append(QString("%1 %2 %3").arg(stopPos.lat, 0, 'f', 7).arg(stopPos.lng, 0, 'f', 7).arg(stopPos.heading));
            commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
        }
    }
	else if(para->paraSt.pub.vehicleType==9)//永顺电铲
    {
        if(!stopCalClass::stopOk)//指点停靠
        {
            QString str;
            str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));
            if(!stopCalClass::avPosOk)
                str.append("GPS信息无效，请检查");
            else
                str.append("不存在合理停靠点，请选择停靠航向");
            commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
        }
        else //if(para->paraSt.excavator.fixedPoint == 0)//定点停靠
        {
            //xuzhongkun 20200515
            curMater=PUB_MATER_CLOUD_ROCK;

            emit h2gReqMaterSignal(PUB_MATER_CLOUD_ROCK);
            //20200515 end
            ui->controlStackedWidget->setCurrentWidget(ui->controlLoadWorkSetPage);
            QString str;
            str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));
            if (para->paraSt.excavator.fixedPoint)
            {
                str.append("定点停靠");
            }
            else
            {
                str.append("指点停靠");
            }
            str.append(QString("%1 %2 %3").arg(stopPos.lat, 0, 'f', 7).arg(stopPos.lng, 0, 'f', 7).arg(stopPos.heading));
            commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
        }
    }
}

void uiExcavator::on_fixWarnningOkBtn_clicked()
{
    //stopHeadingChoice->clearValue();

    ui->controlStackedWidget->setCurrentWidget(ui->controlExcavatorStopCheckPage);
    ui->centralStackedWidget->setCurrentWidget(ui->centralStopHeadingChoicePage);
}

void uiExcavator::on_stopCheckOkBtn_clicked()
{
    //xuzhongkun获取4个参数的设置值,写入到配置文件
    ParaParsing* para = ParaParsing::instance();

    para->setPara("/conf/hmi/minetrunck.ini","/minetrunck/vehicleLength",QString::number(stopHeadingChoice->getVehicleLength(),'f',1));
    para->setPara("/conf/hmi/minetrunck.ini","/minetrunck/vehicleWidth",QString::number(stopHeadingChoice->getVehicleWidth(),'f',1));
    para->setPara("/conf/hmi/excavator.ini","/stop/stopLength",QString::number(stopHeadingChoice->getStopLength(),'f',1));
    para->setPara("/conf/hmi/excavator.ini","/stop/stopWidth",QString::number(stopHeadingChoice->getStopWidth(),'f',1));


    //获取修正角度并计算停靠经纬航向
    stopFixHeading = stopHeadingChoice->getStopCompAngle();
    if ((stopFixHeading < -90.0) || (stopFixHeading > 90.0))
    {
        stopFixHeading = 0.0;
    }

    para->setPara("/conf/hmi/excavator.ini","/stop/stopCompAngle",QString::number(stopFixHeading,'f',1));
    system("sync");
    stopCalClass::stopCalAvOffsetFun(&stopPos, stopFixHeading);
    ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlExcavatorWorkPage);
}

void uiExcavator::on_fixWarnningBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlExcavatorWorkPage);
}

void uiExcavator::on_stopCheckBackBtn_clicked()
{
    //20200513 四个参数恢复
    ParaParsing* para = ParaParsing::instance();
    para->excavatorParsing();
    para->minetrunckParsing();

    double stopLength = para->paraSt.excavator.stopLength;
    double stopWidth = para->paraSt.excavator.stopWidth;
    double stopCompAngle = para->paraSt.excavator.stopCompAngle;

    double vehicleWidth = para->paraSt.minetrunck.vehicleWidth;
    double vehicleLength = para->paraSt.minetrunck.vehicleLength;

    stopHeadingChoice->setVehicleLength(vehicleLength);
    stopHeadingChoice->setVehicleWidth(vehicleWidth);
    stopHeadingChoice->setStopLength(stopLength);
    stopHeadingChoice->setStopWidth(stopWidth);
    stopHeadingChoice->setStopCompAngle(stopCompAngle);


    //20200513 end
    ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlExcavatorWorkPage);
}

void uiExcavator::on_materChangeOkBtn_clicked()
{
//获取选中的物料类型并发送物料变更
    if(materChoice->buttonGroup->checkedButton() != NULL)
    {
        switch (materChoice->buttonGroup->checkedId())
        {
            case 1:
            {
//                curMater = PUB_MATER_LOW_ORE;
                emit h2gReqMaterSignal(PUB_MATER_CLOUD_ROCK);
                break;
            }
            case 2:
            {
//                curMater = PUB_MATER_HIGH_ORE;
                emit h2gReqMaterSignal(PUB_MATER_HIGH_ORE);
                break;
            }
            case 3:
            {
//                curMater = PUB_MATER_CLOUD_ROCK;
                emit h2gReqMaterSignal(PUB_MATER_LOW_ORE);
                break;
            }
            default:
                break;
        }
    }

    ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlExcavatorWorkPage);
}

void uiExcavator::on_materChangeBackBtn_clicked()
{
    ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlExcavatorWorkPage);
}

void uiExcavator::on_changeMaterBtn_clicked()
{
    ParaParsing* para = ParaParsing::instance();
    para->excavatorParsing();
//    if(para->paraSt.pub.vehicleType==9)//永顺电铲一键停靠
//    {
//        double stopCompAngle = para->paraSt.excavator.stopCompAngle;
//        QString str;
//        str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));
//        if(stopCalClass::stopCalAvOffsetFun(&stopPos, stopCompAngle))
//            str.append("一键停靠完成");
//        else str.append("一键停靠失败");
//        if(!stopCalClass::avPosOk)str.append(":GPS信息无效");
//        ui->messageBrowser->append(str);
//    }
//    else//通用电铲
    {
        materChoice->clearChoice();

        ui->centralStackedWidget->setCurrentWidget(ui->centralMaterChoicePage);
        ui->controlStackedWidget->setCurrentWidget(ui->controlExcavatorMaterCheckPage);
    }
}

void uiExcavator::on_workBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
}

void uiExcavator::on_loadRunWorkBtn_clicked()
{
    //发送允许驶入
    QString str;
    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));

    WorkListDeal* worklist = WorkListDeal::getHandle();
    struct pubVehicleDevNameStateST* firstNode = new struct pubVehicleDevNameStateST;
    bool result = false;
    if(ui->LoadListInfoTable->currentRow() >= 0)
    {
        memset(firstNode->localname, 0, 20);
        if(ui->LoadListInfoTable->item(ui->LoadListInfoTable->currentRow(), 0) != NULL)
        {
            QString name = ui->LoadListInfoTable->item(ui->LoadListInfoTable->currentRow(), 0)->text();
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

    if(firstNode->workState == PUB_WORK_OPT_IN_TASK)
    {

        struct pubVehicleResWorkOptST opt;
        memmove(opt.localimei, localImeiName.localimei, 15);
        memmove(opt.localname, localImeiName.localname, 20);
        memmove(opt.remoteimei, firstNode->localimei, 15);
        memmove(opt.remotename, firstNode->localname, 20);

        ParaParsing* para = ParaParsing::instance();
        para->excavatorParsing();
        if(para->paraSt.excavator.fixedPoint)
        {
            opt.lat = para->paraSt.excavator.stopLat;
            opt.lng = para->paraSt.excavator.stopLng;
            opt.heading = para->paraSt.excavator.stopHeading;
        }
        else
        {
            //stopCalClass stopcal;
            //stopcal.stopCalFun(&stopPos, stopFixHeading);

            opt.lat = stopPos.lat;
            opt.lng = stopPos.lng;
            opt.heading = stopPos.heading;
        }

        opt.taskType = PUB_WORK_OPT_LOAD_TYPE;
        opt.cmdType = PUB_WORK_OPT_IN_TASK;
        opt.loadStatu = PUB_WORK_STATE_EMPTYLOAD_TYPE;
        opt.materType = curMater;
        opt.utcTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
        emit h2gResWorkOptSignal(opt);
        str.append("触发允许驶入");
        commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
    }
    else
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

void uiExcavator::on_quickStopBtn_clicked()
{
    double stopCompAngle = ParaParsing::instance()->paraSt.excavator.stopCompAngle;
    QString str;
    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));
    if(stopCalClass::stopCalAvOffsetFun(&stopPos, stopCompAngle))
        str.append("一键停靠完成");
    else
        str.append("一键停靠失败");
    if(!stopCalClass::avPosOk)
        str.append(":GPS信息无效");
    else//一键停靠成功
    {
        //xuzhongkun 20200515
        curMater=PUB_MATER_CLOUD_ROCK;
        //20200515 end
        str.append(QDateTime::currentDateTime().toString("\nhh:mm:ss :"));
        if (ParaParsing::instance()->paraSt.excavator.fixedPoint)
        {
            str.append("定点停靠");
        }
        else
        {
            str.append("指点停靠");
        }
        str.append(QString("%1 %2 %3").arg(stopPos.lat, 0, 'f', 7).arg(stopPos.lng, 0, 'f', 7).arg(stopPos.heading));

    }
    commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
//    qDebug()<<str;
}
