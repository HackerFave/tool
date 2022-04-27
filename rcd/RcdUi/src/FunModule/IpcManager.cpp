#include "IpcManager.h"
#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include "../Protocol/ZmqMsgPackage.h"
#include "../Protocol/HmiCmdPackage.h"
//#include "zmq_interface.h"
IpcManager* IpcManager::s_pInstance = nullptr;

IpcManager::IpcManager(QObject *parent) : IController(parent),
    m_pSubControl(nullptr),m_pSubGateway(nullptr),m_pPubUI(nullptr),
    m_pPubUIL(nullptr),m_pPubUIR(nullptr),m_pPubUIM(nullptr),
    m_pSubUIL(nullptr),m_pSubUIR(nullptr),m_pSubUIM(nullptr)
{

    /********************测试新版本zmq数据接口**********************/
//    connect(initInstance(),SIGNAL(signalSubTopicData(const void*,int,const uint32_t & )),
//            this,SLOT(slotTestZmq(const void*,int,const uint32_t &)));

    connect(&_testTimer,SIGNAL(timeout()),this,SLOT(slotTimerTest()));
//    _testTimer.start(3000);
    /******************************************************************/

    int procType = System::getProcessType();
    // 与外设进程，网关进程通讯配置部分
    m_pSubControl = new ZmqSubscriber();
    m_pSubControl->ConnectTo(ZMQ_ADDR_CONTROL);
    m_pSubControl->Subscribe(ZMQ_TOPIC_BUTTON, IpcManager::ctrlButtonHandler);

    if(procType & PT_MidProc){

        m_pSubGateway = new ZmqSubscriber();
        m_pSubGateway->ConnectTo(ZMQ_ADDR_Gateway);
        m_pSubGateway->Subscribe(ZMQ_TOPIC_VEHICLE_RAWDATA, IpcManager::vehicleRawDataHandler);
        m_pSubGateway->Subscribe(ZMQ_TOPIC_VEHICLE_STATUS, IpcManager::vehicleStatusHandler);
        m_pSubGateway->Subscribe(ZMA_TOPIC_VEHICLE_DELAY, IpcManager::vehicleDelayHandler);
        m_pSubGateway->Subscribe(ZMQ_TOPIC_RESP_REMOTE_CONTROL, IpcManager::respRemotelHandler);
        m_pSubGateway->Subscribe(ZMQ_TOPIC_REPORT_HMI_MSG, IpcManager::respHmimsgHandler);

        m_pPubUI =  new ZmqPublisher(ZMQ_ADDR_UI);
    }
    // 与各个屏幕进程通讯配置部分
    if(procType == PT_MidProc){
        m_pSubUIL = new ZmqSubscriber();
        m_pSubUIL->ConnectTo(ZMQ_ADDR_UI_L);
        m_pSubUIL->Subscribe(ZMQ_TOPIC_PROC_MID, IpcManager::forwardHandler);

        m_pSubUIR = new ZmqSubscriber();
        m_pSubUIR->ConnectTo(ZMQ_ADDR_UI_R);
        m_pSubUIR->Subscribe(ZMQ_TOPIC_PROC_MID, IpcManager::forwardHandler);

        m_pPubUIM = new ZmqPublisher(ZMQ_ADDR_UI_M);
    }else if(procType == PT_RightProc){
        m_pSubUIL = new ZmqSubscriber();
        m_pSubUIL->ConnectTo(ZMQ_ADDR_UI_L);
        m_pSubUIL->Subscribe(ZMQ_TOPIC_PROC_RIGHT, IpcManager::forwardHandler);

        m_pSubUIM = new ZmqSubscriber();
        m_pSubUIM->ConnectTo(ZMQ_ADDR_UI_M);
        m_pSubUIM->Subscribe(ZMQ_TOPIC_PROC_RIGHT, IpcManager::forwardHandler);

        m_pPubUIR = new ZmqPublisher(ZMQ_ADDR_UI_R);
    }else if(procType == PT_LeftProc){
        m_pSubUIM = new ZmqSubscriber();
        m_pSubUIM->ConnectTo(ZMQ_ADDR_UI_M);
        m_pSubUIM->Subscribe(ZMQ_TOPIC_PROC_LEFT, IpcManager::forwardHandler);

        m_pSubUIR = new ZmqSubscriber();
        m_pSubUIR->ConnectTo(ZMQ_ADDR_UI_R);
        m_pSubUIR->Subscribe(ZMQ_TOPIC_PROC_LEFT, IpcManager::forwardHandler);

        m_pPubUIL = new ZmqPublisher(ZMQ_ADDR_UI_L);
    }
}

