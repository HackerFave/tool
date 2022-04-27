#include "CloudWebWgt.h"
#include "ui_CloudWebWgt.h"
#include <QScreen>
#include <QVBoxLayout>
#include <QDebug>

#include "Component/WebEngine.h"

CloudWebWgt* CloudWebWgt::s_pInstance = nullptr; //初始化静态数据成员

CloudWebWgt::CloudWebWgt(QWidget *parent) :
    IFunWidget(parent),
    ui(new Ui::CloudWebWgt)
{
    ui->setupUi(this);
    m_pCL = new CloudWebCL();
    m_pCL->doInit();
    this->setCL(m_pCL);

    m_pModel = new CloudWebModel();
    this->setModel(m_pModel);
    m_pCL->setModel(m_pModel);

    ui->webFrame1->setTitleString(QString::fromUtf8("行车地图"));
    ui->webFrame1->setFrameType(TGFrame::InfoFrameLeftPage1No1);

    m_pWeb = new WebEngine();
    QVBoxLayout* pVLayout = new QVBoxLayout(ui->webFrame1);
    pVLayout->setContentsMargins(20,50,20,20);
    pVLayout->setSpacing(6);

    pVLayout->addWidget(m_pWeb);
    ui->webFrame1->setLayout(pVLayout);


    pVLayout = new QVBoxLayout(this);
    pVLayout->setContentsMargins(0, 0, 0, 0);
    pVLayout->setSpacing(0);

    pVLayout->addWidget(ui->webFrame1);
    this->setLayout(pVLayout);

}

CloudWebWgt::~CloudWebWgt()
{
    delete ui;
    delete m_pCL;
    delete m_pModel;
}

CloudWebWgt *CloudWebWgt::GetInstance()
{
    if(s_pInstance == nullptr) {
        s_pInstance = new CloudWebWgt();
    }
    return s_pInstance;
}

void CloudWebWgt::resizeEvent(QResizeEvent *event)
{
    QScreen *pScreen = QGuiApplication::primaryScreen();
    if(event->size().height() > pScreen->size().height()/2){
        ui->webFrame1->setFrameType(TGFrame::InfoFrameLeftPage1No1);
    }else{
        ui->webFrame1->setFrameType(TGFrame::InfoFrameLeftPage2No1);
    }
}

void CloudWebWgt::initWidget()
{
    qInfo()<<"CloudWebWgt::initWidget";
    m_pCL->intoAction();
    this->updateWidget();
}

bool CloudWebWgt::isQuit()
{
    qInfo()<<"CloudWebWgt::isQuit";
    m_pCL->exitAction();
    return true;
}

void CloudWebWgt::updateWidget()
{
    qInfo()<<"CloudWebWgt::updateWidget";
    m_pWeb->load(m_pModel->m_url);
}

void CloudWebWgt::onClDataCome(int cmd, QVariant data)
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

void CloudWebWgt::onModelUpdate(int cmd)
{
    CloudWebModel::ModelCmdType tCmd = static_cast<CloudWebModel::ModelCmdType>(cmd);
    switch (tCmd) {
    case CloudWebModel::MC_UpdateAll:
    {
        this->updateWidget();
        break;
    }
    case CloudWebModel::MC_LoadUrl:
    {
        m_pWeb->load(m_pModel->m_url);
        break;
    }

    default:
        break;
    }
}

