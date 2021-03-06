#include "uicrush.h"
#include "ui_uicrush.h"
#include "HmiFunction/log.h"
#include "HmiFunction/paraparsing.h"

uiCrush::uiCrush(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::uiCrush)
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

    abnormalChoice = new AbnormalCrush;
    QGridLayout *abnormalChoiceGrid = new QGridLayout;
    abnormalChoiceGrid->addWidget(abnormalChoice);
    ui->centralAbnormalPage->setLayout(abnormalChoiceGrid);

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
    blinkTimer->setInterval(50);

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

    connect(this, SIGNAL(h2gSetWorkStatuSignal(pubVehicleWorkStatuST)), hmicmd, SLOT(h2gSetWorkStatuSlots(pubVehicleWorkStatuST))); //??????????????????
    connect(this, SIGNAL(h2gUpFileSignal(fileUpST)), hmicmd, SLOT(h2gUpFileSlots(fileUpST))); //????????????,fileDir???????????????0
    connect(this, SIGNAL(h2gShutDownSignal()), hmicmd, SLOT(h2gShutDownSlots()));  //????????????????????????
    connect(this, SIGNAL(h2gReqWorkOptSignal(pubVehicleReqWorkOptST)), hmicmd, SLOT(h2gReqWorkOptSlots(pubVehicleReqWorkOptST))); //??????????????????
    connect(this, SIGNAL(h2gSetLoadSignal(char)), hmicmd, SLOT(h2gSetLoadSlots(char))); //??????????????????
    connect(this, SIGNAL(h2gReqMaterSignal(short)), hmicmd, SLOT(h2gReqMaterSlots(short))); //??????????????????
    connect(this, SIGNAL(h2gResWorkOptSignal(pubVehicleResWorkOptST)), hmicmd, SLOT(h2gResWorkOptSlots(pubVehicleResWorkOptST)));//????????????????????????

    connect(hmicmd, SIGNAL(g2hPosSignal(struct pubVehiclePosST)), this, SLOT(g2hPosSlot(struct pubVehiclePosST)));  //????????????hmi????????????
    connect(hmicmd, SIGNAL(g2hReqWorkOnStatuSignal(struct pubVehicleWorkStatuST)), this, SLOT(g2hReqWorkOnStatuSlot(struct pubVehicleWorkStatuST))); //????????????????????????????????????
    connect(hmicmd, SIGNAL(g2hResWorkOnStatuSignal(struct pubVehicleResWorkStatuST)), this, SLOT(g2hResWorkOnStatuSlot(struct pubVehicleResWorkStatuST))); //????????????????????????????????????
    connect(hmicmd, SIGNAL(g2hReqWorkOptSignal(struct pubVehicleReqWorkOptST)), this, SLOT(g2hReqWorkOptSlot(struct pubVehicleReqWorkOptST))); //????????????????????????
    connect(hmicmd, SIGNAL(g2hReqWorkRealOptSignal(struct pubVehicleReqWorkOptST)), this, SLOT(g2hReqWorkRealOptSlot(struct pubVehicleReqWorkOptST))); //????????????????????????
    connect(hmicmd, SIGNAL(g2hResWorkOptSignal(struct pubVehicleResWorkOptST)), this, SLOT(g2hResWorkOptSlot(struct pubVehicleResWorkOptST))); //?????????????????????????????????????????????????????????????????????????????????????????????????????????
    connect(hmicmd, SIGNAL(g2hCollWarnningSignal(struct pubVehicleCollWarnningST)), this, SLOT(g2hCollWarnningSlot(struct pubVehicleCollWarnningST))); //??????
    connect(hmicmd, SIGNAL(g2hDownMapFileSignal(int, QString)), this, SLOT(g2hDownMapFileSlot(int, QString))); //??????????????????????????????
    connect(hmicmd, SIGNAL(g2hVersionSignal(struct versionST)), this, SLOT(g2hVersionSlot(struct versionST))); //?????????
    connect(hmicmd, SIGNAL(g2hDownTaskFileSignal(bool, QString, QStringList)), this, SLOT(g2hDownTaskFileSlot(bool, QString, QStringList))); //????????????
    connect(hmicmd, SIGNAL(g2hStartUpSignal(struct startUpST)), this, SLOT(g2hStartUpSlot(struct startUpST))); //????????????????????????
    connect(hmicmd, SIGNAL(g2hShutDownAckSignal()), this, SLOT(g2hShutDownAckSlot())); //??????????????????
    connect(hmicmd, SIGNAL(g2hDevNameSignal(struct pubVehicleDevNameST)), this, SLOT(g2hDevNameSlot(struct pubVehicleDevNameST))); //????????????????????????????????????hmi
    connect(hmicmd, SIGNAL(g2hMatAckSignal(short)), this, SLOT(g2hMatAckSlot(short))); //????????????
    connect(hmicmd, SIGNAL(g2hReqTaskSignal(struct pubVehicleReqTaskST )), this, SLOT(g2hReqTaskSlot(struct pubVehicleReqTaskST ))); //????????????
    connect(hmicmd, SIGNAL(g2hResTaskSignal(struct pubVehicleResTaskST )), this, SLOT(g2hResTaskSlot(struct pubVehicleResTaskST ))); //????????????????????????
    connect(hmicmd, SIGNAL(g2hReqRoadSignal(struct pubVehicleReqRoadST )), this, SLOT(g2hReqRoadSlot(struct pubVehicleReqRoadST ))); //????????????
    connect(hmicmd, SIGNAL(g2hResRoadSignal(struct pubVehicleResRoadST )), this, SLOT(g2hResRoadSlot(struct pubVehicleResRoadST ))); //????????????????????????
    connect(hmicmd, SIGNAL(g2hFreeRoadSignal(struct pubVehicleReqRoadST )), this, SLOT(g2hFreeRoadSlot(struct pubVehicleReqRoadST ))); //????????????????????????
    connect(hmicmd, SIGNAL(g2hTemporaryTaskSignal(char )), this, SLOT( g2hTemporaryTaskSlot(char ))); //??????????????????????????????
    connect(hmicmd, SIGNAL(g2hMasterWarnningSignal(struct pubVehicleMasterWarnningST)), this, SLOT( g2hMasterWarnningSlot(struct pubVehicleMasterWarnningST))); //??????????????????
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

    ui->emergenceBtn->setCheckable(false);
    //??????????????????
    if(ParaParsing::instance()->paraSt.pub.vehicleType==7)
    {
        ui->crushInfoTable->item(0, 0)->setText("???????????????");
        ui->crushInfoTable->item(0, 1)->setText("4???");
        ui->crushInfoTable->item(1, 0)->setText("??????");
        ui->crushInfoTable->item(1, 1)->setText("??????");
        ui->crushInfoTable->item(2, 0)->setText("??????");
        ui->crushInfoTable->item(2, 1)->setText("??????");
        ui->crushInfoLabel->setText("???????????????");
        ui->crushLabel->setStyleSheet("border-image:url(:/image/resource/parkingLabel.png);");

        ui->unLoadOverOutBtn->hide();
        ui->unLoadOverOutBtnLabel->hide();

        ui->stateSetBtn->hide();
        ui->stateSetBtnLabel->hide();

        ui->unLoadRunWorkBtn->setCheckable(false);

        ui->unLoadWorkSetBtnLabel->setText("????????????");
        ui->unLoadWorkSetBtn->setStyleSheet("#unLoadWorkSetBtn{border-image:url(:/image/resource/parkingBtn.png);} "
                                            "#unLoadWorkSetBtn:pressed{border-image:url(:/image/resource/parkingPressBtn.png);}");

        //???????????????
        parkingList.clear();
        if(ParaParsing::instance()->paraSt.parking.stop1Lat>0.000001)//????????????
        {
            latlngheading *stop1=new latlngheading;
            stop1->lat=ParaParsing::instance()->paraSt.parking.stop1Lat;
            stop1->lng=ParaParsing::instance()->paraSt.parking.stop1Lng;
            stop1->heading=ParaParsing::instance()->paraSt.parking.stop1Heading;
            parkingList.append(stop1);
        }

        if(ParaParsing::instance()->paraSt.parking.stop2Lat>0.000001)//????????????
        {
            latlngheading *stop2=new latlngheading;
            stop2->lat=ParaParsing::instance()->paraSt.parking.stop2Lat;
            stop2->lng=ParaParsing::instance()->paraSt.parking.stop2Lng;
            stop2->heading=ParaParsing::instance()->paraSt.parking.stop2Heading;
            parkingList.append(stop2);
        }

        if(ParaParsing::instance()->paraSt.parking.stop3Lat>0.000001)//????????????
        {
            latlngheading *stop3=new latlngheading;
            stop3->lat=ParaParsing::instance()->paraSt.parking.stop3Lat;
            stop3->lng=ParaParsing::instance()->paraSt.parking.stop3Lng;
            stop3->heading=ParaParsing::instance()->paraSt.parking.stop3Heading;
            parkingList.append(stop3);
        }

        if(ParaParsing::instance()->paraSt.parking.stop4Lat>0.000001)//????????????
        {
            latlngheading *stop4=new latlngheading;
            stop4->lat=ParaParsing::instance()->paraSt.parking.stop4Lat;
            stop4->lng=ParaParsing::instance()->paraSt.parking.stop4Lng;
            stop4->heading=ParaParsing::instance()->paraSt.parking.stop4Heading;
            parkingList.append(stop4);
        }

        parkingPointItem = new StopPoints(&parkingList);
        parkingPointItem->setPointOffset(QPointF(0,0));

        map->addStopPoint(parkingPointItem);
    }
}

