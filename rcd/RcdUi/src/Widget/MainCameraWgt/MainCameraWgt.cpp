#include "MainCameraWgt.h"
#include "ui_MainCameraWgt.h"
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QPainter>
#include <QThread>
#include <QPixmap>
#include <QTimer>
#include <QDebug>
#include <QStackedLayout>

MainCameraWgt* MainCameraWgt::s_pInstance = nullptr; //初始化静态数据成员

MainCameraWgt::MainCameraWgt(QWidget *parent) :
    IFunWidget(parent),
    ui(new Ui::MainCameraWgt)
{

    ui->setupUi(this);
    // 创建控制层
    m_pCL = new MainCameraCL(this);
    m_pCL->doInit();
    this->setCL(m_pCL);
    // 创建数据层
    m_pModel = new MainCameraModel();
    this->setModel(m_pModel);
    m_pCL->setModel(m_pModel);

    // 页面控件初始化
    ui->pingWgt->setFrameType(TGFrame::TipUnderline);
    ui->modelWgt->setFrameType(TGFrame::TipUnderline);
    ui->midVideoFrame1->setFrameType(TGFrame::VideoFrameMidPage2No1);
    ui->tachometerWgt->setParameter(RingDial::Tachometer,0,100);
    ui->speedWgt->setParameter(RingDial::Speedometer,0,60);
    ui->brakeWgt->setParameter(TriangleFill::Brake,0,100);
    ui->throttleWgt->setParameter(TriangleFill::Throttle,0,100);
    //ui->wheelWgt->setValue(50);
    //ui->widget_ReversingTrackLine->setParameter(0,180);//倒车轨迹线TC
    //ui->widget_ReversingTrackLine->setCurrentValue(20);//倒车轨迹线
    // 处理页面布局
    while (ui->CVLayout->count() > 0) {
        ui->CVLayout->removeItem(ui->CVLayout->itemAt(0));
    }
    ui->CVLayout->addStretch(3);
    QHBoxLayout *pHLayout = new QHBoxLayout();
    pHLayout->addStretch(2);
    pHLayout->addWidget(ui->modelWgt,3);
    pHLayout->addStretch(10);
    pHLayout->addWidget(ui->pingWgt,3);
    pHLayout->addStretch(2);
    ui->CVLayout->addLayout(pHLayout,3);

    ui->CVLayout->addStretch(33);
    ui->CVLayout->addLayout(ui->gridLayout,20);

    m_pSLayout = new QStackedLayout(this);
    m_pSLayout->setStackingMode(QStackedLayout::StackAll);
    m_pSLayout->insertWidget(0,ui->cameraTitle);
    m_pSLayout->insertWidget(0,ui->frontPlayWgt);//old 图像数据显示界面

    //    m_pSLayout->insertWidget(0,ui->widget_newcvfront);
    this->setLayout(m_pSLayout);
//    connect(&_timerTest,SIGNAL(timeout()),this,SLOT(slotTest()));
//    _timerTest.start(15000);

    ui->widget_ReversingTrackLine_old->setVisible(false);
}

MainCameraWgt::~MainCameraWgt()
{
    delete ui;
    delete m_pModel;
}

MainCameraWgt *MainCameraWgt::GetInstance()
{
    if(s_pInstance == nullptr) {
        s_pInstance = new MainCameraWgt();
    }
    return s_pInstance;
}

void MainCameraWgt::initWidget()
{
    qInfo()<<"MainCameraWgt::initWidget";
    m_pCL->intoAction();
}

bool MainCameraWgt::isQuit()
{
    qInfo()<<"MainCameraWgt::isQuit";
    m_pCL->exitAction();
    return true;
}

void MainCameraWgt::updateWidget()
{
    qInfo()<<"MainCameraWgt::updateWidget";
    //    QString urls = QString("rtsp://admin:itage123@192.168.23.221:554/0");
    ui->frontPlayWgt->startPlay(m_pModel->m_frontCameraUrl);//old 图像数据显示界面
    //    ui->widget_newcvfront->StartCapture(m_pModel->m_frontCameraUrl.toStdString());
    ui->backPlayWgt->startPlay(m_pModel->m_backCameraUrl);//old 后摄像头图像数据显示界面
    qDebug()<<m_pModel->m_backCameraUrl<<"后置摄像头url";
    //    ui->widget_newback->StartCapture(m_pModel->m_backCameraUrl.toStdString());
    ui->midVideoFrame1->hide();

    ui->pingWgt->setTitleString(QString::fromUtf8("通信延迟：  ???"));
    ui->modelWgt->setTitleString(QString::fromUtf8("驾驶模式：监控中"));
    ui->speedWgt->setParameter(RingDial::Speedometer,m_pModel->m_speed.min,m_pModel->m_speed.max);
    ui->brakeWgt->setParameter(TriangleFill::Brake,m_pModel->m_brake.min,m_pModel->m_brake.max);
    ui->throttleWgt->setParameter(TriangleFill::Throttle,m_pModel->m_throttle.min,m_pModel->m_throttle.max);
    ui->wheelWgt->setParameter(m_pModel->m_wheel.min,m_pModel->m_wheel.max);
    //ui->widget_ReversingTrackLine->setParameter(m_pModel->m_wheel.min,m_pModel->m_wheel.max);///倒车轨迹线

}

