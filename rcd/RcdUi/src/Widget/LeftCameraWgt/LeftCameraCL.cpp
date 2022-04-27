#include "LeftCameraCL.h"
#include <QDebug>
#include <QSettings>

LeftCameraCL::LeftCameraCL(QObject *parent) : IController(parent)
{

}

LeftCameraCL::~LeftCameraCL()
{

}

void LeftCameraCL::doInit()
{
    qInfo()<<"LeftCameraCL::doInit()";
}

void LeftCameraCL::doUnInit()
{

}

bool LeftCameraCL::exitAction()
{
    return true;
}

bool LeftCameraCL::intoAction()
{
    QSettings config("rcdconfig.ini",QSettings::IniFormat);
    QString currImei = System::getCurrentVehicle();
    m_pModel->m_url = config.value(QString("%1/Left").arg(currImei)).toString();

    QString tmpStr(config.value(QString("%1/Name").arg(currImei)).toString());
    m_pModel->m_name = QString::fromUtf8("【%1】 左摄像头").arg(tmpStr);
    return true;
}

void LeftCameraCL::getFromAny(ModuleDir moduleDir,int cmd,QVariant data)
{
    moduleDir.clear();
    Q_UNUSED(data)
    LeftCameraWgtCmdType Cmd = static_cast<LeftCameraWgtCmdType>(cmd);

    switch (Cmd) {
    case LC_UpdateTitle:{
        break;
    }

    default:{break;}
    }

}


void LeftCameraCL::setModel(LeftCameraModel *model)
{
    m_pModel = model;
}
