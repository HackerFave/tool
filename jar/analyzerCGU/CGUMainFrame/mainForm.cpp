#include "mainForm.h"
#include "ui_mainForm.h"
#include "manager/PluginManager.h"
#include <QtDebug>
#include <QWindow>
static QString _outText = "";
mainForm *mainForm::P_mainForm = nullptr;
mainForm::mainForm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mainForm)
{
    ui->setupUi(this);
    P_mainForm = this;
    initOtherForm();
    qInstallMessageHandler(logOutput);
    _node = new Node("monitor");//唯一node
#if 1
    _workThread = new QThread;
    P_workThread = new workThread(_node);

    P_workThread->moveToThread(_workThread);
    connect(_workThread,SIGNAL(finished()),P_workThread,SLOT(deleteLater()));
    connect(_workThread,SIGNAL(started()),P_workThread,SLOT(slotDoWork()));
    _workThread->start();

    //数据获取线程
    P_dataInfoThread = new dataInfoThread;
    _thread = new QThread;
    P_dataInfoThread->moveToThread(_thread);
    connect(_thread,SIGNAL(finished()),P_dataInfoThread,SLOT(deleteLater()));

    //初始化线程

    P_initThread = new initThread;
    _threadInit = new QThread;
    P_initThread->moveToThread(_threadInit);
    connect(_threadInit,SIGNAL(finished()),P_initThread,SLOT(deleteLater()));
    connect(_threadInit,SIGNAL(started()),P_initThread,SLOT(slotDoWork()));
    _threadInit->start();

    connect( P_initThread,SIGNAL(signalNoticeThread(void*)),this,SLOT(slotStartThread(void*)),Qt::QueuedConnection);
    connect( P_initThread,SIGNAL(signalNoticeThread(void*)),P_dataInfoThread,SLOT(slotDoWork(void*)));
#endif
    initUi();
#if 1
    connect(P_dataInfoThread,SIGNAL(signalBackendData(const std::string&)),
            this,SLOT(slotBackendData(const std::string&)));//获取后端数据发送到各个插件
#endif
    connect(this,SIGNAL(signalNewPlugin(const QString &)),this,SLOT(slotNewPlugin(const QString &)),Qt::DirectConnection);
    this->setWindowTitle("监控工具");
}

mainForm::~mainForm()
{
    PluginManager::instance()->unloadPlugin(QApplication::applicationDirPath());
    if(_thread){
        if(_thread->isRunning())
        {
            P_dataInfoThread->stopWork();  //关闭线程槽函数
            _thread->quit();            //退出事件循环
            _thread->wait();            //释放线程槽函数资源
        }
        if(_threadInit->isRunning())
        {
            P_initThread->stopWork();  //关闭线程槽函数
            _threadInit->quit();            //退出事件循环
            _threadInit->wait();            //释放线程槽函数资源
        }
    }
    if(_outputForm){
        delete _outputForm;
    }
    delete ui;
}
void mainForm::initUi()
{
    ui->tabWidget->setTabsClosable(true);
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(slotCloseTab(int)),Qt::QueuedConnection);
    ui->tabWidget->clear();
    _widgetHash.clear();//插件界面映射
    _pluginNamePath.clear();//插件名与路径映射
    PluginManager::instance()->ReadPluginsInfo(_pluginNamePath,QApplication::applicationDirPath(),_widgetHash,_node);
    QHash<QString,QWidget*>::iterator it;
    for(it=_widgetHash.begin();it!=_widgetHash.end();++it){
        QAction* action = new QAction(it.key(),this);
        ui->menuPlugin->addAction(action);
        connect(action,SIGNAL(triggered(bool)),this,SLOT(slotAction(bool)));
        connect(this,SIGNAL(signalPluginWidgetsPointHash(QHash<QString,QWidget*>))
                ,it.value(),SLOT(slotPluginWidgetsPointHash(QHash<QString,QWidget*>)));//插件界面列表
    }
    emit signalPluginWidgetsPointHash(_widgetHash);
}
void mainForm::slotAction(bool)
{
    QString text = qobject_cast<QAction *>(this->sender())->text();
    ui->tabWidget->addTab(_widgetHash.value(text),text);
    connect(this,SIGNAL(signalUploadBackendData(const std::string&)),
            _widgetHash.value(text),SLOT(slotRecvBackendData(const std::string&)));//推送后台数据到各个插件

    connect(_widgetHash.value(text),SIGNAL(signalPluginToBackendData(const std::string&)),
            this,SLOT(slotPluginToBackendData(const std::string&)));//插件发送数据到主框架

    connect(this,SIGNAL(signalSetThreadStatus(bool)),
            _widgetHash.value(text),SLOT(slotSetThreadStatus(bool)));//启动插件线程

    emit signalSetThreadStatus(true);//开始线程
}
void mainForm::slotActionOutput(bool)
{
    QString text = qobject_cast<QAction *>(this->sender())->text();
    ui->tabWidget->addTab(_outputForm,"output");
}
void mainForm::initOtherForm()
{
    _actionOutputLog = new QAction("output");
    _outputForm = new outputForm;
    ui->menuOther->addAction(_actionOutputLog);
    connect(_actionOutputLog,SIGNAL(triggered(bool)),this,SLOT(slotActionOutput(bool)));
}
//启动数据获取线程
void mainForm::slotStartThread(void* monitor)
{
    _thread->start();
}
//获取后台数据分发到各个插件
void mainForm::slotBackendData(const std::string& data)
{
    //    QJsonObject json= publicClass::getInstance()->getJsonObjectFromString(data.c_str());
    //    qDebug()<<"[monitor backend data:]"/*data.c_str()<<*/;
    emit signalUploadBackendData(data);
}

