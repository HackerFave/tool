#include "RightCameraCL.h"
#include <QSettings>
#include <QDebug>
#include <QMessageBox>
#include <QApplication>

RightCameraCL::RightCameraCL(QObject *parent) : IController(parent)
{

}

RightCameraCL::~RightCameraCL()
{

}

void RightCameraCL::doInit()
{
    qInfo()<<"RightCameraCL::doInit()";
}

void RightCameraCL::doUnInit()
{

}

bool RightCameraCL::exitAction()
{
    return true;
}

bool RightCameraCL::intoAction()
{
    QSettings config("rcdconfig.ini",QSettings::IniFormat);
    QString currImei = System::getCurrentVehicle();
    m_pModel->m_url = config.value(QString("%1/Right").arg(currImei)).toString();
    QString tmpStr(config.value(QString("%1/Name").arg(currImei)).toString());
    m_pModel->m_name = QString::fromUtf8("【%1】 右摄像头").arg(tmpStr);
    return true;
}

void RightCameraCL::getFromAny(ModuleDir moduleDir,int cmd,QVariant data)
{
    moduleDir.clear();
    RightCameraWgtCmdType Cmd = static_cast<RightCameraWgtCmdType>(cmd);

    switch (Cmd) {
    case RC_InputKey:{
        emit pushDataToView(CL_InputKey);
        inputHandler(static_cast<Qt::Key>(data.toInt()));
        break;
    }

    default:{break;}
    }

}

void RightCameraCL::inputHandler(Qt::Key key)
{
    qDebug()<<"RightCameraCL::inputHandler"<<key;

    if(key == Qt::Key_Escape/*Qt::Key_Up*/){
        ModuleDir moduleDir;
        moduleDir.append(MD_IPCManager);
        emit sendToMain(moduleDir,IM_ReqRemoteEnd);

        moduleDir.clear();
        moduleDir.append(MD_MainProcessor);
        emit sendToMain(moduleDir,MP_SwitchLayout,LT_Monitor);
    }else if(key == Qt::Key_Enter || key == Qt::Key_Return){
        if(m_pModel->m_select == 1){
            QMessageBox msgbox(QMessageBox::Question, QString::fromUtf8("提示"), QString::fromUtf8("是否完成了停靠？"),
                               QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No),
                               static_cast<QWidget*>(this->parent()->parent()));

            msgbox.setObjectName("MessageBox");
            msgbox.setButtonText(QMessageBox::Yes,QString::fromUtf8("是？"));
            msgbox.setButtonText(QMessageBox::No,QString::fromUtf8("否？"));
            if(msgbox.exec() == QMessageBox::Yes){
                ModuleDir moduleDir;
                moduleDir.append(MD_IPCManager);
                emit sendToMain(moduleDir,IM_ReqSendHmiMsg);
            }
        }
    }else if(key == Qt::Key_Left || key == Qt::Key_Right){
        m_pModel->m_select = m_pModel->m_select > 1 ? 1: 2;
        emit pushDataToView(CL_UpdateSelect);
    }
}


void RightCameraCL::setModel(RightCameraModel *model)
{
    m_pModel = model;
}