void MainCameraWgt::showEvent(QShowEvent *)
{
    qInfo()<<"MainCameraWgt::startPlay";
    this->updateWidget();
}

void MainCameraWgt::hideEvent(QHideEvent *)
{
    qInfo()<<"MainCameraWgt::stopPlay";
    ui->frontPlayWgt->stopPlay();//old 图像数据显示界面
    //    ui->widget_newcvfront->StopCapture();
    ui->backPlayWgt->stopPlay();
    //    ui->widget_newback->StopCapture();
}

// 数据更新触发页面更新
void MainCameraWgt::onModelUpdate(int cmd)
{
    MainCameraModel::ModelCmdType tCmd = static_cast<MainCameraModel::ModelCmdType>(cmd);
    switch (tCmd) {
    case MainCameraModel::MC_UpdateAll:
    {
        this->updateWidget();
        break;
    }
    case MainCameraModel::MC_ThrottleBrakeWheel:
    {
        ui->brakeWgt->setValue(m_pModel->m_brake.current);

        ui->wheelWgt->setValue(m_pModel->m_wheel.current);

        ui->widget_ReversingTrackLine_b->setAngleValue(m_pModel->m_wheel.current);//倒车轨迹线
        //        ui->widget_newcvfront->setAngleValue(m_pModel->m_wheel.current);
        ui->throttleWgt->setValue(m_pModel->m_throttle.current);

        break;
    }
    case MainCameraModel::MC_Brake:{
        ui->brakeWgt->setValue(m_pModel->m_brake.current);
        break;
    }
    case MainCameraModel::MC_Throttle:{//加速踏板百分比
        ui->throttleWgt->setValue(m_pModel->m_throttle.current);
        break;
    }
    case MainCameraModel::MC_Wheel:{//方向盘转角
        ui->wheelWgt->setValue(m_pModel->m_wheel.current);
        //        ui->widget_newcvfront->setAngleValue(m_pModel->m_wheel.current);
        if(m_pModel->m_gear==GearWgt::Gear_D){
//            ui->widget_ReversingTrackLine_f->setValue(-m_pModel->m_wheel.current);//倒车轨迹线
            ui->widget_ReversingTrackLine_f->setAngleValue(-m_pModel->m_wheel.current);//前进车线
        }
        else if(m_pModel->m_gear==GearWgt::Gear_R){
            ui->widget_ReversingTrackLine_b->setAngleValue(m_pModel->m_wheel.current);//倒车轨迹线
        }
        break;
    }
    case MainCameraModel::MC_Speed://速度
    {
        ui->speedWgt->setValue(m_pModel->m_speed.current);
        break;
    }
    case MainCameraModel::MC_Gear://档位
    {

        this->changeLayout(m_pModel->m_gear);
        if(m_pModel->m_gear==GearWgt::Gear_N){
            //            ui->widget_newcvfront->setVisbleLine(false);
            ui->widget_ReversingTrackLine_b->setVisible(false);
            ui->widget_ReversingTrackLine_f->setVisible(false);
        }else if(m_pModel->m_gear==GearWgt::Gear_D) {
            //            ui->widget_newcvfront->setVisbleLine(true);
            ui->widget_ReversingTrackLine_b->setVisible(false);
            ui->widget_ReversingTrackLine_f->setVisible(true);
        }
        else if(m_pModel->m_gear==GearWgt::Gear_R){

            ui->widget_ReversingTrackLine_f->setVisible(false);
            ui->widget_ReversingTrackLine_b->setVisible(true);
        }
        ui->gearWgt->setValue(m_pModel->m_gear);
        break;
    }
    case MainCameraModel::MC_Delay:
    {
        if(m_pModel->m_vehicleDelay == 0){
            ui->pingWgt->setTitleString(QString::fromUtf8("通信延迟： <1 ms"));
        }else{
            ui->pingWgt->setTitleString(QString::fromUtf8("通信延迟：%1  ms").arg(m_pModel->m_vehicleDelay));
        }
        break;
    }
    case MainCameraModel::MC_RemoteStatus:
    {
        ui->modelWgt->setTitleString(getRemoteStatusStr(m_pModel->m_remoteStatus));
        break;
    }
    case MainCameraModel::MC_Throttle_RingDila://转速单位换算需要÷100
    {
        ui->tachometerWgt->setValue(m_pModel->m_speed_raw.current/100);
        break;
    }
    default:
        break;
    }

    qDebug()<<"刹车百分比："<<m_pModel->m_brake.current<<"发动机转速:"<<m_pModel->m_speed_raw.current
           <<"油门百分比:"<<m_pModel->m_throttle.current<<"档位："<<m_pModel->m_gear
          <<"速度:"<<m_pModel->m_speed.current<<"方向盘转角："<<m_pModel->m_wheel.current
         <<"_________________________________";
}

