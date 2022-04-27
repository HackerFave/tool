#include "MainProcessor.h"
#include <QApplication>
#include <QWindow>
#include <QFile>
#include <QDesktopWidget>
#include <QDebug>
#include <QMouseEvent>
#include <QTimer>
#include "Mainwindow.h"
#include "Interface/IFunWidget.h"
#include "FunModule/IpcManager.h"
#include "FunModule/InteractiveManager.h"
#include "Interface/WgtFactory.h"
#include <public/saveGifForm.h>
MainProcessor::MainProcessor(QObject *parent)
    : QObject(parent),
      m_mainWinddow(nullptr)
{
    // 初始化样式
    this->initStyle();
    // 初始化系统
    this->initSystem();
    // 初始化功能模块
    this->initFunModule();
    // 初始主窗口
    this->initMainWindow();
}

MainProcessor::~MainProcessor()
{
    emit killProc();//杀死进程
    delete m_mainWinddow;
}
//初始化样式表
void MainProcessor::initStyle()
{
    // 载入QSS样式
    QFile file(QString(":style/qss/%1.css").arg("blue"));
    file.open(QFile::ReadOnly);
    QString qss(QLatin1String(file.readAll()));
    qApp->setStyleSheet(qss);
    qApp->setPalette(QPalette(QColor("#F0F0F0")));
    file.close();
}
//初始化系统
void MainProcessor::initSystem()
{
    System::getInstance();
    WgtFactory::initWidget();//屏幕工厂类初始化
    int procType = System::getProcessType();

    // 主进程开启其他两个进程，并绑定实现任意进程退出全退出。
    if(procType == PT_MidProc){
        QStringList arguments;
        arguments.append("-R");
        arguments.append("-L");
        for(auto argument : arguments){
            QProcess *pProc = new QProcess();
            connect(this,SIGNAL(killProc()),pProc,SLOT(kill()));
            connect(pProc,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(procFinished(int,QProcess::ExitStatus)));
            pProc->start("RCD",QStringList(argument));
            pProc->waitForStarted();
        }
    }

    //    QProcess process;
    //    QString path = "gifsave";
    //    process.start(path);

}
//初始化功能模块
void MainProcessor::initFunModule()
{
    // 进程通讯功能初始化
    m_pIpcManager = IpcManager::GetInstance(this);//zmq作为模块间数据交互

    connect(m_pIpcManager, SIGNAL(sendToMain(ModuleDir,int,QVariant)),
            this, SLOT(getFromAny(ModuleDir,int,QVariant)));//getFromAny() 所有消息入口

    // 交互管理初始化 （主要处理按键操作）
    InteractiveManager::GetInstance(this);

    connect(pInteractiveIns, SIGNAL(sendToMain(ModuleDir,int,QVariant)),
            this, SLOT(getFromAny(ModuleDir,int,QVariant)),Qt::QueuedConnection);

    //录屏模块
    connect(saveGifForm::instance(), SIGNAL(signalGifForm(QMap<QString,bool>)),
            this, SLOT(slotGifForm(QMap<QString,bool>)));
}
//初始化主界面
void MainProcessor::initMainWindow()
{

    // 左中右屏幕各自显示页面
    m_mainWinddow = new MainWindow();
    int procType = System::getProcessType();
    if(procType & PT_MidProc){//中间进程界面
        this->switchWidget(SA_MainScreen,MD_MainMonitor);
    }
    if(procType & PT_RightProc){//右侧进程界面
        this->switchWidget(SA_RightScreenL,MD_AllInfo);
        this->switchWidget(SA_RightScreenR,MD_WarnInfo);
    }
    if(procType & PT_LeftProc){//左侧进程界面
        this->switchWidget(SA_LeftScreenTopR,MD_LeftCamera);
        this->switchWidget(SA_LeftScreenTopL,MD_DriveCamera);
        this->switchWidget(SA_LeftScreenBottom,MD_CloudWeb);
    }
}
//展示所有界面
void MainProcessor::run()
{
    m_mainWinddow->showAll();

}
//事件过滤重写
bool MainProcessor::eventFilter(QObject *target, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress
            || event->type() == QEvent::MouseButtonDblClick
            || event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        if(e->button() == Qt::RightButton)
        {//不支持鼠标右击
            return true;
        }
    }
    else if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        this->keyPressHandle(target,keyEvent->key());

    }//过滤掉鼠标移动悬浮与离开事件
    else if(event->type() == QEvent::HoverMove
            || event->type() == QEvent::HoverEnter
            || event->type() == QEvent::HoverLeave)
    {
        return true;
    }

    return QObject::eventFilter(target, event);
}
//键盘建输入（过滤掉鼠标键）
void MainProcessor::keyPressHandle(QObject *target,int key)
{
    if(target->isWindowType()){//是否继承了window
        qDebug() << static_cast<Qt::Key>(key) << target->objectName();
        if(target->objectName() == "MessageBoxWindow"){
            return;
        }
        ModuleDir moduleDir;
        moduleDir.append(MD_InteractiveManager);//交互管理模块
        this->getFromAny(moduleDir,IA_KeyPress, QVariant(key)); //接收处理所有模块命令
    }
}
//进程退出
void MainProcessor::procFinished(int,QProcess::ExitStatus)
{
    QApplication::exit();
}
//主进程模块
void MainProcessor::mainProcess(ModuleDir &,int &cmd, QVariant &data)
{
    MainProcessorCmdType Cmd = static_cast<MainProcessorCmdType>(cmd);
    switch (Cmd)
    {
    case MP_SwitchWgt:{
        ScreenAreaType screenArea = static_cast<ScreenAreaType>(data.toList().first().toInt());
        ModuleType module = static_cast<ModuleType>(data.toList().last().toInt());
        this->switchWidget(screenArea,module);
        break;
    }
    case MP_SwitchLayout:{
        this->switchLayout(data.toInt());
        break;
    }
    case MP_SoftKeyPress:{

        QKeyEvent keyPress(QEvent::KeyPress,data.toUInt(),Qt::NoModifier);
        //QKeyEvent keyRelease(QEvent::KeyRelease,data.toUInt(),Qt::NoModifier);
        qDebug()<<static_cast<Qt::Key>(data.toUInt())<<"外部按键…………………………………………………………";
        if(QApplication::focusWindow() != nullptr){
            QApplication::sendEvent(QApplication::focusWindow(), &keyPress);
        }
        QApplication::sendEvent(QApplication::focusWindow(), &keyPress);
        break;
    }case MP_IpcForward:{
        ModuleDir md;
        md.append(data.toList().at(0).toInt());
        int Cmd = data.toList().at(1).toInt();
        this->doFromAny(md,Cmd,data.toList().last());
        break;
    }
    default:
        break;
    }
}