uiCrush::~uiCrush()
{
    delete ui;
}

void uiCrush::on_stateSetBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlStateSetPage);//?????????????????????
}

void uiCrush::on_loginOutBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlLoginOutPage);//????????????????????????
    ui->centralStackedWidget->setCurrentWidget(ui->centralLoginOutPage);//??????ID????????????
}

void uiCrush::on_otherSetBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlOtherSetPage);//????????????????????????
}

void uiCrush::on_loginOutBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);//???????????????
    ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);//?????????????????????
}

void uiCrush::on_loginOutOkBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);//???????????????
    ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);//?????????????????????
}

void uiCrush::on_outWorkBackBtn_clicked()
{
    ui->centralStackedWidget->setCurrentWidget(ui->centralWorkStatePage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlUnLoadWorkSetPage);//???????????????
}

void uiCrush::on_stateSetBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);//???????????????
}

//????????????????????????
//void uiCrush::on_workSetOkBtn_clicked()
//{
//    if(workChoice->buttonGroup->checkedButton() != NULL)
//    {
//        switch(workChoice->buttonGroup->checkedId())
//        {
//            case 1://????????????
//            {
//                break;
//            }
//            case 2://??????????????????
//            {
//                ui->controlStackedWidget->setCurrentWidget(ui->controlLoadStatePage);
//                ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
//                break;
//            }
//            case 3://????????????
//            {
//                ui->controlStackedWidget->setCurrentWidget(ui->controlAddOilCheckPage);
//                ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
//                break;
//            }
//            case 4://??????????????????
//            {
//                ui->controlStackedWidget->setCurrentWidget(17);
//                ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
//                break;
//            }
//            case 5://??????????????????
//            {
//                ui->controlStackedWidget->setCurrentWidget(5);//??????????????????????????????
//                ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
//                break;
//            }
//            case 6://????????????
//            {
//                break;
//            }
//            case 7://????????????
//            {
//                break;
//            }
//            default:
//                break;
//        }

//        workChoice->clearChoice();

//    }
//}

//????????????????????????
void uiCrush::on_workSetBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);//???????????????
}

void uiCrush::on_faultBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlStateSetPage);//??????????????????
    ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);//?????????????????????
}

void uiCrush::on_faultOkBtn_clicked()
{
    //????????????????????????
    pubVehicleWorkStatuST statu;

//    if(faultChoice->buttonGroup->checkedButton())
    {
        statu.statuType = PUB_VEHICLE_FAULT_WORK_STATU;

        emit h2gSetWorkStatuSignal(statu);

        ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);//???????????????
        ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);//?????????????????????
    }
}

//??????????????????
void uiCrush::on_loadStateBackBtn_clicked()
{
    ui->centralStackedWidget->setCurrentWidget(ui->centralWorkStatePage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlUnLoadWorkSetPage);//????????????????????????
}

//????????????
void uiCrush::on_addOilBackBtn_clicked()
{
    ui->centralStackedWidget->setCurrentWidget(ui->centralWorkStatePage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlUnLoadWorkSetPage);//????????????????????????
}

//????????????
void uiCrush::on_addOilOkBtn_clicked()
{
    //??????????????????
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);//???????????????
}

void uiCrush::on_outWorkOkBtn_clicked()
{
    //????????????????????????
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

    ui->controlStackedWidget->setCurrentWidget(0);//???????????????
}

void uiCrush::on_faultBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlFaultCheckPage);//????????????????????????
    ui->centralStackedWidget->setCurrentWidget(ui->centralFaultPage);//????????????????????????

//    faultChoice->clearChoice();
}

//??????????????????
void uiCrush::on_emptyLoadBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlEmptyLoadCheckPage);
}

//??????????????????
void uiCrush::on_halfLoadBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlHaldLoadCheckPage);
}

//??????????????????
void uiCrush::on_fullLoadBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlFullLoadCheckPage);
}

//??????????????????????????????
void uiCrush::on_readyBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlReadyCheckPage);
}

//????????????????????????????????????
void uiCrush::on_readyOkBtn_clicked()
{
//??????????????????
    struct pubVehicleWorkStatuST statu;

    memset((char*)&statu, 0, sizeof(struct pubVehicleWorkStatuST));
    statu.statuType = PUB_VEHICLE_READY_WORK_STATU;

    emit h2gSetWorkStatuSignal(statu);

    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
}

//????????????????????????????????????
void uiCrush::on_readyBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlStateSetPage);//???????????????
}

//?????????????????????
void uiCrush::on_delayBtn_clicked()
{
    delayChoice->clearChoice();

    ui->centralStackedWidget->setCurrentWidget(ui->centralDelayPage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlDelayPage);
}

//??????10??????????????????
void uiCrush::on_delay10Btn_clicked()
{
//????????????10????????????
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

//??????30??????????????????
void uiCrush::on_delay30Btn_clicked()
{
//????????????30????????????
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

//??????60??????????????????
void uiCrush::on_delay60Btn_clicked()
{
//????????????60????????????
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

//??????????????????
void uiCrush::on_delayBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlStateSetPage);
    ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
}

void uiCrush::on_voiceAdjustBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlVoicePage);
}

void uiCrush::on_brightAdjustBtn_clicked()
{
    commonUIFunction::instance()->on_brightAdjustBtn_clicked(ui->controlStackedWidget,
                                                             ui->controlBrightPage);
}

void uiCrush::on_editionBtn_clicked()
{
    commonUIFunction::instance()->h2gSendHmiVerCMD();
    ui->controlStackedWidget->setCurrentWidget(ui->controlEditionPage);
}

void uiCrush::on_otherSetBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);//???????????????
}

void uiCrush::on_brightMinusBtn_clicked()
{
//????????????
}

void uiCrush::on_brightPlusBtn_clicked()
{
//????????????
}

void uiCrush::on_brightBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlOtherSetPage);
}

void uiCrush::on_editionBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlOtherSetPage);
}

void uiCrush::on_voiceBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlOtherSetPage);
}

void uiCrush::on_emptyOkBtn_clicked()
{
    //????????????????????????

    emit h2gSetLoadSignal(PUB_WORK_STATE_EMPTYLOAD_TYPE);

    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
}

void uiCrush::on_emptyBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlLoadStatePage);
}

void uiCrush::on_halfOkBtn_clicked()
{
    //????????????????????????
    emit h2gSetLoadSignal(PUB_WORK_STATE_HALFLOAD_TYPE);

    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
}

void uiCrush::on_halfBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlLoadStatePage);
}

void uiCrush::on_fullOkBtn_clicked()
{
    //????????????????????????
    emit h2gSetLoadSignal(PUB_WORK_STATE_FULLLOAD_TYPE);

    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
}

