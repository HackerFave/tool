#include "AllInfoWgt.h"
#include "ui_AllInfoWgt.h"
#include <QVBoxLayout>
#include <QDebug>

#include "Component/Dashboard.h"
#include "Component/AnalogClock.h"

AllInfoWgt* AllInfoWgt::s_pInstance = nullptr; //初始化静态数据成员

AllInfoWgt::AllInfoWgt(QWidget *parent) :
    IFunWidget(parent),
    ui(new Ui::AllInfoWgt)
{
    ui->setupUi(this);
    m_pCL = new AllInfoCL();
    m_pCL->doInit();
    this->setCL(m_pCL);

    m_pModel = new AllInfoModel();
    this->setModel(m_pModel);
    m_pCL->setModel(m_pModel);

    ui->rightTitle->setTitleString(QString::fromUtf8("矿卡数据"));
    ui->rightTitle->setFrameType(TGFrame::InfoFrameRightPage1No1);

    QVBoxLayout* pVLayout = new QVBoxLayout(this);
    pVLayout->setContentsMargins(0,0,0,0);
    pVLayout->setSpacing(0);
    pVLayout->addStretch(3);
    pVLayout->addWidget(ui->rightTitle,94);
    pVLayout->addStretch(3);
    this->setLayout(pVLayout);
}

AllInfoWgt::~AllInfoWgt()
{
    delete ui;
    delete m_pCL;
    delete m_pModel;
}

AllInfoWgt *AllInfoWgt::GetInstance()
{
    if(s_pInstance == nullptr) {
        s_pInstance = new AllInfoWgt();
    }
    return s_pInstance;
}

void AllInfoWgt::initWidget()
{
    qInfo()<<"AllInfoWgt::initWidget";
    m_pCL->intoAction();
    this->updateWidget();
}

bool AllInfoWgt::isQuit()
{
    qInfo()<<"AllInfoWgt::isQuit";
    m_pCL->exitAction();
    return true;
}

void AllInfoWgt::updateWidget()
{
    qInfo()<<"AllInfoWgt::updateWidget";
}

void AllInfoWgt::onClDataCome(int cmd, QVariant data)
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

void AllInfoWgt::onModelUpdate(int cmd)
{
    AllInfoModel::ModelCmdType tCmd = static_cast<AllInfoModel::ModelCmdType>(cmd);
    switch (tCmd) {
    case AllInfoModel::MC_UpdateAll:
    {
        this->updateWidget();
        break;
    }
    case AllInfoModel::MC_LoadUrl:
    {

        break;
    }

    default:
        break;
    }
}