//
// @brief:类似中间件函数，所有通讯消息的入口
// @params: moduleDir-模块地址  cmd-命令 data-数据
// @ret:void
// @birth:created by hyx on 2020
//
void MainProcessor::getFromAny(ModuleDir moduleDir,int cmd, QVariant data)
{

    if(moduleDir.isEmpty()){
        return;
    }
    ModuleType Dir;
    Dir = static_cast<ModuleType>(moduleDir.first());
    MessageType msgType = whichMsg(Dir,cmd,data);// 枚举转换
    switch (msgType) {
    case MT_ThisProc:{
        this->doFromAny(moduleDir,cmd,data);
        break;

    }
    case MT_Broadcast:{
        this->doFromAny(moduleDir,cmd,data);
        int procType = System::getProcessType();
        if(procType == PT_MidProc){
            m_pIpcManager->ipcForward(PT_LeftProc,moduleDir,cmd,data);
            m_pIpcManager->ipcForward(PT_RightProc,moduleDir,cmd,data);
        }else if(procType == PT_RightProc){
            m_pIpcManager->ipcForward(PT_LeftProc,moduleDir,cmd,data);
            m_pIpcManager->ipcForward(PT_MidProc,moduleDir,cmd,data);
        }else if(procType == PT_LeftProc){
            m_pIpcManager->ipcForward(PT_RightProc,moduleDir,cmd,data);
            m_pIpcManager->ipcForward(PT_MidProc,moduleDir,cmd,data);
        }
        break;
    }
    case MT_LeftProc:{
        m_pIpcManager->ipcForward(PT_LeftProc,moduleDir,cmd,data);
        break;
    }
    case MT_MidProc:{
        m_pIpcManager->ipcForward(PT_MidProc,moduleDir,cmd,data);
        break;
    }
    case MT_RightProc:{
        m_pIpcManager->ipcForward(PT_RightProc,moduleDir,cmd,data);
        break;
    }
    default:
        break;
    }

}

//
// @brief:本进程需要处理的所有消息入口
// @params: moduleDir-模块地址  cmd-命令 data-数据
// @ret:void
// @birth:created by hyx on 2020
//
void MainProcessor::doFromAny(ModuleDir moduleDir,int &cmd, QVariant &data)
{
    if(moduleDir.isEmpty()){
        return;
    }
    ModuleType Dir;
    Dir = static_cast<ModuleType>(moduleDir.first());
    moduleDir.removeFirst();
    switch (Dir){
    case MD_MainProcessor:{//主处理模块
        this->mainProcess(moduleDir,cmd,data);
        break;
    }
    case MD_IPCManager:{//进程通信管理模块
        m_pIpcManager->getFromAny(moduleDir,cmd,data);
        break;
    }
    case MD_InteractiveManager:{//输入交互管理模块
        pInteractiveIns->getFromAny(moduleDir,cmd,data);
        qDebug()<<"交互管理模块------333333333333333333333333333333";
        break;
    }
    default:{
        WgtFactory::getFunWidget(Dir)
                ->getCL()->getFromAny(moduleDir,cmd,data);
        break;
    }
    }
}