void uiCrush::on_fullBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlLoadStatePage);
}

void uiCrush::g2hPosSlot(struct pubVehiclePosST info)  //????????????hmi????????????
{
    PosInfo* pos = PosInfo::getHandle();
    pos->setPosInfo(info);
}

void uiCrush::g2hReqWorkOnStatuSlot(struct pubVehicleWorkStatuST workStatu) //????????????????????????????????????
{
//?????????????????????????????????????????????
    QString str;
    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));

    str.append("?????????????????????");

    switch(workStatu.statuType)
    {
        case PUB_VEHICLE_READY_WORK_STATU:
        {
            str.append("??????");
            break;
        }
        case PUB_VEHICLE_DEALY_WORK_STATU:
        {
            str.append("??????");
            break;
        }
        case PUB_VEHICLE_FAULT_WORK_STATU:
        {
            str.append("??????");
            break;
        }
        case PUB_VEHICLE_BACKUP_WORK_STATU:
        {
            str.append("??????");
            break;
        }
        default:
        {
            str.append("??????");
            break;
        }
    }

    commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
}

void uiCrush::g2hResWorkOnStatuSlot(struct pubVehicleResWorkStatuST cloudReq) //????????????????????????????????????
{
    commonUIFunction::instance()->g2hResWorkOnStatuSlot(ui->messageBrowser,cloudReq);

}

void uiCrush::g2hReqWorkOptSlot(struct pubVehicleReqWorkOptST reqWork) //????????????????????????
{
    commonUIFunction::instance()->g2hReqWorkOptSlot(ui->messageBrowser,reqWork);
    mineInfoTimerSlot();
}

void uiCrush::g2hReqWorkRealOptSlot(struct pubVehicleReqWorkOptST reqWork) //????????????????????????
{
    commonUIFunction::instance()->g2hReqWorkRealOptSlot(ui->messageBrowser,reqWork);
    mineInfoTimerSlot();
}

void uiCrush::g2hResWorkOptSlot(struct pubVehicleResWorkOptST opt) //?????????????????????????????????????????????????????????????????????????????????????????????????????????
{
    workStateInfo = opt;
}

void uiCrush::g2hCollWarnningSlot(struct pubVehicleCollWarnningST otherVehicle) //??????-????????????
{
//????????????
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

void uiCrush::g2hDownMapFileSlot(int flag, QString path) //??????????????????????????????
{
    //???????????????????????????????????????????????????????????????
    QString str;
    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));

    LOG_DEBUG("g2hDownMapFileSlot flag[%d], path[%s]", flag, path.toStdString().data());
    if(flag != MapUpdateMode_FASLE)
    {
        LOG_DEBUG("%s", path.toStdString().data());

        str.append("??????????????????");
        str.append(QString("%1%2").arg(",????????????:").arg(path.split("/").last()));

        if(flag == MapUpdateMode_NEW_FILE)
        {
            emit addDbInfoSignal(ParaParsing::instance()->paraSt.pub.mapPath+"/HDMap.db");
            //map->addDbInfo("/storage/mapinfo/HDMap.db");
        }
    }
    else
    {
        str.append("??????????????????");
    }

    commonUIFunction::instance()->messageShow(ui->messageBrowser,str);

}

void uiCrush::g2hVersionSlot(struct versionST versiondataSt) //?????????
{
    commonUIFunction::instance()->g2hVersionSlot(ui->editionTable,versiondataSt,&versiondata);

}

void uiCrush::g2hDownTaskFileSlot(bool flag, QString path, QStringList taskList) //????????????
{
    //?????????????????????????????????????????????????????????????????????
    QString str;
    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));

    if(flag)
    {
        str.append("????????????????????????");
        str.append(QString("%1%2").arg(",????????????:").arg(path.split("/").last()));


        if(!taskList.isEmpty())
        {
            map->addTaskInfo(taskList);
        }
    }
    else
    {
        str.append("????????????????????????");
    }

    commonUIFunction::instance()->messageShow(ui->messageBrowser,str);

}

void uiCrush::g2hStartUpSlot(struct startUpST) //????????????????????????
{
//?????????????????????????????????
    QString str;
    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :") + QString("????????????????????????"));
    commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
}

void uiCrush::g2hShutDownAckSlot() //??????????????????
{
//?????????????????????????????????
    QString str;
    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :") + QString("????????????????????????"));
    commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
}

void uiCrush::g2hDevNameSlot(struct pubVehicleDevNameST imeiName) //????????????????????????????????????hmi
{
    localImeiName = imeiName;
    quint64 width=real2pix*ParaParsing::instance()->paraSt.pub.vehicleWidth;
    quint64 length=real2pix*ParaParsing::instance()->paraSt.pub.vehicleLength;
    if(width<10)width=10;
    if(length<10)length=10;

    if(ParaParsing::instance()->paraSt.pub.vehicleType==8)
        map->setSelfVehicle(CRUSHER, imeiName.localname, QSize(width, length));
    else if(ParaParsing::instance()->paraSt.pub.vehicleType==7)
        map->setSelfVehicle(PARKING,imeiName.localname, QSize(width, length));
}

void uiCrush::g2hMatAckSlot(short mater) //????????????
{
//???????????????????????????
    QString str;
    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :") + QString("???????????????"));

    switch(mater)
    {
        case PUB_MATER_CLOUD_ROCK:
        {
            str.append("?????????");
            break;
        }
        case PUB_MATER_HIGH_ORE:
        {
            str.append("?????????");
            break;
        }
        case PUB_MATER_LOW_ORE:
        {
            str.append("?????????");
            break;
        }
        default:
        {
            str.append("??????");
            break;
        }
    }

    commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
}

void uiCrush::g2hReqTaskSlot(struct pubVehicleReqTaskST reqTask) //????????????
{
    commonUIFunction::instance()->g2hReqTaskSlot(ui->messageBrowser,reqTask);

}

void uiCrush::g2hResTaskSlot(struct pubVehicleResTaskST resTask) //????????????????????????
{
    commonUIFunction::instance()->g2hResTaskSlot(ui->messageBrowser,ui->titleDestLabel,resTask,taskInfo);

}

void uiCrush::g2hReqRoadSlot(struct pubVehicleReqRoadST reqRoad) //????????????
{
//??????????????????????????????
    QString str;

    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));
    str.append(QString::number(reqRoad.roadNum) + QString("???"));

    if(reqRoad.roadType== PUB_VEHICLE_ROAD_TYPE_CROSSING)
    {
        str.append("??????");
    }
    else if(reqRoad.roadType== PUB_VEHICLEROAD_TYPE_ONEWAY)
    {
        str.append("?????????");
    }

    if(reqRoad.roadDir == PUB_VEHICLEROAD_DIR_LOAD)
    {
        str.append("??????");
    }
    else if(reqRoad.roadDir == PUB_VEHICLEROAD_DOR_UNLOAD)
    {
        str.append("??????");
    }

    str.append("????????????");

    commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
}

void uiCrush::g2hResRoadSlot(struct pubVehicleResRoadST resRoad) //????????????????????????
{
//????????????????????????????????????
    QString str;

    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));
    str.append(QString::number(resRoad.roadNum) + QString("???"));

    if(resRoad.roadType== PUB_VEHICLE_ROAD_TYPE_CROSSING)
    {
        str.append("??????");
    }
    else if(resRoad.roadType== PUB_VEHICLEROAD_TYPE_ONEWAY)
    {
        str.append("?????????");
    }

    if(resRoad.roadDir == PUB_VEHICLEROAD_DIR_LOAD)
    {
        str.append("??????");
    }
    else if(resRoad.roadDir == PUB_VEHICLEROAD_DOR_UNLOAD)
    {
        str.append("??????");
    }

    if(resRoad.result == 1)
    {
        str.append("????????????");
    }
    else if(resRoad.result == 2)
    {
        str.append("????????????");
    }
    else
    {
        str.append("??????????????????");
    }

    commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
}

