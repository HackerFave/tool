#include "InteractiveManager.h"
#include <QDebug>

InteractiveManager* InteractiveManager::s_pInstance = nullptr;

InteractiveManager::InteractiveManager(QObject *parent) : IController(parent)
{
    m_focusModule = MD_MainMonitor;
    m_focusRelationship.clear();
}

InteractiveManager::~InteractiveManager()
{

}

InteractiveManager *InteractiveManager::GetInstance(QObject *parent)
{
    if(s_pInstance == nullptr) {
        s_pInstance = new InteractiveManager(parent);
    }
    return s_pInstance;
}

void InteractiveManager::getFromAny(ModuleDir moduleDir,int cmd,QVariant data)
{
    moduleDir.clear();
    InteractiveManagerCmdType Cmd = static_cast<InteractiveManagerCmdType>(cmd);//更新当前界面焦点

    switch (Cmd) {
    case IA_KeyPress:{
        Qt::Key key = static_cast<Qt::Key>(data.toInt());
        if(key == Qt::Key_PageUp || key == Qt::Key_PageDown ){
            ModuleDir module;
            module.append(MD_MainCamera);
            emit sendToMain(module,CC_KeyPress,data);
             qDebug()<<static_cast<Qt::Key>(data.toInt())<<
                       "InteractiveManager：：遥控驾驶申请--------------------------";
        }else{
            ModuleDir module;
            module.append(m_focusModule);
            emit sendToMain(module,CC_KeyPress,data); //统一命令
//            qDebug()<<static_cast<Qt::Key>(data.toInt())<<
//                      "InteractiveManager：：遥控驾驶申请--------------------------";
        }
        break;
    }
    case IA_SwitchLayout:{
        LayoutType layout = static_cast<LayoutType>(data.toInt());
        if(layout == LT_Monitor){
            m_focusModule = MD_MainMonitor;
        }else if(layout == LT_RemoteControl){
            m_focusModule = MD_RightCamera;
        }
    }
    default:{break;}
    }

}


