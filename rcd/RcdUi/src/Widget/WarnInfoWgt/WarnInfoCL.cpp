#include "WarnInfoCL.h"
#include <QDebug>

WarnInfoCL::WarnInfoCL(QObject *parent) : IController(parent)
{

}

WarnInfoCL::~WarnInfoCL()
{

}

void WarnInfoCL::doInit()
{
    qInfo()<<"WarnInfoCL::doInit()";
}

void WarnInfoCL::doUnInit()
{

}

bool WarnInfoCL::exitAction()
{
    return true;
}

bool WarnInfoCL::intoAction()
{

    return true;
}

void WarnInfoCL::getFromAny(ModuleDir moduleDir,int cmd,QVariant data)
{
    moduleDir.clear();
    WarnInfoWgtCmdType Cmd = static_cast<WarnInfoWgtCmdType>(cmd);

    switch (Cmd) {
    case VI_LoadNewUrl:{
        QString url = data.toString();
        m_pModel->setUrl(url);
        break;
    }

    default:{break;}
    }

}


void WarnInfoCL::setModel(WarnInfoModel *model)
{
    m_pModel = model;
}