void uiCrush::g2hFreeRoadSlot(struct pubVehicleReqRoadST freeRoad) //????????????????????????
{
//????????????????????????????????????????????????
    QString str;

    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));
    str.append(QString::number(freeRoad.roadNum) + QString("???"));

    if(freeRoad.roadType== PUB_VEHICLE_ROAD_TYPE_CROSSING)
    {
        str.append("??????");
    }
    else if(freeRoad.roadType== PUB_VEHICLEROAD_TYPE_ONEWAY)
    {
        str.append("?????????");
    }

    if(freeRoad.roadDir == PUB_VEHICLEROAD_DIR_LOAD)
    {
        str.append("??????");
    }
    else if(freeRoad.roadDir == PUB_VEHICLEROAD_DOR_UNLOAD)
    {
        str.append("??????");
    }

    str.append("????????????");

    commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
}

void uiCrush::g2hTemporaryTaskSlot(char type) //??????????????????????????????
{
    commonUIFunction::instance()->g2hTemporaryTaskSlot(ui->messageBrowser,type);
}

void uiCrush::g2hMasterWarnningSlot(struct pubVehicleMasterWarnningST materWarning) //??????????????????
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

void uiCrush::g2hGatewayOffline(bool status)
{
    commonUIFunction::instance()->g2hGatewayOffline(ui->messageBrowser,status);
}

void uiCrush::insertWarnningSlots(int num)
{
    try
    {
        QString str;
        str.append(QDateTime::currentDateTime().toString("hh:mm:ss :")).append("??????").append(warningMap.at(num).first.data());
        commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
    }
    catch(const std::exception)
    {

    }
}

void uiCrush::removeWarnningSlots(int num)
{
    try
    {
        QString str;
        str.append(QDateTime::currentDateTime().toString("hh:mm:ss :")).append("??????").append(warningMap.at(num).first.data());
        commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
    }
    catch(const std::exception)
    {

    }
}