QString MainCameraWgt::getRemoteStatusStr(MainCameraModel::RemoteStatus status)
{
    switch (status) {
    case MainCameraModel::RS_Monitoring: {
        return QString::fromUtf8("驾驶模式：监控中");
        break;
    }
    case MainCameraModel::RS_RemoteControl: {
        return QString::fromUtf8("驾驶模式：遥控驾驶中");
        break;
    }
    case MainCameraModel::RS_RemoteControlFail: {
        return QString::fromUtf8("驾驶模式：遥控失败");
        break;
    }
    case MainCameraModel::RS_RemoteControlDisconnect: {
        return QString::fromUtf8("驾驶模式：已断连");
        break;
    }
    default:
        break;
    }
    return QString();
}

// 倒档时的后视镜布局改变处理。
void MainCameraWgt::changeLayout(int layout)
{
    static int sLastType = 0;
    //    Gear_D = 0x1,
    //    Gear_N = 0x0,
    //    Gear_R = -0x01

    if(layout == GearWgt::Gear_R && sLastType != GearWgt::Gear_R){//R档
        m_pSLayout->removeWidget(ui->frontPlayWgt);
        ui->minPLayout->removeWidget(ui->backPlayWgt);
        m_pSLayout->insertWidget(0,ui->backPlayWgt);//
        ui->minPLayout->addWidget(ui->frontPlayWgt,1);
        ui->frontPlayWgt->setShadowType(TGFrame::ShadowMidPage2No1);
        ui->backPlayWgt->setVisible(true);
        ui->frontPlayWgt->setVisible(true);/*
        ui->widget_ReversingTrackLine_f->setVisible(false);
        ui->widget_ReversingTrackLine_b->setVisible(true);*/
        ui->midVideoFrame1->show();

    }else if(layout != GearWgt::Gear_R && sLastType == GearWgt::Gear_R){
        m_pSLayout->removeWidget(ui->backPlayWgt);
        ui->minPLayout->removeWidget(ui->frontPlayWgt);
        m_pSLayout->insertWidget(0,ui->frontPlayWgt);
        ui->minPLayout->addWidget(ui->backPlayWgt,1);
        ui->frontPlayWgt->setVisible(true);/*
        ui->widget_ReversingTrackLine_b->setVisible(false);
        ui->widget_ReversingTrackLine_f->setVisible(true);*/
        ui->frontPlayWgt->setShadowType(TGFrame::None);
        ui->midVideoFrame1->hide();
    }
    sLastType = layout;
}

//// 倒档时的后视镜布局改变处理。
//void MainCameraWgt::changeLayout(int layout)
//{
//    static int sLastType = 0;
//    if(layout == GearWgt::Gear_N){
//        ui->widget_newcvfront->setVisbleLine(false);
//    }else {
//        ui->widget_newcvfront->setVisbleLine(true);
//    }
//    if(layout == GearWgt::Gear_R && sLastType != GearWgt::Gear_R){
//        // m_pSLayout->removeWidget(ui->frontPlayWgt);//old 图像数据显示界面
//        m_pSLayout->removeWidget(ui->widget_newcvfront);//new 图像数据显示界面
//        ui->minPLayout->removeWidget(ui->widget_newback);
//        m_pSLayout->insertWidget(0,ui->widget_newback);
//        ui->minPLayout->addWidget(ui->widget_newcvfront,1);//new 图像数据显示界面
//        ui->widget_newcvfront->setVisbleLine(false);
////        ui->widget_newback->setSize(500,500);
//        ui->widget_newback->setVisible(true);