IpcManager::~IpcManager()
{
    delete m_pPubUI;
    delete m_pPubUIL;delete m_pPubUIR;delete m_pPubUIM;
    delete m_pSubUIL;delete m_pSubUIR;delete m_pSubUIM;
}

IpcManager *IpcManager::GetInstance(QObject *parent)
{
    if(s_pInstance == nullptr) {
        s_pInstance = new IpcManager(parent);
    }
    return s_pInstance;
}

void IpcManager::ctrlDataHandler(const std::string &topic, const void *data, int len)
{
    QString qtopic(QString::fromStdString(topic));
    QString qout(QString::fromUtf8(reinterpret_cast<const char *>(data), len));
    //qDebug() << "Topic:" << qtopic << " len:" << len << " Msg:" << qout ;
    ModuleDir moduleDir;
    moduleDir.append(MD_MainCamera);
}

void IpcManager::ctrlStatusHandler(const std::string &topic, const void *data, int len)
{
    QString qtopic(QString::fromStdString(topic));
    QString qout(QString::fromUtf8(reinterpret_cast<const char *>(data), len));
    //qDebug() << "Topic:" << qtopic << " len:" << len << " Msg:" << qout ;
}

void IpcManager::ctrlButtonHandler(const std::string &, const void *data, int len)
{
    ZmqMsgPackage msg;
    if(!msg.c2uButtonKey(data,len)){
        return;
    }
    for (int i = 0;i < msg.m_cmds.size(); i++) {
        QMetaObject::invokeMethod(s_pInstance,"sendToMain",
                                  Q_ARG(ModuleDir,msg.m_moduleDirs.at(i)),
                                  Q_ARG(int,msg.m_cmds.at(i)),
                                  Q_ARG(QVariant,msg.m_datas.at(i)));
        qDebug()<<msg.m_cmds.at(i)<<msg.m_datas.at(i)<<msg.m_moduleDirs.at(i)
               <<"按键×××××××××××××××××××××××××××××";
    }
}

void IpcManager::vehicleRawDataHandler(const std::string &, const void *data, int len)
{
    //QString qTopic(QString::fromStdString(topic));
    //qDebug() << "Topic:" << qTopic << " len:" << len ;
    ZmqMsgPackage msg;
    if(!msg.g2uVehicleRawData(data,len)){
        qWarning() << "ZmqMsgPackage::g2uVehicleRawData error !" ;
        return;
    }
    for (int i = 0;i < msg.m_cmds.size(); i++) {
        ModuleDir dir = msg.m_moduleDirs.at(i);
        if(System::messageFilter(static_cast<ModuleType>(dir.first()),msg.m_imei)){
            QMetaObject::invokeMethod(s_pInstance,"sendToMain",
                                      Q_ARG(ModuleDir,msg.m_moduleDirs.at(i)),
                                      Q_ARG(int,msg.m_cmds.at(i)),
                                      Q_ARG(QVariant,msg.m_datas.at(i)));
        }
    }
}

void IpcManager::vehicleStatusHandler(const std::string &, const void *data, int len)
{
    ZmqMsgPackage msg;
    if(!msg.g2uVehicleStatus(data,len)){
        qWarning() << "ZmqMsgPackage::g2uVehicleStatus error !" ;
        return;
    }
    for (int i = 0;i < msg.m_cmds.size(); i++) {
        QMetaObject::invokeMethod(s_pInstance,"sendToMain",
                                  Q_ARG(ModuleDir,msg.m_moduleDirs.at(i)),
                                  Q_ARG(int,msg.m_cmds.at(i)),
                                  Q_ARG(QVariant,msg.m_datas.at(i)));
    }
}

void IpcManager::vehicleDelayHandler(const std::string &, const void *data, int len)
{
    ZmqMsgPackage msg;
    if(!msg.g2uVehicleDelay(data,len)){
        qWarning() << "ZmqMsgPackage::g2uVehicleDelay error !" ;
        return;
    }
    for (int i = 0;i < msg.m_cmds.size(); i++) {
        ModuleDir dir = msg.m_moduleDirs.at(i);
        if(System::messageFilter(static_cast<ModuleType>(dir.first()),msg.m_imei)){
            QMetaObject::invokeMethod(s_pInstance,"sendToMain",
                                      Q_ARG(ModuleDir,msg.m_moduleDirs.at(i)),
                                      Q_ARG(int,msg.m_cmds.at(i)),
                                      Q_ARG(QVariant,msg.m_datas.at(i)));
        }
    }
}

