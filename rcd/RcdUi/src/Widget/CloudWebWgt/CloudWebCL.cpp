#include "CloudWebCL.h"
#include <QDebug>

CloudWebCL::CloudWebCL(QObject *parent) : IController(parent)
{

}

CloudWebCL::~CloudWebCL()
{

}

void CloudWebCL::doInit()
{
    qInfo()<<"CloudWebCL::doInit()";
}

void CloudWebCL::doUnInit()
{

}

bool CloudWebCL::exitAction()
{
    return true;
}

bool CloudWebCL::intoAction()
{

    return true;
}

void CloudWebCL::getFromAny(ModuleDir moduleDir,int cmd,QVariant data)
{
    moduleDir.clear();
    CloudWebWgtCmdType Cmd = static_cast<CloudWebWgtCmdType>(cmd);

    switch (Cmd) {
    case CW_LoadNewUrl:{
        QString url = data.toString();
        m_pModel->setUrl(url);
        break;
    }

    default:{break;}
    }

}


void CloudWebCL::setModel(CloudWebModel *model)
{
    m_pModel = model;
}
