#include "DriveCameraWgt.h"
#include "ui_DriveCameraWgt.h"
#include <QVBoxLayout>
#include <QShowEvent>
#include <QPainter>
#include <QThread>
#include <QPixmap>
#include <QTimer>
#include <QSettings>
#include <QDebug>

#include "Component/TitleWgt.h"

DriveCameraWgt* DriveCameraWgt::s_pInstance = nullptr; //初始化静态数据成员

DriveCameraWgt::DriveCameraWgt(QWidget *parent) :
    IFunWidget(parent),
    ui(new Ui::DriveCameraWgt)
{
    ui->setupUi(this);
    m_pCL = new DriveCameraCL();
    m_pCL->doInit();
    this->setCL(m_pCL);

    ui->leftVideoFrame1->setFrameType(TGFrame::VideoFrameLeftPage2No1);
    ui->playWgt->setShadowType(TGFrame::ShadowLeftPage2No1);

    QVBoxLayout* pVLayout = new QVBoxLayout(this);
    pVLayout->setContentsMargins(0,0,0,0);
    pVLayout->setSpacing(0);

    pVLayout->addWidget(ui->leftVideoFrame1);

    QVBoxLayout *pFrameVLayout = new QVBoxLayout(ui->leftVideoFrame1);
    pFrameVLayout->setContentsMargins(20, 20, 20, 20);
    pFrameVLayout->setSpacing(10);
    m_pTitleWgt = new TitleWgt(ui->leftVideoFrame1);
    QString str(QString::fromUtf8("【xxxx】 驾驶舱摄像头"));
    m_pTitleWgt->setTitleString(str);
    pFrameVLayout->addWidget(m_pTitleWgt,1);
    pFrameVLayout->addWidget(ui->playWgt,9);
    ui->leftVideoFrame1->setLayout(pFrameVLayout);

    this->setLayout(pVLayout);
}

DriveCameraWgt::~DriveCameraWgt()
{
    delete ui;
    delete m_pCL;
}

DriveCameraWgt *DriveCameraWgt::GetInstance()
{
    if(s_pInstance == nullptr) {
        s_pInstance = new DriveCameraWgt();
    }
    return s_pInstance;
}

void DriveCameraWgt::initWidget()
{
    qInfo()<<"DriveCameraWgt::initWidget";
    m_pCL->intoAction();
}

bool DriveCameraWgt::isQuit()
{
    qInfo()<<"DriveCameraWgt::isQuit";
    m_pCL->exitAction();
    return true;
}

void DriveCameraWgt::updateWidget()
{
    qInfo()<<"DriveCameraWgt::updateWidget";
    QSettings config("rcdconfig.ini",QSettings::IniFormat);
    QString currImei = System::getCurrentVehicle();
    QString tmpStr = config.value(QString("%1/Drive").arg(currImei)).toString();
    ui->playWgt->startPlay(tmpStr);
    tmpStr = config.value(QString("%1/Name").arg(currImei)).toString();
    tmpStr = QString::fromUtf8("【%1】 驾驶舱摄像头").arg(tmpStr);
    m_pTitleWgt->setTitleString(tmpStr);
}

void DriveCameraWgt::showEvent(QShowEvent *)
{
    this->updateWidget();
}

void DriveCameraWgt::hideEvent(QHideEvent *)
{
    qInfo()<<"DriveCameraWgt::stopPlay";
    ui->playWgt->stopPlay();
}


void DriveCameraWgt::onClDataCome(int cmd, QVariant data)
{
    data.isValid();
    switch (cmd) {
    case 1:
    {
        break;
    }
    case 2:
    {

        break;
    }

    case 3:
    {

        break;
    }

    default:
        break;
    }
}