void IpcManager::respRemotelHandler(const std::string &, const void *data, int len)
{
    ZmqMsgPackage msg;
    if(!msg.g2uRespRemotel(data,len)){
        qWarning() << "ZmqMsgPackage::g2uRespRemotel error !" ;
        return;
    }
    for (int i = 0;i < msg.m_cmds.size(); i++) {
        ModuleDir dir = msg.m_moduleDirs.at(i);
        if(System::messageFilter(static_cast<ModuleType>(dir.first()),msg.m_imei)){
            QMetaObject::invokeMethod(s_pInstance,"sendToMain",
                                      Q_ARG(ModuleDir,msg.m_moduleDirs.at(i)),
                                      Q_ARG(int,msg.m_cmds.at(i)),
                                      Q_ARG(QVariant,msg.m_datas.at(i)));
        }
    }
}
//获取数据
void IpcManager::respHmimsgHandler(const std::string &, const void *data, int len)
{
    if(sizeof(HmiMsg) != len){
        qWarning() << "respHmimsgHandler len error . len:" << len ;
        return;
    }

    HmiMsg *pHmiMsg = (HmiMsg*)data;
    QString imei(QLatin1String(reinterpret_cast<char*>(pHmiMsg->imei),IMEI_LENGTH));
    if(System::getCurrentVehicle() == imei){
        HmiCmdPackage msg;
        if(msg.hmiDecodeG2H(pHmiMsg->body,pHmiMsg->bodyLen) < 0){
            qWarning() << "HmiCmdPackage::hmiDecodeG2H error !" ;
            return;
        }
        for (int i = 0;i < msg.m_cmds.size(); i++) {
            QMetaObject::invokeMethod(s_pInstance,"sendToMain",
                                      Q_ARG(ModuleDir,msg.m_moduleDirs.at(i)),
                                      Q_ARG(int,msg.m_cmds.at(i)),
                                      Q_ARG(QVariant,msg.m_datas.at(i)));
        }
    }
}


void IpcManager::forwardHandler(const std::string &, const void *data, int len)
{
    ZmqMsgPackage msg;
    if(!msg.u2uForward(data,len)){
        return;
    }
    QMetaObject::invokeMethod(s_pInstance,"sendToMain",
                              Q_ARG(ModuleDir,msg.m_moduleDirs.first()),
                              Q_ARG(int,msg.m_cmds.first()),
                              Q_ARG(QVariant,msg.m_datas.first()));
}

void IpcManager::ipcForward(ProcessType type,ModuleDir &moduleDir, int &cmd, QVariant &data)
{

    QByteArray tDest;
    ZmqMsgPackage::encodeIpcForward(tDest,moduleDir,cmd,data);

    int procType = System::getProcessType();
    if(procType == PT_MidProc)
    {
        if(type == PT_LeftProc){
            m_pPubUIM->Publish(ZMQ_TOPIC_PROC_LEFT,tDest.constData(),tDest.length());
        }else if(type == PT_RightProc){
            m_pPubUIM->Publish(ZMQ_TOPIC_PROC_RIGHT,tDest.constData(),tDest.length());
        }
    }
    else if(procType == PT_RightProc)
    {
        if(type == PT_LeftProc){
            m_pPubUIR->Publish(ZMQ_TOPIC_PROC_LEFT,tDest.constData(),tDest.length());
        }else if(type == PT_MidProc){
            m_pPubUIR->Publish(ZMQ_TOPIC_PROC_MID,tDest.constData(),tDest.length());
        }
    }
    else if(procType == PT_LeftProc){
        if(type == PT_RightProc){
            m_pPubUIL->Publish(ZMQ_TOPIC_PROC_RIGHT,tDest.constData(),tDest.length());
        }else if(type == PT_MidProc){
            m_pPubUIL->Publish(ZMQ_TOPIC_PROC_MID,tDest.constData(),tDest.length());
        }
    }
}


