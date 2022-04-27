#include "systemInfoPlugin.h"
#include "ui_systemInfoPlugin.h"
#include <QDebug>
#include <QTableWidget>
systemInfoPlugin::systemInfoPlugin(QWidget *parent,const void *node) :
    BaseWidget(parent),
    _node(node),
    ui(new Ui::systemInfoPlugin)
{
    ui->setupUi(this);
    qDebug()<<"[ 插件：]"<<"systemInfoPlugin";
    initComboBox();
    connect(ui->lineEdit_shell, SIGNAL(returnPressed()), this, SLOT(slotLineEditPubData()));//连接信号与槽
    _workThread = new QThread;
    P_workThread = new workThread(_node);
    P_workThread->moveToThread(_workThread);
    connect(_workThread,SIGNAL(finished()),P_workThread,SLOT(deleteLater()));
    connect(_workThread,SIGNAL(started()),P_workThread,SLOT(slotDoWork()));

    connect(P_workThread,SIGNAL(signalShellCMDResult(const void*)),this,SLOT(slotShellCMDResult(const void*)));
    connect(P_workThread,SIGNAL(signalDataDiskMonitor(const void*)),this,SLOT(slotDataDiskMonitor(const void*)));
    connect(P_workThread,SIGNAL(signalDataCPUMEMMonitor(const void*)),this,SLOT(slotDataCPUMEMMonitor(const void*)));
    void * currentnode = const_cast<void*>(_node);
    //kill 进程
    _MsgKILLProcess_Write = static_cast<Node*>(currentnode)->CreateWriter<MsgKILLProcessPubSubType>(Topic_KILLProcess, QOS_PROFILE_EVENT_LOCAL);
    //一键清除
    _MsgClearAll_Write = static_cast<Node*>(currentnode)->CreateWriter<MsgClearAllPubSubType>(Topic_ClearAll, QOS_PROFILE_EVENT_LOCAL);
    // shell 指令
    _MsgShellCMD_Write = static_cast<Node*>(currentnode)->CreateWriter<MsgShellCMDPubSubType>(Topic_ShellCMD, QOS_PROFILE_EVENT_LOCAL);

    setAttribute(Qt::WA_DeleteOnClose);
    ui->tabWidget_cpu->clear();
    ui->tabWidget_Disk->clear();
}

