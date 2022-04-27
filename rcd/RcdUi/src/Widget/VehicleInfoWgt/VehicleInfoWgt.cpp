#include "VehicleInfoWgt.h"
#include "ui_VehicleInfoWgt.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

#include "Component/Dashboard.h"
#include "Component/AnalogClock.h"

VehicleInfoWgt* VehicleInfoWgt::s_pInstance = nullptr; //初始化静态数据成员

VehicleInfoWgt::VehicleInfoWgt(QWidget *parent) :
    IFunWidget(parent),
    ui(new Ui::VehicleInfoWgt)
{
    ui->setupUi(this);
    m_pCL = new VehicleInfoCL();
    m_pCL->doInit();
    this->setCL(m_pCL);

    m_pModel = new VehicleInfoModel();
    this->setModel(m_pModel);
    m_pCL->setModel(m_pModel);

    ui->rightTitleFrame1->setFrameType(TGFrame::InfoFrameRightPage2No1);
    ui->rightTitleFrame2->setFrameType(TGFrame::InfoFrameRightPage2No4);
    ui->rightTitleFrame1->setTitleString(QString::fromUtf8("车辆信息"));
    ui->rightTitleFrame2->setTitleString(QString::fromUtf8("系统信息"));

    QVBoxLayout* pVLayout = new QVBoxLayout(this);
    pVLayout->setContentsMargins(0,0,0,0);
    pVLayout->setSpacing(0);

    pVLayout->addStretch(2);
    pVLayout->addWidget(ui->rightTitleFrame1,62);
    pVLayout->addStretch(3);
    pVLayout->addWidget(ui->rightTitleFrame2,35);
    pVLayout->addStretch(2);

    ui->vehicleWgt->setParameter(Dashboard::Volmeter,0,100);
    ui->vehicleWgt_2->setParameter(Dashboard::OilMeter,0,100);
    ui->vehicleWgt_3->setParameter(Dashboard::OilPressureMeter,0,100);
    ui->vehicleWgt_4->setParameter(Dashboard::CoolantTemperature,0,100);
    ui->scrollArea->setStyleSheet("QWidget {background-color:transparent;}");//transparent

//    p_mapWidget = new QtSimpleMap;
//    QGridLayout * layout = new QGridLayout;
//    layout->setMargin(0);

//    layout->addWidget(p_mapWidget);
//    ui->rightTitleFrame1->setLayout(layout);
    /**********隐藏历史控件***************/
//    ui->vehicleWgt->setVisible(false);
//    ui->vehicleWgt_2->setVisible(false);
//    ui->vehicleWgt_3->setVisible(false);
//    ui->vehicleWgt_4->setVisible(false);
//    ui->map_widget->setVisible(false);
    /**************************地图***************/

    this->setLayout(pVLayout);

    pVLayout = new QVBoxLayout(ui->sysInfoWgt);
    pVLayout->setContentsMargins(0,0,0,0);
    pVLayout->setSpacing(0);
    pVLayout->addStretch(1);
    pVLayout->addWidget(ui->sysInfo,9);
    ui->sysInfo->document()->setMaximumBlockCount(10000);
    ui->sysInfo->insertPlainText(m_pModel->m_allInfo);
    //    _wMapTest = new  MapWidget;
    //    ui->rightTitleFrame1->setVisible(false);

}

VehicleInfoWgt::~VehicleInfoWgt()
{
    delete ui;
    delete m_pCL;
    delete m_pModel;
}

VehicleInfoWgt *VehicleInfoWgt::GetInstance()
{
    if(s_pInstance == nullptr) {
        s_pInstance = new VehicleInfoWgt();
    }
    return s_pInstance;
}

void VehicleInfoWgt::initWidget()
{
    qInfo()<<"VehicleInfoWgt::initWidget";
    m_pCL->intoAction();
    this->updateWidget();


}

bool VehicleInfoWgt::isQuit()
{
    qInfo()<<"VehicleInfoWgt::isQuit";
    m_pCL->exitAction();
    ui->sysInfo->clear();
    return true;
}

void VehicleInfoWgt::updateWidget()
{
    qInfo() << "VehicleInfoWgt::updateWidget";
    ui->sysInfo->moveCursor(QTextCursor::End);
}

void VehicleInfoWgt::onClDataCome(int cmd, QVariant data)
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

void VehicleInfoWgt::onModelUpdate(int cmd)
{
    VehicleInfoModel::ModelCmdType tCmd = static_cast<VehicleInfoModel::ModelCmdType>(cmd);
    switch (tCmd) {
    case VehicleInfoModel::MC_UpdateAll:
    {
        this->updateWidget();
        break;
    }
    case VehicleInfoModel::MC_AddSysInfo:
    {
        ui->sysInfo->insertPlainText(m_pModel->m_newInfo + "\r\n");
        ui->sysInfo->moveCursor(QTextCursor::End);
        break;
    }
    case VehicleInfoModel::MC_Oil://油量
    {
        ui->vehicleWgt_2->setValue(m_pModel->_oilvalue);
        break;
    }
    default:
        break;
    }
}