void IpcManager::getFromAny(ModuleDir,int cmd,QVariant)
{
    QByteArray tDest;
    IPCManagerCmdType Cmd = static_cast<IPCManagerCmdType>(cmd);
    QString imei(System::getCurrentVehicle());


    switch (Cmd) {
    case IM_ReqRemoteStart:{
        ZmqMsgPackage::encodeReqRemote(tDest,imei,REQ_REMOTE_CONTROL_START);
         qDebug()<< static_cast<IPCManagerCmdType>(cmd)<<"IM_ReqRemoteStart---------------$$$$$$$$$$$$$$$$$$$$$";
        break;
    }
    case IM_ReqRemoteEnd:{
        ZmqMsgPackage::encodeReqRemote(tDest,imei,REQ_REMOTE_CONTROL_END);
         qDebug()<< static_cast<IPCManagerCmdType>(cmd)<<"IM_ReqRemoteEnd---------------$$$$$$$$$$$$$$$$$$$$$$$$";
        break;
    }
    case IM_ReqSendHmiMsg:{
        pubVehicleReqWorkOpt0F21ST tWorkSt;
        tWorkSt.taskType = 0;
        tWorkSt.cmdType = 0x01;
        tWorkSt.utcTime = QDateTime::currentMSecsSinceEpoch();

        uint8_t jt808Org[2048];
        int len = HmiCmdPackage::hmiEncodeReqWorkOpt(jt808Org,tWorkSt);

        QByteArray jt808Array(reinterpret_cast<char*>(jt808Org),len);
        ZmqMsgPackage::encodeReqSendHmiMsg(tDest,imei,jt808Array);

        break;
    }
    default:{return;}
    }

    m_pPubUI->Publish(ZMQ_TOPIC_REQ_REMOTE_CONTROL,tDest.constData(),tDest.length());
    qDebug()<< ZMQ_TOPIC_REQ_REMOTE_CONTROL<<tDest<<"Publish->车端";
}

void IpcManager::slotTestZmq(const void* fbuf,int id,const uint32_t &len)
{

////    QScopedPointer<const void*>p(&fbuf);
//    switch (id) {
//    case Topic_ID::TOPIC_TCP_GNSS_GPS_INFO_ENUM :
//    {
//        auto con =  flatbuffers::GetRoot<GpsImuInfo>(fbuf);
//    }
//        break;
//    case Topic_ID::TOPIC_TCP_GNSS_GPS_STATUS_ENUM :
//    {
////        const GpsStatus *f = static_cast<const GpsStatus*>(fbuf);
////        qDebug()<<"新加入测试数据"<<"GpsStatus222222222";
//    }
//        break;
//    case Topic_ID::TOPIC_IPC_MESSAGE_TEST_TOPIC_ENUM :
//    {
//            auto con =  flatbuffers::GetRoot<Msg8105>(fbuf);
//            qDebug()<<"测试数据:"<<con->result()<<len;
//    }
//        break;
//    case Topic_ID::TOPIC_HUAWEI_GNSS_BROADCAST_DATA_ENUM :
//    {
//        auto con =  flatbuffers::GetRoot<MsgVector>(fbuf);
//        for (auto it = con->vectormsg()->cbegin(); it != con->vectormsg()->cend(); it ++) {

//            qDebug()<<"测试数据1111:"<<*it;
//        }

//        QThread::sleep(2);

//    }
//        break;
//    case Topic_ID::TOPIC_GNSS_BROADCAST_DATA_ENUM:
//    {
//        qDebug()<<"新加入测试数据"<<"############";
//    }
//    case Topic_ID::UNKNOW:
//    {
//        const char *error= static_cast<const char*>(fbuf);
//        qDebug()<<error;
//    }

//    default:
//        break;
//    }
//    if(fbuf){
//        delete fbuf;
//    }
}
 void IpcManager::slotTimerTest()
 {
     _currentId  = 0x01000016;

     if(_currentId == 0x01000016){
         _currentId = 0x01000017;
     }
     else {
         _currentId = 0x01000016;
     }
     QVector<ModuleDir>   m_moduleDirs;
     QVector<int>         m_cmds;
     QVector<QVariant>    m_datas;

     ModuleDir module;
     module.append(/*MD_MainProcessor*/MD_InteractiveManager);
     QVariant variant(QVariant::UInt);
     variant.setValue(static_cast<Qt::Key>(_currentId));

     m_moduleDirs.append(module);
     m_cmds.append(/*MP_SoftKeyPress*/IA_KeyPress);
     m_datas.append(variant);

     for (int i = 0;i < m_cmds.size(); i++) {
         QMetaObject::invokeMethod(s_pInstance,"sendToMain",
                                   Q_ARG(ModuleDir,m_moduleDirs.at(i)),
                                   Q_ARG(int,m_cmds.at(i)),
                                   Q_ARG(QVariant,m_datas.at(i)));
         qDebug()<<m_cmds.at(i)<<m_datas.at(i)<<m_moduleDirs.at(i)
                <<"按键×××××××××××××××××××××××××××××";
     }
 }