systemInfoPlugin::~systemInfoPlugin()
{
    qDebug()<<"【子界面正在析构：systemInfoPlugin】";
    if(_workThread->isRunning())
    {
        P_workThread->stopWork();  //关闭线程槽函数
        _workThread->quit();            //退出事件循环
        _workThread->wait();            //释放线程槽函数资源
    }

    delete ui;
}
void systemInfoPlugin::slotRecvBackendData(const std::string& data)
{
    qDebug()<<QString("[%1获取主框架数据:]").arg(this->objectName());
}
void systemInfoPlugin::slotPluginWidgetsPointHash(QHash<QString, QWidget *> hash)
{
    QHash<QString,QWidget*>::iterator it;
    for (it = hash.begin();it!=hash.end();it++) {

    }
}
void systemInfoPlugin::on_pushButton_clear_clicked()
{
    ui->textBrowser_output->clear();
}
void systemInfoPlugin::slotSetThreadStatus(bool status)
{
    if(status){
        _workThread->start();
    }
}
void systemInfoPlugin::slotShellCMDResult(const void* data)
{
    void * str = const_cast<void*>(data);
    MsgShellCMDResult* tmp = static_cast<MsgShellCMDResult*>(str);
    ui->textBrowser_output->append(QString("[指令回复$:]%1").arg(tmp->result().c_str()));
}
//磁盘监控
void systemInfoPlugin::slotDataDiskMonitor(const void* data)
{
    void * str = const_cast<void*>(data);
    MsgDiskMonitor *tmp = static_cast<MsgDiskMonitor*>(str);
    //    ui->label_moudle_disk->setText(QString("模块：%1").arg(getMoudle(tmp->moudle())));
    if(_hashTableViewDisk.keys().contains(getMoudle(tmp->moudle()))){
        _hashTableViewDisk.value(getMoudle(tmp->moudle()))->setDiskDataInfo(tmp->diskList());
    }
    else {
        baseTableViewDisk * disk = new baseTableViewDisk;
        disk->setObjectName(getMoudle(tmp->moudle()));
        ui->tabWidget_Disk->addTab(disk,getMoudle(tmp->moudle()));
        _hashTableViewDisk.insert(getMoudle(tmp->moudle()),disk);
        disk->setDiskDataInfo(tmp->diskList());
    }

}
//CPU监控
void systemInfoPlugin::slotDataCPUMEMMonitor(const void* data)
{
    void * str = const_cast<void*>(data);
    MsgCPUMEMMonitor *tmp = static_cast<MsgCPUMEMMonitor*>(str);
    //    ui->label_moudle_cpu->setText(QString("模块：%1 ").arg(getMoudle(tmp->moudle())));
    ui->label_CPUUsage->setText(QString("CPU总使用率：%1 %").arg( tmp->CPUUsage())); // CPU总使用率 0.0-100.0
    ui->label_CPUTemp->setText(QString("CPU温度：%1").arg( tmp->CPUTemp()));  // CPU温度 如 56.98
    ui->label_MEMUsage->setText(QString("内存总使用率：%1 %").arg(tmp->MEMUsage())); //内存总使用率 0.0-100.0
    if(_hashTableViewCPU.keys().contains(getMoudle(tmp->moudle()))){
        _hashTableViewCPU.value(getMoudle(tmp->moudle()))->setCPUDataInfo(tmp->processList());
    }
    else {
        baseTableViewCPU * cpu = new baseTableViewCPU;
        connect(cpu,SIGNAL(signalKillProc(MsgKILLProcess)),this,SLOT(slotKillProc(MsgKILLProcess)),Qt::DirectConnection);
        cpu->setObjectName(getMoudle(tmp->moudle()));
        ui->tabWidget_cpu->addTab(cpu,getMoudle(tmp->moudle()));
        _hashTableViewCPU.insert(getMoudle(tmp->moudle()),cpu);
        cpu->setCPUDataInfo(tmp->processList());
    }

}
void systemInfoPlugin::initComboBox()
{
    ui->comboBox_moudle->clear();
    ui->comboBox_moudle->addItem("Unknown");
    ui->comboBox_moudle->addItem("CCU");
    ui->comboBox_moudle->addItem("CGU");
    ui->comboBox_moudle->addItem("CGU_TBOX");
    ui->comboBox_moudle->addItem("HMI");
    ui->comboBox_moudle->addItem("ICU");
}
void systemInfoPlugin::slotLineEditPubData()
{
    if(ui->lineEdit_shell->text().simplified().isEmpty()){
        return;
    }
    ui->textBrowser_output->append(QString("[下发指令$:]%1").arg(ui->lineEdit_shell->text().simplified()));
    MsgShellCMD tmp;
    tmp.cmd(ui->lineEdit_shell->text().simplified().toStdString());//shell命令
    tmp.moudle(static_cast<moudleType>(ui->comboBox_moudle->currentIndex()));//模块
    tmp.overTime(2);//超时时间
    _MsgShellCMD_Write->Write(tmp);
}
//一键清除
void systemInfoPlugin::on_pushButton_backend_data_clicked()
{
    MsgClearAll tmp;
    tmp.moudle(static_cast<moudleType>(ui->comboBox_moudle->currentIndex()));//模块
    _MsgClearAll_Write->Write(tmp);
}
//kill 进程
void systemInfoPlugin::slotKillProc(MsgKILLProcess tmp)
{
    _MsgKILLProcess_Write->Write(tmp);
}

