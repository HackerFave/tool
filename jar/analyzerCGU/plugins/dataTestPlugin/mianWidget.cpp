#include "mianWidget.h"
#include "ui_mianWidget.h"
#include "idl/systemInfo.h"
#include "idl/systemInfoPubSubTypes.h"
#include "dds_wrapper/include/node.hpp"
#include "idl/systemInfoTypeObject.h"
#include <QDebug>
using namespace tg_msgs::dds;

mianWidget::mianWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mianWidget)
{
    ui->setupUi(this);
}

mianWidget::~mianWidget()
{
    delete ui;
}


void mianWidget::on_pushButton_test_clicked()
{
    Node node("systeminfo");
    // puber
    MsgShellCMDResult tmp;
    srand((int)time(0));
    std::string name = "module_name" + std::to_string(rand() % 100);
    std::cout << name << std::endl;
    // puber
    auto writer = node.CreateWriter<MsgShellCMDResultPubSubType>(Topic_ShellCMDResult, QOS_PROFILE_EVENT_LOCAL);

    int i = 0;
    tmp.cmd("我在测试---fuck all！");
    tmp.moudle(moudleType::HMI);
    tmp.result("我猜你看到我了……");
    writer->Write(tmp);

}

void mianWidget::on_pushButton_test_5_clicked()
{
    Node node("systeminfo");
    // puber
    //    MsgShellCMDResult tmp;
//    MsgDiskMonitor tmp1;
    MsgCPUMEMMonitor tmp2;
    srand((int)time(0));
    std::string name = "module_name" + std::to_string(rand() % 100);
    std::cout << name << std::endl;
    // puber
//    auto writer1 = node.CreateWriter<MsgDiskMonitorPubSubType>(Topic_DiskMonitor, QOS_PROFILE_EVENT_LOCAL);
    auto writer2 = node.CreateWriter<MsgCPUMEMMonitorPubSubType>(Topic_CPUMonitor, QOS_PROFILE_EVENT_LOCAL);
    //    while(1){
    int i = 0;


//    for(int i=0;i<40;i++){
//        //           tmp1.diskList()
//        MsgDiskStatST st;
//        st.fileSystem("/test333/"); //分区名 如/dev/mmcblk3p1
//        st.mountedOn("/home4444");  //挂载路径 如/home/work
//        st.size("123");       //总大小 如29G 847M
//        st.used("124");       //使用大小 如9.8G 4.0K
//        st.usage("55");      //使用率 如37%
//        tmp1.diskList().push_back(st);
//    }
//    tmp1.moudle(moudleType::HMI);
//    writer1->Write(tmp1);

    for(int i=0;i<40;i++){
        MsgProcessST st;
        st.process("/home/work/cloudClient");    //进程名 如/home/work/cloudClient
        st.pid(10086); //进程pid
        st.pCPUUsage(38);   // CPU使用率 0.0-100.0
        st.pMEMUsage(89);   //内存使用率 0.0-100.0
        tmp2.processList().push_back(st);
    }
    tmp2.CPUUsage(33);
    tmp2.CPUTemp(56.99);
    tmp2.MEMUsage(56);
    tmp2.moudle(moudleType::HMI);
    writer2->Write(tmp2);
}

void mianWidget::on_pushButton_test_4_clicked()
{
    Node node("systeminfo");
    // puber
    //    MsgShellCMDResult tmp;
//    MsgDiskMonitor tmp1;
    MsgCPUMEMMonitor tmp2;
    srand((int)time(0));
    std::string name = "module_name" + std::to_string(rand() % 100);
    std::cout << name << std::endl;
    // puber
//    auto writer1 = node.CreateWriter<MsgDiskMonitorPubSubType>(Topic_DiskMonitor, QOS_PROFILE_EVENT_LOCAL);
    auto writer2 = node.CreateWriter<MsgCPUMEMMonitorPubSubType>(Topic_CPUMonitor, QOS_PROFILE_EVENT_LOCAL);
    //    while(1){
    int i = 0;


//    for(int i=0;i<40;i++){
//        //           tmp1.diskList()
//        MsgDiskStatST st;
//        st.fileSystem("/test/"); //分区名 如/dev/mmcblk3p1
//        st.mountedOn("/home");  //挂载路径 如/home/work
//        st.size("111");       //总大小 如29G 847M
//        st.used("222");       //使用大小 如9.8G 4.0K
//        st.usage("333");      //使用率 如37%
//        tmp1.diskList().push_back(st);
//    }
//    tmp1.moudle(moudleType::CGU);
//    writer1->Write(tmp1);

    for(int i=0;i<40;i++){
        MsgProcessST st;
        st.process("****");    //进程名 如/home/work/cloudClient
        st.pid(10086); //进程pid
        st.pCPUUsage(110);   // CPU使用率 0.0-100.0
        st.pMEMUsage(119);   //内存使用率 0.0-100.0
        tmp2.processList().push_back(st);
    }
    tmp2.CPUUsage(33);
    tmp2.CPUTemp(56.99);
    tmp2.MEMUsage(56);
    tmp2.moudle(moudleType::CGU);
    writer2->Write(tmp2);
}

