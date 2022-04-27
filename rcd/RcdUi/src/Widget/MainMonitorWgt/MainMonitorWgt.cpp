#include "MainMonitorWgt.h"
#include "ui_MainMonitorWgt.h"
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPainter>
#include <QThread>
#include <QPixmap>
#include <QTimer>
#include <QDebug>

MainMonitorWgt* MainMonitorWgt::s_pInstance = nullptr; //初始化静态数据成员

MainMonitorWgt::MainMonitorWgt(QWidget *parent) :
    IFunWidget(parent),
    ui(new Ui::MainMonitorWgt)
{
    ui->setupUi(this);
    // 创建控制层
    m_pCL = new MainMonitorCL(this);
    m_pCL->doInit();
    this->setCL(m_pCL);
    // 创建数据层
    m_pModel = new MainMonitorModel();
    this->setModel(m_pModel);
    m_pCL->setModel(m_pModel);
    // 布局
    QVBoxLayout* pVLayout = new QVBoxLayout(ui->title);
    pVLayout->setContentsMargins(0,0,0,0);
    pVLayout->setSpacing(0);

    pVLayout->addStretch(6);

    QGridLayout *pGridLayout = new QGridLayout();
    pGridLayout->setHorizontalSpacing(10);
    pGridLayout->setVerticalSpacing(20);
    pGridLayout->setContentsMargins(20,0,20,0);
    pVLayout->addLayout(pGridLayout,30);

    pVLayout->addStretch(2);

    pVLayout->addWidget(ui->pageWgt,2);
    pVLayout->addStretch(2);

    pGridLayout->addWidget(ui->midVideoFrame1,0,0,1,1);
    pGridLayout->addWidget(ui->midVideoFrame2,0,1,1,1);
    pGridLayout->addWidget(ui->midVideoFrame3,0,2,1,1);
    pGridLayout->addWidget(ui->midVideoFrame4,1,0,1,1);
    pGridLayout->addWidget(ui->midVideoFrame5,1,1,1,1);
    pGridLayout->addWidget(ui->midVideoFrame6,1,2,1,1);

    ui->title->setLayout(pVLayout);

    m_frameVector.append(ui->midVideoFrame1);
    m_frameVector.append(ui->midVideoFrame2);
    m_frameVector.append(ui->midVideoFrame3);
    m_frameVector.append(ui->midVideoFrame4);
    m_frameVector.append(ui->midVideoFrame5);
    m_frameVector.append(ui->midVideoFrame6);

    m_playWgtVector.append(ui->playWgt1);
    m_playWgtVector.append(ui->playWgt2);
    m_playWgtVector.append(ui->playWgt3);
    m_playWgtVector.append(ui->playWgt4);
    m_playWgtVector.append(ui->playWgt5);
    m_playWgtVector.append(ui->playWgt6);
    for(auto playWgt : m_playWgtVector){
        playWgt->setShadowType(TGFrame::ShadowMidPage1No1);
    }

    m_nameVector.append(ui->labelImei1);
    m_nameVector.append(ui->labelImei2);
    m_nameVector.append(ui->labelImei3);
    m_nameVector.append(ui->labelImei4);
    m_nameVector.append(ui->labelImei5);
    m_nameVector.append(ui->labelImei6);
}

MainMonitorWgt::~MainMonitorWgt()
{
    delete ui;
    delete m_pModel;
}

MainMonitorWgt *MainMonitorWgt::GetInstance()
{
    if(s_pInstance == nullptr) {
        s_pInstance = new MainMonitorWgt();
    }
    return s_pInstance;
}

void MainMonitorWgt::initWidget()
{
    qInfo()<<"MainMonitorWgt::initWidget";
    m_pCL->intoAction();
}

bool MainMonitorWgt::isQuit()
{
    qInfo()<<"MainMonitorWgt::isQuit";
    m_pCL->exitAction();
    return true;
}

void MainMonitorWgt::updateWidget()
{
    qInfo()<<"MainMonitorWgt::updateWidget";

    for(int i = 0;i < 6;i++){
        QString &imei = m_pModel->m_showVehicle[i];
        if(imei.isEmpty()){
            m_playWgtVector[i]->stopPlay();
            m_nameVector[i]->clear();
        }else{
            m_playWgtVector[i]->startPlay(m_pModel->m_vehicleMap[imei].url);
            m_nameVector[i]->setText(QString::fromUtf8("【%1】").arg(m_pModel->m_vehicleMap[imei].name));
        }
        if(i == m_pModel->m_select){
            m_frameVector[i]->setFrameType(TGFrame::VideoFrameMidPage1No2);
        }else{
            m_frameVector[i]->setFrameType(TGFrame::VideoFrameMidPage1No1);
        }
    }
    ui->pageWgt->setPage(m_pModel->m_pageIndex);
    ui->pageWgt->setMaxPage(m_pModel->m_maxPage);
}

void MainMonitorWgt::showEvent(QShowEvent *)
{
    qInfo()<<"MainMonitorWgt::startPlay";
    QWidget *pWidget = static_cast<QWidget*>(this->parent());
    pWidget->activateWindow();
    pWidget->grabKeyboard(); //强制抓取键盘输入，并使当前界面获取键盘焦点。
    this->updateWidget();
}

void MainMonitorWgt::hideEvent(QHideEvent *)
{
    qInfo()<<"MainMonitorWgt::stopPlay";
    for(int i = 0;i < 6;i++){
        m_playWgtVector[i]->stopPlay();
    }
}

void MainMonitorWgt::onClDataCome(int cmd, QVariant )
{
    MainMonitorCL::CLCmdType tCmd = static_cast<MainMonitorCL::CLCmdType>(cmd);
    switch (tCmd) {
    case MainMonitorCL::CL_UpdateAll:{
        if(!this->isHidden()){
            this->updateWidget();
        }
        break;
    }
    case MainMonitorCL::CL_UpdateSelect:{
        for(int i = 0;i < 6;i++){
            if(i == m_pModel->m_select){
                m_frameVector[i]->setFrameType(TGFrame::VideoFrameMidPage1No2);
            }else{
                m_frameVector[i]->setFrameType(TGFrame::VideoFrameMidPage1No1);
            }
        }
        break;
    }
    case MainMonitorCL::CL_InputKey:{
        QWidget *pWidget = static_cast<QWidget*>(this->parent());
        pWidget->activateWindow();
        pWidget->releaseKeyboard(); //有按键按下，完成抓取，释放全局按键抓取。
        break;
    }
    default:
        break;
    }
}

void MainMonitorWgt::onModelUpdate(int cmd)
{
    MainMonitorModel::ModelCmdType tCmd = static_cast<MainMonitorModel::ModelCmdType>(cmd);
    switch (tCmd) {
    case MainMonitorModel::MC_UpdateAll:
    {
        break;
    }
    case MainMonitorModel::MC_UpdateVehicle:
    {

        break;
    }

    default:
        break;
    }
}

