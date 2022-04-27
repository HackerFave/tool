#include "mianWidget.h"

#include <QApplication>
#include "idl/systemInfo.h"
#include "idl/systemInfoPubSubTypes.h"
#include "idl/systemInfoTypeObject.h"
#include "dds_wrapper/include/node.hpp"
#include <QtDebug>
using namespace tg_msgs::dds;

void Callback(const MsgShellCMDResult &tmp)
{
    //    printf("recv tmp: %d\n", tmp.index());
    //     qDebug()<<tmp.cmd().c_str()<<":"<<tmp.moudle()<<":"<<tmp.result().c_str()<<"********************************************";
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mianWidget w;
    w.show();
    Node node("systeminfo");
    Node node1("systeminfo1");
    Node node2("systeminfo2");
    Node node3("systeminfo3");
    // puber

    //    auto writer = node.CreateWriter<MsgShellCMDResultPubSubType>(Topic_ShellCMDResult,QOS_PROFILE_EVENT_LOCAL);
    //    while (1) {
    //        sleep(1);
    //        tmp.cmd("我在测试---fuck all！");
    //        tmp.moudle(moudleType::HMI);
    //        tmp.result("日了狗了，我猜你看到我了……");
    //        writer->Write(tmp);
    //        qDebug()<<"**********************************";
    //    }


    srand((int)time(0));
    std::string name = "module_name" + std::to_string(rand() % 100);
    std::cout << name << std::endl;
    // puber
    auto writer = node.CreateWriter<MsgShellCMDResultPubSubType>(Topic_ShellCMDResult, QOS_PROFILE_EVENT_LOCAL);

    auto writer1 = node.CreateWriter<MsgDiskMonitorPubSubType>(Topic_DiskMonitor, QOS_PROFILE_EVENT_LOCAL);
    auto writer2 = node.CreateWriter<MsgCPUMEMMonitorPubSubType>(Topic_CPUMonitor, QOS_PROFILE_EVENT_LOCAL);

    auto writer11 = node1.CreateWriter<MsgDiskMonitorPubSubType>(Topic_DiskMonitor, QOS_PROFILE_EVENT_LOCAL);
    auto writer22 = node1.CreateWriter<MsgCPUMEMMonitorPubSubType>(Topic_CPUMonitor, QOS_PROFILE_EVENT_LOCAL);
    auto writer111 = node2.CreateWriter<MsgDiskMonitorPubSubType>(Topic_DiskMonitor, QOS_PROFILE_EVENT_LOCAL);
    auto writer222 = node2.CreateWriter<MsgCPUMEMMonitorPubSubType>(Topic_CPUMonitor, QOS_PROFILE_EVENT_LOCAL);
    auto writer1111 = node3.CreateWriter<MsgDiskMonitorPubSubType>(Topic_DiskMonitor, QOS_PROFILE_EVENT_LOCAL);
    auto writer2222 = node3.CreateWriter<MsgCPUMEMMonitorPubSubType>(Topic_CPUMonitor, QOS_PROFILE_EVENT_LOCAL);

    while(1){
        int i = 0;
        MsgShellCMDResult tmp;
        MsgDiskMonitor tmp1;
        MsgCPUMEMMonitor tmp2;
        MsgDiskMonitor tmp11;
        MsgCPUMEMMonitor tmp22;
        MsgDiskMonitor tmp111;
        MsgCPUMEMMonitor tmp222;
        MsgDiskMonitor tmp1111;
        MsgCPUMEMMonitor tmp2222;
        tmp.cmd("我在测试---fuck all！");
        tmp.moudle(moudleType::HMI);
        tmp.result("我猜你看到我了……");
        writer->Write(tmp);

        for(int i=0;i<40;i++){
            //           tmp1.diskList()
            MsgDiskStatST st;
            st.fileSystem("/test/"); //分区名 如/dev/mmcblk3p1
            st.mountedOn("/home");  //挂载路径 如/home/work
            st.size("200");       //总大小 如29G 847M
            st.used("99");       //使用大小 如9.8G 4.0K
            st.usage("22");      //使用率 如37%
            tmp1.diskList().push_back(st);
            tmp11.diskList().push_back(st);
            tmp111.diskList().push_back(st);
            tmp1111.diskList().push_back(st);
        }
        tmp1.moudle(moudleType::HMI);
        tmp111.moudle(moudleType::CGU);
        tmp11.moudle(moudleType::CGU_TBOX);
        tmp1111.moudle(moudleType::ICU);
        writer1->Write(tmp1);
        writer11->Write(tmp11);
        writer111->Write(tmp111);
        writer1111->Write(tmp1111);

        for(int i=0;i<40;i++){
            MsgProcessST st;
            st.process("/home/work/cloudClient");    //进程名 如/home/work/cloudClient
            st.pid(10086); //进程pid
            st.pCPUUsage(38);   // CPU使用率 0.0-100.0
            st.pMEMUsage(89);   //内存使用率 0.0-100.0
            tmp2.processList().push_back(st);
            tmp22.processList().push_back(st);
            tmp222.processList().push_back(st);
            tmp2222.processList().push_back(st);
        }
        tmp2.CPUUsage(33);
        tmp2.CPUTemp(56.99);
        tmp2.MEMUsage(56);
        tmp2.moudle(moudleType::HMI);
        tmp22.moudle(moudleType::CGU);
        tmp222.moudle(moudleType::CGU_TBOX);
        tmp2222.moudle(moudleType::ICU);
        writer2->Write(tmp2);
        writer22->Write(tmp22);
        writer222->Write(tmp222);
        writer2222->Write(tmp2222);
        sleep(3);
    }
    return a.exec();
}
