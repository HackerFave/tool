#include "MainMonitorCL.h"
#include "../include/qtzmq.h"
#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#include <QApplication>
#include <QDesktopWidget>
#include <QPushButton>
#include "public/saveGifForm.h"
#include "../messageBox/messageBoxForm.h"
#include "../messageBox/messageBox.h"
#define MAXPLAYWGT  6

MainMonitorCL::MainMonitorCL(QObject *parent) : IController(parent)
{

}

MainMonitorCL::~MainMonitorCL()
{

}

void MainMonitorCL::doInit()
{
    qInfo()<<"MainMonitorCL::doInit()";
}

void MainMonitorCL::doUnInit()
{

}

bool MainMonitorCL::exitAction()
{
    return true;
}

bool MainMonitorCL::intoAction()
{
    this->refreshDisplayData();
    return true;
}

void MainMonitorCL::getFromAny(ModuleDir moduleDir,int cmd,QVariant data)
{
    moduleDir.clear();
    MainMonitorWgtCmdType Cmd = static_cast<MainMonitorWgtCmdType>(cmd);

    switch (Cmd) {
    case MM_InputKey:{
        emit pushDataToView(CL_InputKey);
        inputHandler(static_cast<Qt::Key>(data.toInt()));
        break;
    }
    case MM_VehicleStatus:{
        vehicleStatusHandler(data);
        break;
    }

    default:{break;}
    }

}

void MainMonitorCL::vehicleStatusHandler(QVariant &data)
{
    VehicleStatus status = data.value<VehicleStatus>();
    MainMonitorModel::VehicleInfo tVehicle;
    tVehicle.imei = QLatin1String(reinterpret_cast<char*>(status.imei),IMEI_LENGTH);

    switch (status.status) {
    case VEHICLE_STATUS_CONNECTED:{
        QSettings config("rcdconfig.ini",QSettings::IniFormat);
        QVariant value(config.value(QString("%1/Forward").arg(tVehicle.imei)));
        if(value.isValid()){
            tVehicle.name = config.value(QString("%1/Name").arg(tVehicle.imei)).toString();
            tVehicle.url = value.toString();
        }
        if(m_pModel->addVehicle(tVehicle)){
            this->refreshDisplayData();
            emit pushDataToView(CL_UpdateAll);
        }
        break;
    }
    case VEHICLE_STATUS_DISCONNECTED:{
        if(m_pModel->removeVehicle(tVehicle.imei)){
            this->refreshDisplayData();
            emit pushDataToView(CL_UpdateAll);
        }
        break;
    }
    case VEHICLE_STATUS_NO_CAN:
        qDebug() << "VEHICLE_STATUS_NO_CAN";
        break;
    case VEHICLE_STATUS_UNKNOWN:
        qDebug() << "VEHICLE_STATUS_UNKNOWN";
        break;
    default:
        break;
    }

}