//        ui->widget_newcvfront->setVisible(true);
//        //  ui->frontPlayWgt->setVisible(true);

////        qDebug()<<ui->backPlayWgt->isVisible()<<"后摄像头界面是否打开";
//        ui->midVideoFrame1->setFixedSize(423, 250);
//        ui->midVideoFrame1->show();

//    }else if(layout != GearWgt::Gear_R && sLastType == GearWgt::Gear_R){
//        m_pSLayout->removeWidget(ui->widget_newback);
//        //  ui->minPLayout->removeWidget(ui->frontPlayWgt);//old 图像数据显示界面
//        ui->minPLayout->removeWidget(ui->widget_newcvfront);//new 图像数据显示界面
//        // m_pSLayout->insertWidget(0,ui->frontPlayWgt);//old 图像数据显示界面
//        m_pSLayout->insertWidget(0,ui->widget_newcvfront);//new 图像数据显示界面
//        ui->minPLayout->addWidget(ui->widget_newback,1);
//        // ui->frontPlayWgt->setVisible(true);

//        ui->widget_newcvfront->setVisible(true);

//        //ui->frontPlayWgt->setShadowType(TGFrame::None);
//        ui->midVideoFrame1->hide();
//    }

//    sLastType = layout;
//}

// 倒档时的后视镜布局改变处理。
//void MainCameraWgt::changeLayout(int layout)
//{
//    static int sLastType = 0;
//    //    Gear_D = 0x1,
//    //    Gear_N = 0x0,
//    //    Gear_R = -0x01
//    if(layout == GearWgt::Gear_N){
//        ui->widget_newcvfront->setVisbleLine(false);
//    }else {
//        ui->widget_newcvfront->setVisbleLine(true);
//    }
//    if(layout == GearWgt::Gear_R && sLastType != GearWgt::Gear_R){//R档
//        m_pSLayout->removeWidget(ui->widget_newcvfront);
//        ui->minPLayout->removeWidget(ui->widget_newback);
//        ui->widget_newcvfront->setVisbleLine(false);
//        ui->widget_newback->setSize(100,100);
//        m_pSLayout->insertWidget(0,ui->widget_newcvfront);//
//        ui->minPLayout->addWidget(ui->widget_newcvfront,1);

//        //        ui->frontPlayWgt->setShadowType(TGFrame::ShadowMidPage2No1);

//        ui->widget_newcvfront->setVisible(true);
//        ui->widget_newback->setVisible(true);
//        ui->midVideoFrame1->setFixedSize(423, 250);
//        ui->midVideoFrame1->show();

//        //        m_pSLayout->removeWidget(ui->widget_newcvfront);
//        //        ui->minPLayout->removeWidget(ui->backPlayWgt);
//        //        m_pSLayout->insertWidget(0,ui->backPlayWgt);//
//        //        ui->minPLayout->addWidget(ui->widget_newcvfront,1);

//        //        ui->backPlayWgt->setVisible(true);
//        //        ui->widget_newcvfront->setVisible(true);
//        //        ui->midVideoFrame1->setFixedSize(423, 250);
//        //        ui->midVideoFrame1->show();
//        qDebug()<<"当前是R档显示画中画";

//    }else if(layout != GearWgt::Gear_R && sLastType == GearWgt::Gear_R){
//        m_pSLayout->removeWidget(ui->widget_newback);
//        ui->minPLayout->removeWidget(ui->widget_newcvfront);
//        m_pSLayout->insertWidget(0,ui->widget_newcvfront);
//        ui->minPLayout->addWidget(ui->widget_newback,1);
//        ui->widget_newcvfront->setVisible(true);
//        ui->widget_newcvfront->setVisbleLine(true);
//        ui->midVideoFrame1->hide();
//        qDebug()<<"当前是D档显示";
//    }

//    sLastType = layout;
//}
void MainCameraWgt::slotTest()
{
    if(_isR==0){
        this->changeLayout(GearWgt::Gear_R);
        ui->widget_ReversingTrackLine_b->setVisible(true);
        ui->widget_ReversingTrackLine_f->setVisible(true);
        _isR = 1;
    }
    else if(_isR==1) {
        this->changeLayout(GearWgt::Gear_D);
        _isR = 2;
    }
    else {
        this->changeLayout(GearWgt::Gear_N);
        _isR =0;
    }
}