MainProcessor::MessageType MainProcessor::whichMsg(ModuleType &mdType,int &cmd,QVariant &data)
{
    MessageType msgType = MT_ThisProc;//本进程消息
    int procType = System::getProcessType();

    if(mdType == MD_MainProcessor){
        MainProcessorCmdType Cmd = static_cast<MainProcessorCmdType>(cmd);
        switch (Cmd) {
        case MP_SwitchWgt:{
            ModuleType module = static_cast<ModuleType>(data.toList().last().toInt());
            int belongProc = System::belongTo(module);
            if(procType & belongProc){
                msgType = MT_ThisProc;
            }else {
                if(belongProc == PT_LeftProc){
                    msgType = MT_LeftProc;
                }else if(belongProc == PT_RightProc){
                    msgType = MT_RightProc;
                }else{
                    msgType = MT_MidProc;
                }
            }
            break;
        }
        case MP_UpdateSysData:
        case MP_SwitchLayout:{
            msgType = MT_Broadcast;
            break;
        }
        case MP_SoftKeyPress:
        case MP_IpcForward:{
            msgType = MT_ThisProc;
            break;
        }
        default:
            break;
        }
    }else if(mdType == MD_IPCManager || mdType == MD_InteractiveManager){
        if(procType & PT_MidProc){
            msgType = MT_ThisProc;
        }else{
            msgType = MT_MidProc;
        }
    }else{
        int belongProc = System::belongTo(mdType);
        if(procType & belongProc){
            msgType = MT_ThisProc;
        }else {
            if(belongProc == PT_LeftProc){
                msgType = MT_LeftProc;
            }else if(belongProc == PT_RightProc){
                msgType = MT_RightProc;
            }else{
                msgType = MT_MidProc;
            }
        }
    }
    return msgType;
}

//
// @brief:布局切换
// @params: layoutType 布局类型
// @ret:void
// @birth:created by hyx on 2020
//
void MainProcessor::switchLayout(int layoutType)
{
    LayoutType layout = static_cast<LayoutType>(layoutType);
    int procType = System::getProcessType();
    switch (layout){
    case LT_Monitor:{
        if(procType & PT_MidProc){
            this->switchWidget(SA_MainScreen, MD_MainMonitor);
        }
        if(procType & PT_RightProc){
            this->switchWidget(SA_RightScreenL, MD_AllInfo);
            this->switchWidget(SA_RightScreenR, MD_WarnInfo);
        }
        break;
    }
    case LT_RemoteControl:{
        if(procType & PT_MidProc){
            this->switchWidget(SA_MainScreen, MD_MainCamera);
        }
        if(procType & PT_RightProc){
            this->switchWidget(SA_RightScreenL, MD_RightCamera);
            this->switchWidget(SA_RightScreenR, MD_VehicleInfo);
        }
        break;
    }
    default:
        break;
    }

    m_mainWinddow->switchLayout(layout);

    if(procType & PT_MidProc){
        pInteractiveIns->getFromAny(ModuleDir(),IA_SwitchLayout,QVariant(layoutType));
    }
}

//
// @brief:widget窗口切换
// @params: Area：切换区域  wgtId：切换目标
// @ret:void
// @birth:created by hyx on 2020
//
void MainProcessor::switchWidget(ScreenAreaType Area, int wgtId)
{
    // 从widget工厂获取目标widget
    IFunWidget *pSwitchWgt = WgtFactory::getFunWidget(static_cast<ModuleType>(wgtId));
    IFunWidget *pCurrenWgt = nullptr;

    auto iter = m_currentWgt.find(Area);
    if(iter != m_currentWgt.end()){
        pCurrenWgt = m_currentWgt[Area];
    }

    if(pSwitchWgt == nullptr || pCurrenWgt == pSwitchWgt) //相同页面返回
    {
        return;
    }

    if(pCurrenWgt != nullptr && !pCurrenWgt->isHidden()) //如果当前有窗口，断开信号并退出
    {
        if(!pCurrenWgt->isQuit())//是否可以退出，不可则返回
        {
            return;
        }

        if(!pSwitchWgt->isEnter())//是否可以进入，不可则返回
        {
            return;
        }
        disconnect(pCurrenWgt->getCL(), SIGNAL(sendToMain(ModuleDir,int,QVariant)),
                   this, SLOT(getFromAny(ModuleDir,int,QVariant)));
        pCurrenWgt->quit();
    }

    if (pSwitchWgt->getCL() == nullptr) {
        qCritical() << "pSwitchWgt->getCL() == nullptr ";
    }
    connect(pSwitchWgt->getCL(), SIGNAL(sendToMain(ModuleDir,int,QVariant)),
            this, SLOT(getFromAny(ModuleDir,int,QVariant)));


    m_currentWgt[Area] = pSwitchWgt;

    pCurrenWgt = pSwitchWgt;

    pCurrenWgt->initWidget();

    m_mainWinddow->addShowWidget(Area,pCurrenWgt);

    pCurrenWgt->showedWidgetDo();
}

void MainProcessor::slotGifForm(QMap<QString,bool> map)//启动录屏
{
    m_mainWinddow->showAllGif(map);
}