void mianWidget::on_pushButton_test_2_clicked()
{
    Node node("systeminfo");
    // puber
    //    MsgShellCMDResult tmp;
//    MsgDiskMonitor tmp1;
    MsgCPUMEMMonitor tmp2;
    srand((int)time(0));
    std::string name = "module_name" + std::to_string(rand() % 100);
    std::cout << name << std::endl;
    // puber
//    auto writer1 = node.CreateWriter<MsgDiskMonitorPubSubType>(Topic_DiskMonitor, QOS_PROFILE_EVENT_LOCAL);
    auto writer2 = node.CreateWriter<MsgCPUMEMMonitorPubSubType>(Topic_CPUMonitor, QOS_PROFILE_EVENT_LOCAL);
    //    while(1){
    int i = 0;


//    for(int i=0;i<40;i++){
//        //           tmp1.diskList()
//        MsgDiskStatST st;
//        st.fileSystem("/test/"); //分区名 如/dev/mmcblk3p1
//        st.mountedOn("/home");  //挂载路径 如/home/work
//        st.size("111");       //总大小 如29G 847M
//        st.used("222");       //使用大小 如9.8G 4.0K
//        st.usage("333");      //使用率 如37%
//        tmp1.diskList().push_back(st);
//    }
//    tmp1.moudle(moudleType::CGU_TBOX);
//    writer1->Write(tmp1);

    for(int i=0;i<40;i++){
        MsgProcessST st;
        st.process("****");    //进程名 如/home/work/cloudClient
        st.pid(10086); //进程pid
        st.pCPUUsage(110);   // CPU使用率 0.0-100.0
        st.pMEMUsage(119);   //内存使用率 0.0-100.0
        tmp2.processList().push_back(st);
    }
    tmp2.CPUUsage(33);
    tmp2.CPUTemp(56.99);
    tmp2.MEMUsage(56);
    tmp2.moudle(moudleType::CGU_TBOX);
    writer2->Write(tmp2);
}

void mianWidget::on_pushButton_test_3_clicked()
{

    Node node("systeminfo");
    // puber
    //    MsgShellCMDResult tmp;
//    MsgDiskMonitor tmp1;
    MsgCPUMEMMonitor tmp2;
    srand((int)time(0));
    std::string name = "module_name" + std::to_string(rand() % 100);
    std::cout << name << std::endl;
    // puber
//    auto writer1 = node.CreateWriter<MsgDiskMonitorPubSubType>(Topic_DiskMonitor, QOS_PROFILE_EVENT_LOCAL);
    auto writer2 = node.CreateWriter<MsgCPUMEMMonitorPubSubType>(Topic_CPUMonitor, QOS_PROFILE_EVENT_LOCAL);
    //    while(1){
    int i = 0;


//    for(int i=0;i<40;i++){
//        //           tmp1.diskList()
//        MsgDiskStatST st;
//        st.fileSystem("/test/"); //分区名 如/dev/mmcblk3p1
//        st.mountedOn("/home");  //挂载路径 如/home/work
//        st.size("111");       //总大小 如29G 847M
//        st.used("222");       //使用大小 如9.8G 4.0K
//        st.usage("333");      //使用率 如37%
//        tmp1.diskList().push_back(st);
//    }
//    tmp1.moudle(moudleType::ICU);
//    writer1->Write(tmp1);

    for(int i=0;i<40;i++){
        MsgProcessST st;
        st.process("****");    //进程名 如/home/work/cloudClient
        st.pid(10086); //进程pid
        st.pCPUUsage(110);   // CPU使用率 0.0-100.0
        st.pMEMUsage(119);   //内存使用率 0.0-100.0
        tmp2.processList().push_back(st);
    }
    tmp2.CPUUsage(33);
    tmp2.CPUTemp(56.99);
    tmp2.MEMUsage(56);
    tmp2.moudle(moudleType::ICU);
    writer2->Write(tmp2);
}
