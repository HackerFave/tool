#include "RightCameraWgt.h"
#include "ui_RightCameraWgt.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QShowEvent>
#include <QPainter>
#include <QThread>
#include <QPixmap>
#include <QTimer>
#include <QDebug>

#include "Component/TitleWgt.h"

RightCameraWgt* RightCameraWgt::s_pInstance = nullptr; //初始化静态数据成员

RightCameraWgt::RightCameraWgt(QWidget *parent) :
    IFunWidget(parent),
    ui(new Ui::RightCameraWgt)
{
    ui->setupUi(this);
    m_pCL = new RightCameraCL(this);
    m_pCL->doInit();
    this->setCL(m_pCL);

    m_pModel = new RightCameraModel();
    this->setModel(m_pModel);
    m_pCL->setModel(m_pModel);

    QGridLayout *pGridLayout = new QGridLayout(ui->doInfoWgt);
    pGridLayout->setSpacing(0);
    pGridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    pGridLayout->setContentsMargins(0, 0, 0, 0);

    pGridLayout->addWidget(ui->stopOverLab, 2, 1, 1, 1);
    pGridLayout->addWidget(ui->exitQueueLab, 2, 3, 1, 1);
    pGridLayout->addWidget(ui->exitQueueImg, 1, 3, 1, 1);
    pGridLayout->addWidget(ui->stopOverImg, 1, 1, 1, 1);

    pGridLayout->setRowStretch(0, 3);
    pGridLayout->setRowStretch(1, 4);
    pGridLayout->setRowStretch(2, 2);
    pGridLayout->setRowStretch(3, 1);
    pGridLayout->setColumnStretch(0, 2);
    pGridLayout->setColumnStretch(1, 4);
    pGridLayout->setColumnStretch(2, 3);
    pGridLayout->setColumnStretch(3, 4);
    pGridLayout->setColumnStretch(4, 2);

    ui->playWgt->setShadowType(TGFrame::ShadowRightPage2No1);
    ui->rightVideoFrame1->setFrameType(TGFrame::VideoFrameRightPage2No1);
    ui->rightTitleFrame1->setFrameType(TGFrame::InfoFrameRightPage2No2);
    ui->rightTitleFrame2->setFrameType(TGFrame::InfoFrameRightPage2No3);
    ui->rightTitleFrame1->setTitleString(QString::fromUtf8("任务信息"));
    ui->rightTitleFrame2->setTitleString(QString::fromUtf8("操作信息"));

    QVBoxLayout* pVLayout = new QVBoxLayout(this);
    pVLayout->setContentsMargins(0,0,0,0);
    pVLayout->setSpacing(0);

    pVLayout->addStretch(2);
    pVLayout->addWidget(ui->rightVideoFrame1,62);
    pVLayout->addStretch(3);

    QHBoxLayout* pHLayout = new QHBoxLayout();
    pVLayout->addLayout(pHLayout,35);
    pHLayout->addWidget(ui->rightTitleFrame1,10);
    pHLayout->addStretch(1);
    pHLayout->addWidget(ui->rightTitleFrame2,10);

    pVLayout->addStretch(2);

    QVBoxLayout *pFrameVLayout = new QVBoxLayout(ui->rightVideoFrame1);
    pFrameVLayout->setContentsMargins(20, 20, 20, 20);
    pFrameVLayout->setSpacing(10);
    m_pTitleWgt = new TitleWgt(ui->rightVideoFrame1);
    QString str(QString::fromUtf8("【xxxx】 右摄像头"));
    m_pTitleWgt->setTitleString(str);
    pFrameVLayout->addWidget(m_pTitleWgt,1);
    pFrameVLayout->addWidget(ui->playWgt,9);
    ui->rightVideoFrame1->setLayout(pFrameVLayout);

    this->setLayout(pVLayout);
}

RightCameraWgt::~RightCameraWgt()
{
    delete ui;
    delete m_pCL;
    delete m_pModel;
}

RightCameraWgt *RightCameraWgt::GetInstance()
{
    if(s_pInstance == nullptr) {
        s_pInstance = new RightCameraWgt();
    }
    return s_pInstance;
}

void RightCameraWgt::initWidget()
{
    qInfo()<<"RightCameraWgt::initWidget";
    m_pCL->intoAction();
}

bool RightCameraWgt::isQuit()
{
    qInfo()<<"RightCameraWgt::isQuit";
    m_pCL->exitAction();
    return true;
}

void RightCameraWgt::updateWidget()
{
    qInfo()<<"RightCameraWgt::updateWidget";
    ui->playWgt->startPlay(m_pModel->m_url);
    m_pTitleWgt->setTitleString(m_pModel->m_name);

    if(m_pModel->m_select > 1){
        ui->exitQueueImg->setPixmap(QPixmap(":/resource/button/exitqueue_unhold.png"));
        ui->stopOverImg->setPixmap(QPixmap(":/resource/button/stopover_hold.png"));
    }else{
        ui->exitQueueImg->setPixmap(QPixmap(":/resource/button/exitqueue_hold.png"));
        ui->stopOverImg->setPixmap(QPixmap(":/resource/button/stopover_unhold.png"));
    }
}

void RightCameraWgt::showEvent(QShowEvent *)
{
    QWidget *pWidget = static_cast<QWidget*>(this->parent());
    pWidget->activateWindow();
    pWidget->grabKeyboard(); //强制抓取键盘输入，并使当前界面获取键盘焦点。
    this->updateWidget();
}

void RightCameraWgt::hideEvent(QHideEvent *)
{
    qInfo()<<"RightCameraWgt::stopPlay";
    ui->playWgt->stopPlay();
}

void RightCameraWgt::onClDataCome(int cmd, QVariant )
{
    RightCameraCL::CLCmdType tCmd = static_cast<RightCameraCL::CLCmdType>(cmd);
    switch (tCmd) {
    case RightCameraCL::CL_UpdateAll:
    {
        this->updateWidget();
        break;
    }
    case RightCameraCL::CL_UpdateSelect:
    {
        if(m_pModel->m_select > 1){
            ui->exitQueueImg->setPixmap(QPixmap(":/resource/button/exitqueue_unhold.png"));
            ui->stopOverImg->setPixmap(QPixmap(":/resource/button/stopover_hold.png"));
        }else{
            ui->exitQueueImg->setPixmap(QPixmap(":/resource/button/exitqueue_hold.png"));
            ui->stopOverImg->setPixmap(QPixmap(":/resource/button/stopover_unhold.png"));
        }
        break;
    }
    case RightCameraCL::CL_InputKey:
    {
        QWidget *pWidget = static_cast<QWidget*>(this->parent());
        pWidget->activateWindow();
        pWidget->releaseKeyboard(); //有按键按下，完成抓取，释放全局按键抓取。
        break;
    }
    default:
        break;
    }
}

void RightCameraWgt::onModelUpdate(int cmd)
{
    RightCameraModel::ModelCmdType tCmd = static_cast<RightCameraModel::ModelCmdType>(cmd);
    switch (tCmd) {
    case RightCameraModel::MC_UpdateAll:
    {
        this->updateWidget();
        break;
    }

    default:
        break;
    }
}

