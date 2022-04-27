#include "DriveCameraCL.h"
#include <QDebug>

DriveCameraCL::DriveCameraCL(QObject *parent) : IController(parent)
{

}

DriveCameraCL::~DriveCameraCL()
{

}

void DriveCameraCL::doInit()
{
    qInfo()<<"DriveCameraCL::doInit()";
}

void DriveCameraCL::doUnInit()
{

}

bool DriveCameraCL::exitAction()
{
    return true;
}

bool DriveCameraCL::intoAction()
{

    return true;
}

void DriveCameraCL::getFromAny(ModuleDir moduleDir,int cmd,QVariant )
{
    moduleDir.clear();
    DriveCameraWgtCmdType Cmd = static_cast<DriveCameraWgtCmdType>(cmd);

    switch (Cmd) {
    case DC_UpdateUrl:{
        break;
    }

    default:{break;}
    }

}

