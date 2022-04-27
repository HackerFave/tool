#include "System.h"
#include <QApplication>
#include <QTime>
#include <QSharedMemory>
#include "Interface/WgtFactory.h"
#include <QDebug>

#define KEY_SHARED_MEMORY  "RcdSysShareData"

System *System::m_pInstance = nullptr;

System::System() : m_processType(PT_All),m_pSharedMemory(nullptr)
{
    qRegisterMetaType<ModuleDir>("ModuleDir");
    memset(&m_sysShareData,0,sizeof(m_sysShareData));
    m_pSharedMemory = new QSharedMemory();
}

System::~System()
{
    m_pSharedMemory->detach();
    delete m_pSharedMemory;
}

System *System::getInstance()
{
    if(m_pInstance == nullptr){
        m_pInstance = new System();
    }
    return m_pInstance;
}

void System::createSharedMemory()
{
    m_pSharedMemory->setKey(KEY_SHARED_MEMORY);
    if(m_processType == PT_MidProc){
        if(m_pSharedMemory->isAttached()){
            m_pSharedMemory->detach();
        }
        if(!m_pSharedMemory->create(sizeof(SharedDatas))){
            qWarning() << "Unable to create SharedMemory:" << m_pSharedMemory->errorString();
        }
    }
}

void System::readSharedMemory()
{
    if(!m_pSharedMemory->isAttached()){
        if(!m_pSharedMemory->attach(QSharedMemory::ReadOnly)){
            qWarning() << "Read attach error:"<< m_pSharedMemory->errorString();
            return;
        }
    }
    m_pSharedMemory->lock();
    memcpy(&m_sysShareData,m_pSharedMemory->constData(),sizeof(SharedDatas));
    m_pSharedMemory->unlock();

//    if(m_processType != PT_MidProc){
//        m_pSharedMemory->detach();
//    }
}

void System::writeSharedMemory()
{
    if(!m_pSharedMemory->isAttached()){
        if(!m_pSharedMemory->attach(QSharedMemory::ReadWrite)){
            qWarning() << "Write attach error:"<< m_pSharedMemory->errorString();
            return;
        }
    }
    m_pSharedMemory->lock();
    memcpy(m_pSharedMemory->data(),&m_sysShareData,sizeof(SharedDatas));
    m_pSharedMemory->unlock();

//    if(m_processType != PT_MidProc){
//        m_pSharedMemory->detach();
//    }
}

void System::delayMsecTime(int msecTime,int delayModel) //delayModel=0 阻塞模式延时 delayModel!=0 非阻塞模式延时
{
    if(delayModel > 0){
        QTime n = QTime::currentTime();
        QTime now;
        do{
            now = QTime::currentTime();
            QCoreApplication::processEvents(QEventLoop::AllEvents,100);
        }
        while(n.msecsTo(now)<=msecTime);
    }else{
        QTime n = QTime::currentTime();
        QTime now;
        do{
            now = QTime::currentTime();
        }
        while(n.msecsTo(now)<=msecTime);
    }
}

ProcessType System::belongTo(ModuleType mdType)
{
    ProcessType type = PT_All;
    if(mdType >= MD_LeftCamera && mdType <= MD_CloudWeb){
        type = PT_LeftProc;
    }else if(mdType >= MD_MainCamera && mdType <= MD_MainMonitor){
        type = PT_MidProc;
    }else if(mdType >= MD_RightCamera && mdType <= MD_WarnInfo){
        type = PT_RightProc;
    }
    return type;
}

bool System::messageFilter(ModuleType module)
{
    bool ret = false;
    if(getInstance()->m_processType & belongTo(module)){
        ret = !(WgtFactory::getFunWidget(module)->isHidden());
    }
    return ret;
}

bool System::messageFilter(ModuleType module,QString &imei)
{
    if(messageFilter(module)){
        return (QString(getInstance()->m_sysShareData.vehicleImei) == imei);
    }else{
        return false;
    }
}

void System::setCurrentVehicle(QString &imei)
{
    if(imei.count() < 16){
        strcpy(getInstance()->m_sysShareData.vehicleImei,imei.toUtf8().data());
        if(getInstance()->m_processType != PT_All){
            getInstance()->writeSharedMemory();
        }
    }
}

QString System::getCurrentVehicle()
{
    if(getInstance()->m_processType != PT_All){
        getInstance()->readSharedMemory();
    }
    return QString(getInstance()->m_sysShareData.vehicleImei);
}

void System::setProcessType(ProcessType type)
{
    getInstance()->m_processType = type;
    if(type != PT_All){
        getInstance()->createSharedMemory();
    }
}

int System::getProcessType()
{
    return getInstance()->m_processType;
}

System::SharedDatas* System::getSharedDatas()
{
    return &getInstance()->m_sysShareData;
}