void MainMonitorCL::inputHandler(Qt::Key key)
{
    int flag = 0;

    switch (key) {
    case Qt::Key_Up:{
//        flag = 4;//返回
        break;
        //        flag = 1;
        //        if(m_pModel->m_select < 3){
        //            if(m_pModel->m_pageIndex > 0){
        //                m_pModel->m_pageIndex -= 1;
        //                flag = 2;
        //            }
        //        }else{
        //            m_pModel->m_select -= 3;
        //        }
        //        break;
    }
    case Qt::Key_Down:{
//        flag = 3;//确认
        break;
        //        flag = 1;
        //        if(m_pModel->m_select < 3){
        //            m_pModel->m_select += 3;
        //        }else{
        //            if(m_pModel->m_pageIndex < m_pModel->m_maxPage - 1){
        //                m_pModel->m_pageIndex += 1;
        //                flag = 2;
        //            }
        //        }
        //        break;
    }
    case Qt::Key_Left:{
        flag = 1;
        if(m_pModel->m_select < 1){
            if(m_pModel->m_pageIndex > 0){
                m_pModel->m_pageIndex -= 1;
                flag = 2;
            }
            m_pModel->m_select = 5;
        }else{
            m_pModel->m_select -= 1;
        }
        break;
    }
    case Qt::Key_Right:{
        flag = 1;
        if(m_pModel->m_select < 5){
            m_pModel->m_select += 1;
        }else{
            if(m_pModel->m_pageIndex < m_pModel->m_maxPage - 1){
                m_pModel->m_pageIndex += 1;
                flag = 2;
            }
            m_pModel->m_select = 0;
        }
        break;
    }
    case Qt::Key_Enter:
    case Qt::Key_Return:{
                 flag = 3;
        break;
    }
    case Qt::Key_Escape:{
                flag = 4;
        break;
    }
    case Qt::Key_Space:{
        flag = 5;
        break;
    }
    case Qt::Key_Z:{
        flag = 6;
        break;
    }
    default:
        qDebug() << "not used this key:" << key;
        break;
    }
    if(flag == 1){
        emit pushDataToView(CL_UpdateSelect);
    }else if(flag == 2){
        this->refreshDisplayData();
        emit pushDataToView(CL_UpdateAll);
    }else if(flag == 3){
        QString &imei = m_pModel->m_showVehicle[m_pModel->m_select];
        if(!imei.isEmpty()){

            QMessageBox msgbox(QMessageBox::Question, QString::fromUtf8("提示"), QString::fromUtf8("是否进入遥控驾驶界面？"),
                               QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No),
                               static_cast<QWidget*>(this->parent()->parent()));
            messageBox *msgboxf = static_cast<messageBox*>(&msgbox);
            msgboxf->setObjectName("MessageBox");
            msgboxf->setButtonText(QMessageBox::Yes,QString::fromUtf8("是？"));

            msgboxf->setButtonText(QMessageBox::No,QString::fromUtf8("否？"));
            if(msgboxf->exec() == QMessageBox::Yes){
                System::setCurrentVehicle(m_pModel->m_showVehicle[m_pModel->m_select]);
                ModuleDir moduleDir;
                moduleDir.append(MD_MainProcessor);
                emit sendToMain(moduleDir,MP_SwitchLayout,LT_RemoteControl);
            }

        }
    }else if(flag == 4){
        QMessageBox msgbox(QMessageBox::Question, QString::fromUtf8("提示"), QString::fromUtf8("是否退出程序？"),
                           QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No),
                           static_cast<QWidget*>(this->parent()->parent()));

        msgbox.setObjectName("MessageBox");
        msgbox.setButtonText(QMessageBox::Yes,QString::fromUtf8("是？"));
        msgbox.setButtonText(QMessageBox::No,QString::fromUtf8("否？"));
        if(msgbox.exec() == QMessageBox::Yes){
            QApplication::exit();
        }

    }else if(flag == 5){

        saveGifForm::instance()->showForm();
        MoviePlayer::instance()->close();
    }
    else if (6 == flag) {
        saveGifForm::instance()->close();
        MoviePlayer::instance()->showMaximized();
    }
}

void MainMonitorCL::refreshDisplayData()
{
    int total = m_pModel->m_vehicleList.count();
    m_pModel->m_maxPage = total/MAXPLAYWGT;
    m_pModel->m_maxPage += total%MAXPLAYWGT ? 1 : 0;

    if(m_pModel->m_pageIndex >= m_pModel->m_maxPage){
        m_pModel->m_pageIndex = 0;
        if(m_pModel->m_maxPage > 0){
            m_pModel->m_pageIndex = m_pModel->m_maxPage - 1;
        }
    }

    int pageIndex = m_pModel->m_pageIndex*MAXPLAYWGT;

    for(int i = 0;i < MAXPLAYWGT;i++){
        if(pageIndex + i < total){
            m_pModel->m_showVehicle[i] = m_pModel->m_vehicleList[pageIndex+i];
        }else{
            m_pModel->m_showVehicle[i].clear();
        }
    }
}

void MainMonitorCL::setModel(MainMonitorModel *model)
{
    m_pModel = model;
}