void uiCrush::titleTimerSlot()//???????????????????????????
{
    ui->titleTimeLabel->setText(QDateTime::currentDateTime().toString("yyyy/MM/dd - hh:mm:ss"));

    PosInfo* pos = PosInfo::getHandle();
    pos->getWorkStatu();

    QString stateString;
    stateString.append("???????????????");

    u8 workStatus=pos->getWorkStatu();
    if(ParaParsing::instance()->paraSt.pub.vehicleType==7)//?????????????????????
        workStatus=PUB_VEHICLE_READY_WORK_STATU;
    switch(workStatus)
    {
        case PUB_VEHICLE_READY_WORK_STATU:
        {
            stateString.append("??????");
            break;
        }
        case PUB_VEHICLE_DEALY_WORK_STATU:
        {
            stateString.append("??????");
            break;
        }
        case PUB_VEHICLE_FAULT_WORK_STATU:
        {
            stateString.append("??????");
            break;
        }
        case PUB_VEHICLE_BACKUP_WORK_STATU:
        {
            stateString.append("??????");
            break;
        }
        case PUB_VEHICLE_NA_WORK_STATU:
        {
            stateString.append("??????");
            break;
        }
        default:
        {
            stateString.append("??????");
            break;
        }
    }

    if((pos->getWorkStatu() == PUB_VEHICLE_DEALY_WORK_STATU))
    {
        switch(pos->getDelay())
        {
            case PUB_FAULT_PWR:
            {
                stateString.append(" - ????????????");
                break;
            }
            case PUB_FAULT_CABLE:
            {
                stateString.append(" - ????????????");
                break;
            }
            case PUB_FAULT_CABLE_BROKEN:
            {
                stateString.append(" - ?????????");
                break;
            }
            case PUB_FAULT_OTHER:
            {
                stateString.append(" - ????????????");
                break;
            }
            case PUB_DELAY_STOP:
            {
                stateString.append(" - ????????????");
                break;
            }
            case PUB_DELAY_SCHEDULE_FALUT:
            {
                stateString.append(" - ????????????");
                break;
            }
            case PUB_DELAY_ERROR_ROAD:
            {
                stateString.append(" - ????????????");
                break;
            }
            case PUB_DELAY_AVOIDBLAST:
            {
                stateString.append(" - ??????");
                break;
            }
            case PUB_DELAY_OIL:
            {
                stateString.append(" - ????????????");
                break;
            }
            case PUB_DELAY_REMOTE:
            {
                stateString.append(" - ????????????");
                break;
            }
            case PUB_DELAY_EMERGENCY:
            {
                stateString.append(" - ????????????");
                break;
            }
            case PUB_DELAY_OVER:
            {
                stateString.append(" - ????????????");
                break;
            }
            case PUB_DELAY_CHANGE:
            {
                stateString.append(" - ???????????????");
                break;
            }
            case PUB_DELAY_COLD:
            {
                stateString.append(" - ????????????");
                break;
            }
            case PUB_DELAY_HITCH:
            {
                stateString.append(" - ????????????");
                break;
            }
            case PUB_DELAY_WAIT:
            {
                stateString.append(" - ????????????");
                break;
            }
//            case PUB_DELAY_EXCAVATOR_CHECK:
//            {
//                stateString.append(" - ??????");
//                break;
//            }
//            case PUB_DELAY_EXCAVATOR_CHANGE:
//            {
//                stateString.append(" - ?????????");
//                break;
//            }
//            case PUB_DELAY_EXCAVATOR_FOOD:
//            {
//                stateString.append(" - ?????????");
//                break;
//            }
            case PUB_DELAY_EXCAVATOR_PUSH:
            {
                stateString.append(" - ?????????");
                break;
            }
            case PUB_DELAY_EXCAVATOR_AVOIDBLAST:
            {
                stateString.append(" - ??????");
                break;
            }
            case PUB_DELAY_EXCAVATOR_SHORT:
            {
                stateString.append(" - ????????????");
                break;
            }
            case PUB_DELAY_EXCAVATOR_SWEEP:
            {
                stateString.append(" - ??????");
                break;
            }
            case PUB_DELAY_EXCAVATOR_CARGO:
            {
                stateString.append(" - ??????");
                break;
            }
            case PUB_DELAY_EXCAVATOR_DISPLACE:
            {
                stateString.append(" - ??????");
                break;
            }
            case PUB_DELAY_EXCAVATOR_DIG:
            {
                stateString.append(" - ?????????");
                break;
            }
            case PUB_DELAY_EXCAVATOR_POWER:
            {
                stateString.append(" - ??????");
                break;
            }
            case PUB_DELAY_EXCAVATOR_DROP:
            {
                stateString.append(" - ?????????");
                break;
            }
            case PUB_DELAY_EXCAVATOR_PUMP:
            {
                stateString.append(" - ?????????");
                break;
            }
            case PUB_DELAY_EXCAVATOR_CABLE:
            {
                stateString.append(" - ?????????");
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
                stateString.append(" - ????????????");
                break;
            }
            case PUB_FAULT_CABLE:
            {
                stateString.append(" - ????????????");
                break;
            }
            case PUB_FAULT_CABLE_BROKEN:
            {
                stateString.append(" - ?????????");
                break;
            }
            case PUB_FAULT_OTHER:
            {
                stateString.append(" - ????????????");
                break;
            }
            case PUB_DELAY_STOP:
            {
                stateString.append(" - ????????????");
                break;
            }
            case PUB_DELAY_SCHEDULE_FALUT:
            {
                stateString.append(" - ????????????");
                break;
            }
            case PUB_DELAY_ERROR_ROAD:
            {
                stateString.append(" - ????????????");
                break;
            }
            case PUB_DELAY_AVOIDBLAST:
            {
                stateString.append(" - ??????");
                break;
            }
            case PUB_DELAY_OIL:
            {
                stateString.append(" - ????????????");
                break;
            }
            case PUB_DELAY_REMOTE:
            {
                stateString.append(" - ????????????");
                break;
            }
            case PUB_DELAY_EMERGENCY:
            {
                stateString.append(" - ????????????");
                break;
            }
            case PUB_DELAY_OVER:
            {
                stateString.append(" - ????????????");
                break;
            }
            case PUB_DELAY_CHANGE:
            {
                stateString.append(" - ???????????????");
                break;
            }
            case PUB_DELAY_COLD:
            {
                stateString.append(" - ????????????");
                break;
            }
            case PUB_DELAY_HITCH:
            {
                stateString.append(" - ????????????");
                break;
            }
            case PUB_DELAY_WAIT:
            {
                stateString.append(" - ????????????");
                break;
            }
//            case PUB_DELAY_EXCAVATOR_CHECK:
//            {
//                stateString.append(" - ??????");
//                break;
//            }
//            case PUB_DELAY_EXCAVATOR_CHANGE:
//            {
//                stateString.append(" - ?????????");
//                break;
//            }
//            case PUB_DELAY_EXCAVATOR_FOOD:
//            {
//                stateString.append(" - ?????????");
//                break;
//            }
            case PUB_DELAY_EXCAVATOR_PUSH:
            {
                stateString.append(" - ?????????");
                break;
            }
            case PUB_DELAY_EXCAVATOR_AVOIDBLAST:
            {
                stateString.append(" - ??????");
                break;
            }
            case PUB_DELAY_EXCAVATOR_SHORT:
            {
                stateString.append(" - ????????????");
                break;
            }
            case PUB_DELAY_EXCAVATOR_SWEEP:
            {
                stateString.append(" - ??????");
                break;
            }
            case PUB_DELAY_EXCAVATOR_CARGO:
            {
                stateString.append(" - ??????");
                break;
            }
            case PUB_DELAY_EXCAVATOR_DISPLACE:
            {
                stateString.append(" - ??????");
                break;
            }
            case PUB_DELAY_EXCAVATOR_DIG:
            {
                stateString.append(" - ?????????");
                break;
            }
            case PUB_DELAY_EXCAVATOR_POWER:
            {
                stateString.append(" - ??????");
                break;
            }
            case PUB_DELAY_EXCAVATOR_DROP:
            {
                stateString.append(" - ?????????");
                break;
            }
            case PUB_DELAY_EXCAVATOR_PUMP:
            {
                stateString.append(" - ?????????");
                break;
            }
            case PUB_DELAY_EXCAVATOR_CABLE:
            {
                stateString.append(" - ?????????");
                break;
            }
            default:
            {
                break;
            }
        }
    }

    ui->titleStateLabel->setText(stateString);

    ui->titleNameLabel->setText(QString("??????ID:") +  QString(QByteArray(localImeiName.localname, sizeof(localImeiName.localname))));

    char rssi = pos->getRssi();

    bool warnV2N=pos->getWarnningBit(v2n);


    if(rssi < 10)
    {
        if(warnV2N)//????????????????????????????????????
            ui->titleSignalLabel->setStyleSheet("border-image:url(:/image/resource/noConnect.png);");
        else//????????????????????????????????????rssi??????
            ui->titleSignalLabel->setStyleSheet("border-image:url(:/image/resource/signal0.png);");
    }
    else if((rssi >= 10) && (rssi < 13))
    {
        if(warnV2N)//?????????????????????????????????
            ui->titleSignalLabel->setStyleSheet("border-image:url(:/image/resource/noConnectSignal.png);");
        else//???????????????????????????rssi??????
            ui->titleSignalLabel->setStyleSheet("border-image: url(:/image/resource/signal1.png);");
    }
    else if((rssi >= 13) && (rssi < 16))
    {
        if(warnV2N)//?????????????????????????????????
            ui->titleSignalLabel->setStyleSheet("border-image:url(:/image/resource/noConnectSignal.png);");
        else//???????????????????????????rssi??????
            ui->titleSignalLabel->setStyleSheet("border-image: url(:/image/resource/signal2.png);");
    }
    else if((rssi >= 16) && (rssi < 19))
    {
        if(warnV2N)//?????????????????????????????????
            ui->titleSignalLabel->setStyleSheet("border-image:url(:/image/resource/noConnectSignal.png);");
        else//???????????????????????????rssi??????
            ui->titleSignalLabel->setStyleSheet("border-image: url(:/image/resource/signal3.png);");
    }
    else if((rssi >= 20) && (rssi < 25))
    {
        if(warnV2N)//?????????????????????????????????
            ui->titleSignalLabel->setStyleSheet("border-image:url(:/image/resource/noConnectSignal.png);");
        else//???????????????????????????rssi??????
            ui->titleSignalLabel->setStyleSheet("border-image: url(:/image/resource/signal4.png);");
    }
    else if(rssi >= 25)
    {
        if(warnV2N)//?????????????????????????????????
            ui->titleSignalLabel->setStyleSheet("border-image:url(:/image/resource/noConnectSignal.png);");
        else//???????????????????????????rssi??????
            ui->titleSignalLabel->setStyleSheet("border-image: url(:/image/resource/signal5.png);");
    }

    char gpsState;
    struct latlngheading curPos;
    pos->getCurPos(&curPos);

    gpsState = pos->getGpsState();

    map->updateGpsState(gpsState);

//    qDebug()<<"scene contain self type pixmap:"<<map->scene->items().contains(map->selfVehicle->typeItem);

    if((curPos.lat > 0.0000001) && (curPos.lat < 360) && (curPos.lng > 0.0000001) && (curPos.lng < 360))
    {
        if(!map->scene->items().contains(map->selfVehicle))
        {
            if(map->selfVehicle == NULL)
            {
                int vehicleType=CRUSHER;
                if(ParaParsing::instance()->paraSt.pub.vehicleType==7)
                    vehicleType=PARKING;


                quint64 width=real2pix*ParaParsing::instance()->paraSt.pub.vehicleWidth;
                quint64 length=real2pix*ParaParsing::instance()->paraSt.pub.vehicleLength;
                if(width<10)width=10;
                if(length<10)length=10;
                map->setSelfVehicle(vehicleType, QString::fromUtf8(localImeiName.localname), QSize(width, length));
            }

            
        }

        map->updateVehiclePos(curPos);
        map->scene->setSceneRect(map->scene->itemsBoundingRect());
        map->view->centerOn(map->selfVehicle->typeItem);
    }

    //????????????:??????????????????????????????????????????
    int selectedRow=ui->unLoadListInfoTable->currentRow();
    WorkListDeal *workList=WorkListDeal::getHandle();

    //qDebug()<<"selectedRow:"<<selectedRow;
    if(selectedRow>=0)//?????????
    {

    }else//????????????
    {
        selectedRow=0;
    }
    if(workList->inList->size()>=selectedRow+1)
    {
        static char preState;
        //qDebug()<<QString::number(workList->inList->at(selectedRow).workState);
        //??????inList??????????????????????????????????????????????????????
        switch (workList->inList->at(selectedRow).workState) {
            case PUB_WORK_OPT_IN_TASK:               //??????????????????????????????????????????-?????????
            {
                ui->unLoadRunWorkBtn->setChecked(false);
                ui->unLoadRunWorkBtnLabel->setText("????????????");
                break;
            }
            case PUB_WORK_OPT_FINISH_TASK:           //??????????????????????????????????????????-????????????
            {
               ui->unLoadRunWorkBtn->setChecked(true);
               ui->unLoadRunWorkBtnLabel->setText("????????????");
               break;
            }
            case PUB_WORK_OPT_STOP_TASK:             //??????????????????????????????????????????-??????????????????
            {
                if(preState!=PUB_WORK_OPT_STOP_TASK)
                {
                   //ui->emergenceBtn->setChecked(true);
                    abnormalChoice->clearChoice();

                    ui->controlStackedWidget->setCurrentWidget(ui->controlabnormalCheckPage);
                    ui->centralStackedWidget->setCurrentWidget(ui->centralAbnormalPage);
                }
                break;
            }
            case PUB_WORK_OPT_RECOVER_TASK:          //??????????????????????????????????????????-???????????????????????????
            {
               //ui->emergenceBtn->setChecked(false);
               break;
            }
            case PUB_WORK_OPT_MAN_TASK:              //??????????????????????????????????????????-?????????????????????

            case PUB_WORK_OPT_RE_IN_TASK:            //??????????????????????????????????????????-???????????????????????????

            case PUB_WORK_OPT_UNLOAD_TASK:           //??????????????????????????????????????????-????????????

            case PUB_WORK_OPT_RUN:

            default:
                break;
        }
        preState=workList->inList->at(selectedRow).workState;
    }
}