void mainForm::slotCloseTab(const int& index)
{
    if (index == -1) {
        return;
    }

    QString text = ui->tabWidget->tabText(index);
    if(text.simplified()=="output"){
        _outputForm->close();
        ui->tabWidget->removeTab(index);
        return;
    }
    this->setWindowTitle("正在回收插件资源...");
    disconnect(this,SIGNAL(signalUploadBackendData(const std::string&)),
               _widgetHash.value(text),SLOT(slotRecvBackendData(const std::string&)));//断开到指定插件数据的推送

    disconnect(_widgetHash.value(text),SIGNAL(signalPluginToBackendData(const std::string&)),
               this,SLOT(slotPluginToBackendData(const std::string&)));//断开插件发送数据到主框架
    QWidget* tabItem = ui->tabWidget->widget(index);
    // Removes the tab at position index from this stack of widgets.
    // The page widget itself is not deleted.
    ui->tabWidget->removeTab(index);
    delete(tabItem);
    tabItem = nullptr;
    emit signalNewPlugin(text);//重新构造插件
    this->setWindowTitle("监控工具");

}
void mainForm::slotPluginToBackendData(const std::string& data)//发送数据到主框架
{

    QString text = qobject_cast<QWidget*>(this->sender())->objectName();
    qDebug()<<"[插件:]"<<text<<"[上传数据-->]"<<data.c_str()<<"[到主框架]";
}
void mainForm::slotNewPlugin(const QString &text)
{
    PluginManager::instance()->loadPlugin(_pluginNamePath.value(text),_widgetHash,_node);
    connect(this,SIGNAL(signalPluginWidgetsPointHash(QHash<QString,QWidget*>))
            ,_widgetHash.value(text),SLOT(slotPluginWidgetsPointHash(QHash<QString,QWidget*>)));//插件界面列表
    emit signalPluginWidgetsPointHash(_widgetHash);
}
void mainForm::writeOutPut(const QString &text)
{
    //    if(_outputForm->isVisible()){
    _outputForm->showOutPut(text);
    //    }
}
void mainForm::logOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString text;
    text.append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + " ");
    switch(type)
    {
    case QtDebugMsg:
        text.append("Debug:");
        break;

    case QtWarningMsg:
        text.append("Warning:");
        break;

    case QtCriticalMsg:
        text.append("Critical:");
        break;

    case QtFatalMsg:
        text.append("Fatal:");
    }

    text.append(msg);
    P_mainForm->writeOutPut(text);
}

