#include "LeftCameraWgt.h"
#include "ui_LeftCameraWgt.h"
#include <QVBoxLayout>
#include <QShowEvent>
#include <QPainter>
#include <QThread>
#include <QPixmap>
#include <QTimer>
#include <QDebug>

#include "Component/TitleWgt.h"

LeftCameraWgt* LeftCameraWgt::s_pInstance = nullptr; //初始化静态数据成员

LeftCameraWgt::LeftCameraWgt(QWidget *parent) :
    IFunWidget(parent),
    ui(new Ui::LeftCameraWgt)
{
    ui->setupUi(this);
    m_pCL = new LeftCameraCL();
    m_pCL->doInit();
    this->setCL(m_pCL);

    m_pModel = new LeftCameraModel();
    this->setModel(m_pModel);
    m_pCL->setModel(m_pModel);

    ui->leftVideoFrame1->setFrameType(TGFrame::VideoFrameLeftPage2No2);
    ui->playWgt->setShadowType(TGFrame::ShadowLeftPage2No2);

    QVBoxLayout* pVLayout = new QVBoxLayout(this);
    pVLayout->setContentsMargins(0,0,0,0);
    pVLayout->setSpacing(0);

    pVLayout->addWidget(ui->leftVideoFrame1);

    QVBoxLayout *pFrameVLayout = new QVBoxLayout(ui->leftVideoFrame1);
    pFrameVLayout->setContentsMargins(20, 20, 20, 20);
    pFrameVLayout->setSpacing(10);
    m_pTitleWgt = new TitleWgt(ui->leftVideoFrame1);
    QString str(QString::fromUtf8("【xxxx】 左摄像头"));
    m_pTitleWgt->setTitleString(str);
    pFrameVLayout->addWidget(m_pTitleWgt,1);
    pFrameVLayout->addWidget(ui->playWgt,9);
    ui->leftVideoFrame1->setLayout(pFrameVLayout);

    this->setLayout(pVLayout);
}

LeftCameraWgt::~LeftCameraWgt()
{
    delete ui;
    delete m_pCL;
    delete m_pModel;
}

LeftCameraWgt *LeftCameraWgt::GetInstance()
{
    if(s_pInstance == nullptr) {
        s_pInstance = new LeftCameraWgt();
    }
    return s_pInstance;
}

void LeftCameraWgt::initWidget()
{
    qInfo()<<"LeftCameraWgt::initWidget";
    m_pCL->intoAction();
}

bool LeftCameraWgt::isQuit()
{
    qInfo()<<"LeftCameraWgt::isQuit";
    m_pCL->exitAction();
    return true;
}

void LeftCameraWgt::updateWidget()
{
    qInfo()<<"LeftCameraWgt::updateWidget";
    ui->playWgt->startPlay(m_pModel->m_url);
    m_pTitleWgt->setTitleString(m_pModel->m_name);
}

void LeftCameraWgt::showEvent(QShowEvent *)
{
    m_pCL->intoAction();
    this->updateWidget();
}

void LeftCameraWgt::hideEvent(QHideEvent *)
{
    qInfo()<<"LeftCameraWgt::stopPlay";
    ui->playWgt->stopPlay();
}

void LeftCameraWgt::onClDataCome(int cmd, QVariant data)
{
    data.isValid();
    switch (cmd) {
    case 1:
    {
        this->updateWidget();
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

void LeftCameraWgt::onModelUpdate(int cmd)
{
    LeftCameraModel::ModelCmdType tCmd = static_cast<LeftCameraModel::ModelCmdType>(cmd);
    switch (tCmd) {
    case LeftCameraModel::MC_UpdateAll:
    {
        this->updateWidget();
        break;
    }

    default:
        break;
    }
}