void uiCrush::messageEndTimerSlot()
{
    ui->messageBrowser->moveCursor(QTextCursor::Start);
}

//??????????????????
void uiCrush::on_messageUpBtn_clicked()
{
    ui->messageBrowser->moveCursor(QTextCursor::Up);
    messageEndTimer->start(2000);
}

//??????????????????
void uiCrush::on_messageDownBtn_clicked()
{
    ui->messageBrowser->moveCursor(QTextCursor::Down);
    messageEndTimer->start(2000);
}

void uiCrush::blinkTimerSlot()
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

void uiCrush::collectionTimerSlot()
{
    PosInfo* pos = PosInfo::getHandle();
    struct latlngheading curPos;
    pos->getCurPos(&curPos);

    addPosDataFun(&collectionList, curPos);
}

void uiCrush::mineInfoTimerSlot()
{
    ui->unLoadListInfoTable->setCurrentItem(NULL);
    ui->unLoadListInfoTable->clearContents();

    WorkListDeal* worklist = WorkListDeal::getHandle();

    worklist->listMutex->lock();
    for(int count = 0; count < worklist->inList->size(); count++)
    {
        ui->unLoadListInfoTable->setItem(count, 0,new QTableWidgetItem(worklist->inList->at(count).localname));
        ui->unLoadListInfoTable->setItem(count, 1,new QTableWidgetItem("??????"));

        QString strWorkState;
        switch(worklist->inList->at(count).workState)
        {
            case PUB_WORK_OPT_IN_TASK:               //??????????????????????????????????????????-?????????
            {
                strWorkState = QString::fromUtf8("????????????");
                break;
            }
            case PUB_WORK_OPT_FINISH_TASK:           //??????????????????????????????????????????-????????????
            {
               strWorkState = QString::fromUtf8("????????????");
               break;
            }
            case PUB_WORK_OPT_STOP_TASK:             //??????????????????????????????????????????-??????????????????
            {
               strWorkState = QString::fromUtf8("????????????");
               //????????????????????????
               break;
            }
            case PUB_WORK_OPT_RECOVER_TASK:          //??????????????????????????????????????????-???????????????????????????
            {
               strWorkState = QString::fromUtf8("????????????");
               break;
            }
            case PUB_WORK_OPT_MAN_TASK:              //??????????????????????????????????????????-?????????????????????
            {
               strWorkState = QString::fromUtf8("????????????");
               break;
            }
            case PUB_WORK_OPT_RE_IN_TASK:            //??????????????????????????????????????????-???????????????????????????
            {
               strWorkState = QString::fromUtf8("????????????");
               break;
            }
            case PUB_WORK_OPT_UNLOAD_TASK:           //??????????????????????????????????????????-????????????
            {
               strWorkState = QString::fromUtf8("????????????");
               break;
            }
            case PUB_WORK_OPT_RUN:
            {
                strWorkState = QString::fromUtf8("????????????");
                break;
            }
        }
        ui->unLoadListInfoTable->setItem(count, 2,new QTableWidgetItem(strWorkState));
    }
    worklist->listMutex->unlock();
}

void uiCrush::aroundVehicleUpdateTimerSlot()
{
    map->updateVehicleList(aroundVehicleList);
    aroundVehicleList.clear();
}

void uiCrush::on_collectionBackBtn_clicked()
{
    ui->centralStackedWidget->setCurrentWidget(ui->centralWorkStatePage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlUnLoadWorkSetPage);
}

void uiCrush::on_startCollectionBtn_clicked()
{
    ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionStatePage);
}

void uiCrush::on_updateDataBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionUpdateDataPage);
}

void uiCrush::on_leftBtn_clicked()
{
    ui->suspendRestartBtn->setChecked(false);
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionStatePage);
}

void uiCrush::on_rightBtn_clicked()
{
    ui->suspendRestartBtn->setChecked(false);
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionStatePage);
}

void uiCrush::on_suspendRestartBtn_clicked()
{
    if(ui->suspendRestartBtn->isChecked())
    {
        ui->suspendRestartBtnLabel->setText("??????");
        PosInfo* pos = PosInfo::getHandle();
        struct latlngheading* data = new struct latlngheading;
        pos->getCurPos(data);
        collectionList.append(data);
        collectionTimer->start();
    }
    else
    {
        ui->suspendRestartBtnLabel->setText("??????");
        collectionTimer->stop();
    }
}

void uiCrush::on_stopBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionOverPage);
}

void uiCrush::on_collectionOverBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionStatePage);
}

void uiCrush::on_saveBtn_clicked()
{
    //??????????????????lpx
//    generateLpxFile(QString("%1/%2").arg("/tftpServer/mapinfo").arg(QDateTime::currentDateTime().toString("MM-dd-hh-mm-ss")),collectionList);
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionUpdateDataPage);
}

void uiCrush::on_deleteBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionDeleteDataPage);
}

void uiCrush::on_updateDataOkBtn_clicked()
{
    //??????????????????
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
}

void uiCrush::on_updateDataBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionOverPage);
}

void uiCrush::on_deleteDataOkBtn_clicked()
{
    //????????????????????????
    collectionList.clear();
    ui->controlStackedWidget->setCurrentWidget(ui->controlMainPage);
}

void uiCrush::on_deleteDataBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlCollectionOverPage);
}

void uiCrush::on_unLoadWorkSetBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlUnLoadWorkSetPage);
}

void uiCrush::on_abnormalOkBtn_clicked()
{
    if(abnormalChoice->buttonGroup->checkedButton() != NULL)
    {
        WorkListDeal* worklist = WorkListDeal::getHandle();
        struct pubVehicleDevNameStateST *firstNode = new struct pubVehicleDevNameStateST;
        bool result = false;
        int selectedRow=ui->unLoadListInfoTable->currentRow();
        qDebug()<<"selected row:"<<selectedRow;

        if( selectedRow>= 0)//?????????
        {
            if(ui->unLoadListInfoTable->item(selectedRow,0)&&selectedRow!=0)//???????????????????????????????????????
            {//??????????????????????????????
                *firstNode=worklist->inList->at(selectedRow);
                worklist->setFirstInList(*firstNode);
            }

            if(worklist->inList->size())
            {
                *firstNode = worklist->inList->at(0);
                result=true;
            }else
            {
                result=false;
            }
        }
        else//??????????????????????????????
        {
            result = worklist->getFirstInList(firstNode);
        }

        QString str;
        str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));

        if((result)&&(worklist->inList->size()>selectedRow))
        {
            str.append("[");
            str.append(QString::fromUtf8(QByteArray(firstNode->localname, 20)));
            str.append("] ");
            switch(abnormalChoice->buttonGroup->checkedId())
            {
                case 1:
                {
                    //??????????????????
                    str.append("??????????????????");
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
                    //??????????????????
                    str.append("??????????????????");
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
                    //??????????????????
                    str.append("??????????????????");
                    worklist->removeFirstInList();
//                    ui->unLoadRunWorkBtn->setChecked(false);
//                    ui->unLoadRunWorkBtnLabel->setText("????????????");
                    mineInfoTimerSlot();
                    break;
                }
                case 4:
                {
                    //??????????????????
                    str.append("??????????????????");
                    struct pubVehicleResWorkOptST opt;
                    memmove(opt.localimei, localImeiName.localimei, 15);
                    memmove(opt.localname, localImeiName.localname, 20);
                    memmove(opt.remoteimei, firstNode->localimei, 15);
                    memmove(opt.remotename, firstNode->localname, 20);
                    if(ParaParsing::instance()->paraSt.pub.vehicleType==8)
                    {
                        ParaParsing::instance()->crusherParsing();
                        opt.taskType = PUB_WORK_OPT_UNLOAD_TYPE;
                        opt.cmdType = PUB_WORK_OPT_RECOVER_TASK;
                        opt.loadStatu = PUB_WORK_STATE_FULLLOAD_TYPE;
                        opt.materType = PUB_MATER_LOW_ORE;
                        opt.lat = ParaParsing::instance()->paraSt.crusher.stopLat;
                        opt.lng = ParaParsing::instance()->paraSt.crusher.stopLng;
                        opt.heading = ParaParsing::instance()->paraSt.crusher.stopHeading;
                    }else if(ParaParsing::instance()->paraSt.pub.vehicleType==7)
                    {
                        opt.taskType = PUB_WORK_OPT_UNLOAD_TYPE;
                        opt.cmdType = PUB_WORK_OPT_RECOVER_TASK;
                        opt.loadStatu = PUB_WORK_STATE_FULLLOAD_TYPE;
                        opt.materType = PUB_MATER_LOW_ORE;
                        opt.lat = 0;
                        opt.lng = 0;
                        opt.heading = 0;
                    }
                    opt.utcTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

                    emit h2gResWorkOptSignal(opt);
                    //xuzk2
//                    firstNode->workState = PUB_WORK_OPT_RECOVER_TASK;
//                    worklist->insertInList(*firstNode);
                    break;
                }
                default:
                    break;
            }

            ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
            ui->controlStackedWidget->setCurrentWidget(ui->controlUnLoadWorkSetPage);
        }else
            str.append("??????????????????");
        commonUIFunction::instance()->messageShow(ui->messageBrowser,str);

        delete firstNode;

    }
}

