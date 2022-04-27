#include "AllInfoCL.h"
#include <QDebug>

AllInfoCL::AllInfoCL(QObject *parent) : IController(parent)
{

}

AllInfoCL::~AllInfoCL()
{

}

void AllInfoCL::doInit()
{
    qInfo()<<"AllInfoCL::doInit()";
}

void AllInfoCL::doUnInit()
{

}

bool AllInfoCL::exitAction()
{
    return true;
}

bool AllInfoCL::intoAction()
{

    return true;
}

void AllInfoCL::getFromAny(ModuleDir moduleDir,int cmd,QVariant data)
{
    moduleDir.clear();
    AllInfoWgtCmdType Cmd = static_cast<AllInfoWgtCmdType>(cmd);

    switch (Cmd) {
    case AI_LoadNewUrl:{
        QString url = data.toString();
        m_pModel->setUrl(url);
        break;
    }

    default:{break;}
    }

}


void AllInfoCL::setModel(AllInfoModel *model)
{
    m_pModel = model;
}
