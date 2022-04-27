#include "WarnInfoWgt.h"
#include "ui_WarnInfoWgt.h"
#include <QVBoxLayout>
#include <QDebug>

#include "Component/Dashboard.h"
#include "Component/AnalogClock.h"

WarnInfoWgt* WarnInfoWgt::s_pInstance = nullptr; //初始化静态数据成员

WarnInfoWgt::WarnInfoWgt(QWidget *parent) :
    IFunWidget(parent),
    ui(new Ui::WarnInfoWgt)
{
    ui->setupUi(this);
    m_pCL = new WarnInfoCL();
    m_pCL->doInit();
    this->setCL(m_pCL);

    m_pModel = new WarnInfoModel();
    this->setModel(m_pModel);
    m_pCL->setModel(m_pModel);

    QVBoxLayout* pVLayout = new QVBoxLayout(this);
    pVLayout->setContentsMargins(0,0,0,0);
    pVLayout->setSpacing(0);

    ui->rightTitle->setTitleString(QString::fromUtf8("异常信息"));
    ui->rightTitle->setFrameType(TGFrame::InfoFrameRightPage1No2);

    pVLayout->addStretch(3);
    pVLayout->addWidget(ui->rightTitle,94);
    pVLayout->addStretch(3);

    this->setLayout(pVLayout);
}

WarnInfoWgt::~WarnInfoWgt()
{
    delete ui;
    delete m_pCL;
    delete m_pModel;
}

WarnInfoWgt *WarnInfoWgt::GetInstance()
{
    if(s_pInstance == nullptr) {
        s_pInstance = new WarnInfoWgt();
    }
    return s_pInstance;
}

void WarnInfoWgt::initWidget()
{
    qInfo()<<"WarnInfoWgt::initWidget";
    m_pCL->intoAction();
    this->updateWidget();
}

bool WarnInfoWgt::isQuit()
{
    qInfo()<<"WarnInfoWgt::isQuit";
    m_pCL->exitAction();
    return true;
}

void WarnInfoWgt::updateWidget()
{
    qInfo()<<"WarnInfoWgt::updateWidget";
}

void WarnInfoWgt::onClDataCome(int cmd, QVariant data)
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

void WarnInfoWgt::onModelUpdate(int cmd)
{
    WarnInfoModel::ModelCmdType tCmd = static_cast<WarnInfoModel::ModelCmdType>(cmd);
    switch (tCmd) {
    case WarnInfoModel::MC_UpdateAll:
    {
        this->updateWidget();
        break;
    }
    case WarnInfoModel::MC_LoadUrl:
    {

        break;
    }

    default:
        break;
    }
}