void uiCrush::on_abnormalBtn_clicked()
{

#if 0
    //????????????
    abnormalChoice->clearChoice();

    ui->controlStackedWidget->setCurrentWidget(ui->controlabnormalCheckPage);
    ui->centralStackedWidget->setCurrentWidget(ui->centralAbnormalPage);

#else

    //??????????????????
    WorkListDeal* worklist = WorkListDeal::getHandle();
    struct pubVehicleDevNameStateST *firstNode = new struct pubVehicleDevNameStateST;
    bool result = false;
    int selectedRow=ui->unLoadListInfoTable->currentRow();
    qDebug()<<"selected row:"<<selectedRow;

    if( selectedRow>= 0)//?????????
    {
        if(ui->unLoadListInfoTable->item(selectedRow,0)&&selectedRow!=0)//???????????????????????????????????????
        {//??????????????????????????????
            *firstNode=worklist->inList->at(selectedRow);
            worklist->setFirstInList(*firstNode);
        }

        if(worklist->inList->size())
        {
            *firstNode = worklist->inList->at(0);
            result=true;
        }else
        {
            result=false;
        }
    }
    else//??????????????????????????????
    {
        result = worklist->getFirstInList(firstNode);
    }

    QString str;
    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));

    if((result)&&(worklist->inList->size()>selectedRow))
    {
        str.append("[");
        str.append(QString::fromUtf8(QByteArray(firstNode->localname, 20)));
        str.append("] ");
        //??????????????????
        str.append("??????????????????");
        worklist->removeFirstInList();
//                    ui->unLoadRunWorkBtn->setChecked(false);
//                    ui->unLoadRunWorkBtnLabel->setText("????????????");
        mineInfoTimerSlot();
    }else
        str.append("??????????????????");
    commonUIFunction::instance()->messageShow(ui->messageBrowser,str);

    delete firstNode;
#endif
}

void uiCrush::on_abnormalBackBtn_clicked()
{
    ui->controlStackedWidget->setCurrentWidget(ui->controlUnLoadWorkSetPage);
    ui->centralStackedWidget->setCurrentWidget(ui->centralMapViewPage);
}

void uiCrush::on_unLoadRunWorkBtn_clicked()
{
    WorkListDeal* worklist = WorkListDeal::getHandle();
    struct pubVehicleDevNameStateST* firstNode = new struct pubVehicleDevNameStateST;
    bool result = false;
    QString str;
    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));

    int selectedRow=ui->unLoadListInfoTable->currentRow();
    qDebug()<<"selected row:"<<selectedRow;

    if( selectedRow>= 0)//?????????
    {
        if(ui->unLoadListInfoTable->item(selectedRow,0)&&selectedRow!=0)//???????????????????????????????????????
        {//??????????????????????????????
            *firstNode=worklist->inList->at(selectedRow);
            qDebug()<<"test1:"<<worklist->inList->at(0).localname;
            worklist->setFirstInList(*firstNode);
            qDebug()<<"test2:"<<worklist->inList->at(0).localname;
        }

        if(worklist->inList->size())
        {
            *firstNode = worklist->inList->at(0);
            result=true;
        }else
        {
            result=false;
        }
    }
    else//??????????????????????????????
    {
        result = worklist->getFirstInList(firstNode);
    }
    ui->unLoadRunWorkBtn->setChecked(!ui->unLoadRunWorkBtn->isChecked());
    if((result)&&(worklist->inList->size()>selectedRow))
    {
        if((ui->unLoadRunWorkBtn->isChecked())&&(ParaParsing::instance()->paraSt.pub.vehicleType==8))//false
        {
            //??????????????????
            if(firstNode->workState == PUB_WORK_OPT_FINISH_TASK)//???????????????????????????
            {
                struct pubVehicleResWorkOptST opt;
                memmove(opt.localimei, localImeiName.localimei, 15);
                memmove(opt.localname, localImeiName.localname, 20);
                memmove(opt.remoteimei, firstNode->localimei, 15);
                memmove(opt.remotename, firstNode->localname, 20);
                str.append("[");
                str.append(QString::fromUtf8(QByteArray(firstNode->localname, 20)));
                str.append("] ");

                if(ParaParsing::instance()->paraSt.pub.vehicleType==8)//?????????????????????????????????HMI??????????????????
                {
                    ParaParsing::instance()->crusherParsing();
                    opt.taskType = PUB_WORK_OPT_UNLOAD_TYPE;
                    opt.cmdType = PUB_WORK_OPT_UNLOAD_TASK;
                    opt.loadStatu = PUB_WORK_STATE_FULLLOAD_TYPE;
                    opt.materType = PUB_MATER_LOW_ORE;
                    opt.lat = ParaParsing::instance()->paraSt.crusher.stopLat;
                    opt.lng = ParaParsing::instance()->paraSt.crusher.stopLng;
                    opt.heading = ParaParsing::instance()->paraSt.crusher.stopHeading;
                    str.append("??????????????????");
                }
                opt.utcTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

                emit h2gResWorkOptSignal(opt);
                ui->unLoadRunWorkBtnLabel->setText("????????????");

                commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
            }
            else//??????????????????????????????
            {
                LOG_DEBUG("The FirstNode:[%s]-name[%s] workState is not OPT_FINISH_TASK,is:%d",
                          firstNode->localimei,
                          firstNode->localname,
                          firstNode->workState);

                str.append("[");
                str.append(QString::fromUtf8(QByteArray(firstNode->localname, 20)));
                str.append("] ");
                str.append("????????????????????????");

    //            ui->unLoadRunWorkBtn->setChecked(!ui->unLoadRunWorkBtn->isChecked());
    //            ui->unLoadRunWorkBtnLabel->setText("????????????");
                commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
            }
        }
        else//true
        {
            //??????????????????
            //???????????????????????????????????????????????????????????????????????????

            if((firstNode->workState == PUB_WORK_OPT_IN_TASK)//???????????????????????????
                    ||(firstNode->workState == PUB_WORK_OPT_RECOVER_TASK))//?????????????????????????????????
            {
                struct pubVehicleResWorkOptST opt;
                memmove(opt.localimei, localImeiName.localimei, 15);
                memmove(opt.localname, localImeiName.localname, 20);
                memmove(opt.remoteimei, firstNode->localimei, 15);
                memmove(opt.remotename, firstNode->localname, 20);

                if(ParaParsing::instance()->paraSt.pub.vehicleType==8)//??????????????????????????????????????????
                {
                    ParaParsing::instance()->crusherParsing();
                    opt.taskType = PUB_WORK_OPT_UNLOAD_TYPE;
                    opt.cmdType = PUB_WORK_OPT_IN_TASK;
                    opt.loadStatu = PUB_WORK_STATE_FULLLOAD_TYPE;
                    opt.materType = PUB_MATER_LOW_ORE;
                    opt.lat = ParaParsing::instance()->paraSt.crusher.stopLat;
                    opt.lng = ParaParsing::instance()->paraSt.crusher.stopLng;
                    opt.heading = ParaParsing::instance()->paraSt.crusher.stopHeading;
                }else if(ParaParsing::instance()->paraSt.pub.vehicleType==7)//?????????
                {
                    opt.taskType = PUB_WORK_OPT_LOAD_TYPE;
                    opt.cmdType = PUB_WORK_OPT_IN_TASK;
                    opt.loadStatu = PUB_WORK_STATE_EMPTYLOAD_TYPE;
                    opt.materType = PUB_MATER_LOW_ORE;
                    opt.lat = 0;
                    opt.lng = 0;
                    opt.heading = 0;
                }
                opt.utcTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

                emit h2gResWorkOptSignal(opt);
                if(ParaParsing::instance()->paraSt.pub.vehicleType==8)//?????????
                {
                    ui->unLoadRunWorkBtnLabel->setText("????????????");
                }
                str.append("[");
                str.append(QString::fromUtf8(QByteArray(firstNode->localname, 20)));
                str.append("] ");
                str.append("??????????????????");
//                commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
            }
            else//??????????????????????????????
            {
                LOG_DEBUG("The FirstNode:[%s]-name[%s] workState is not OPT_IN_TASK,is:%d",
                          firstNode->localimei,
                          firstNode->localname,
                          firstNode->workState);
                str.append("[");
                str.append(QString::fromUtf8(QByteArray(firstNode->localname, 20)));
                str.append("] ");
                str.append("????????????????????????");
                if(ParaParsing::instance()->paraSt.pub.vehicleType==8)
                {
    //                ui->unLoadRunWorkBtn->setChecked(!ui->unLoadRunWorkBtn->isChecked());
    //                ui->unLoadRunWorkBtnLabel->setText("????????????");
                }
            }
        }
    }else
    {
        str.append("??????????????????");
    }
    delete firstNode;
    commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
    mineInfoTimerSlot();
}

void uiCrush::on_unLoadOverOutBtn_clicked()
{
    QString str;
    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));
    WorkListDeal* worklist = WorkListDeal::getHandle();
    struct pubVehicleDevNameStateST *firstNode = new struct pubVehicleDevNameStateST;
    bool result = false;
    int selectedRow=ui->unLoadListInfoTable->currentRow();
    qDebug()<<"selected row:"<<selectedRow;

    if( selectedRow>= 0)//?????????
    {
        if(ui->unLoadListInfoTable->item(selectedRow,0)&&selectedRow!=0)//???????????????????????????????????????
        {//??????????????????????????????
            *firstNode=worklist->inList->at(selectedRow);
            worklist->setFirstInList(*firstNode);
        }

        if(worklist->inList->size())
        {
            *firstNode = worklist->inList->at(0);
            result=true;
        }else
        {
            result=false;
        }
    }
    else//??????????????????????????????
    {
        result = worklist->getFirstInList(firstNode);
    }


    if((result)&&(worklist->inList->size()>selectedRow))
    {
        if(firstNode->workState == PUB_WORK_OPT_UNLOAD_TASK)
        {

            str.append("[");
            str.append(QString::fromUtf8(QByteArray(firstNode->localname, 20)));
            str.append("] ");
            str.append("??????????????????");

            struct pubVehicleResWorkOptST opt;
            memmove(opt.localimei, localImeiName.localimei, 15);
            memmove(opt.localname, localImeiName.localname, 20);
            memmove(opt.remoteimei, firstNode->localimei, 15);
            memmove(opt.remotename, firstNode->localname, 20);
            opt.taskType = PUB_WORK_OPT_UNLOAD_TYPE;
            opt.cmdType = PUB_WORK_OPT_OUT_TASK;
            opt.loadStatu = PUB_WORK_STATE_EMPTYLOAD_TYPE;
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
            str.append("????????????????????????");

        }
    }else
    {
        str.append("??????????????????");
    }
    commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
    delete firstNode;
}

void uiCrush::on_emergenceBtn_clicked()
{
    WorkListDeal* worklist = WorkListDeal::getHandle();
    struct pubVehicleDevNameStateST* firstNode = new struct pubVehicleDevNameStateST;
    bool result = false;
    QString str;
    str.append(QDateTime::currentDateTime().toString("hh:mm:ss :"));

    int selectedRow=ui->unLoadListInfoTable->currentRow();
    //qDebug()<<"selected row:"<<selectedRow;

    if( selectedRow>= 0)//?????????
    {
        if(ui->unLoadListInfoTable->item(selectedRow,0)&&selectedRow!=0)//???????????????????????????????????????
        {//??????????????????????????????
            *firstNode=worklist->inList->at(selectedRow);
            qDebug()<<"test1:"<<worklist->inList->at(0).localname;
            worklist->setFirstInList(*firstNode);
            qDebug()<<"test2:"<<worklist->inList->at(0).localname;
        }

        if(worklist->inList->size())
        {
            *firstNode = worklist->inList->at(0);
            result=true;
        }else
        {
            result=false;
        }
    }
    else//??????????????????????????????
    {
        result = worklist->getFirstInList(firstNode);
    }

    if(result&&(worklist->inList->size()>selectedRow))
    {
        //??????????????????

        str.append("[");
        str.append(QString::fromUtf8(QByteArray(firstNode->localname, 20)));
        str.append("] ");
        str.append("??????????????????");
        struct pubVehicleResWorkOptST opt;
        memmove(opt.localimei, localImeiName.localimei, 15);
        memmove(opt.localname, localImeiName.localname, 20);
        memmove(opt.remoteimei,firstNode->localimei, 15);
        memmove(opt.remotename, firstNode->localname, 20);
        if(ParaParsing::instance()->paraSt.pub.vehicleType==8)
        {
            ParaParsing::instance()->crusherParsing();
            opt.taskType = PUB_WORK_OPT_UNLOAD_TYPE;
            opt.cmdType = PUB_WORK_OPT_STOP_TASK;
            opt.loadStatu = PUB_WORK_STATE_FULLLOAD_TYPE;
            opt.materType = PUB_MATER_LOW_ORE;
            opt.lat = ParaParsing::instance()->paraSt.crusher.stopLat;
            opt.lng = ParaParsing::instance()->paraSt.crusher.stopLng;
            opt.heading = ParaParsing::instance()->paraSt.crusher.stopHeading;
        }else if(ParaParsing::instance()->paraSt.pub.vehicleType==7)
        {
            opt.taskType = PUB_WORK_OPT_UNLOAD_TYPE;
            opt.cmdType = PUB_WORK_OPT_STOP_TASK;
            opt.loadStatu = PUB_WORK_STATE_FULLLOAD_TYPE;
            opt.materType = PUB_MATER_LOW_ORE;
            opt.lat = 0;
            opt.lng = 0;
            opt.heading = 0;
        }
        opt.utcTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

        emit h2gResWorkOptSignal(opt);
        abnormalChoice->clearChoice();

        ui->controlStackedWidget->setCurrentWidget(ui->controlabnormalCheckPage);
        ui->centralStackedWidget->setCurrentWidget(ui->centralAbnormalPage);
        mineInfoTimerSlot();
    //  xuzk2
    //    firstNode->workState = PUB_WORK_OPT_STOP_TASK;
    //    worklist->insertInList(*firstNode);
    }else
    {
        str.append("??????????????????");
    }


    commonUIFunction::instance()->messageShow(ui->messageBrowser,str);
    delete firstNode;
}

void uiCrush::on_voiceMinusBtn_clicked()
{
    //????????????
//    struct pubVehiclePosST info;
//    info.gLat = 40.1023023;
//    info.gLng = 116.0987327;
//    info.gHead = 80;
//    g2hPosSlot(info);
}

void uiCrush::on_voicePlusBtn_clicked()
{
    //????????????
//    struct pubVehicleCollWarnningST otherVehicle;
//    otherVehicle.dVehicleLat = 40.1023112;
//    otherVehicle.dVehicleLng = 116.0987327;
//    otherVehicle.dVehicleHeading = 90;
//    memmove(otherVehicle.dName, QByteArray("Huang", 20), 20);
//    g2hCollWarnningSlot(otherVehicle);
}
