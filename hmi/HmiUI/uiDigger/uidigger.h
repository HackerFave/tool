#ifndef UIDIGGER_H
#define UIDIGGER_H

#include <QWidget>
#include <QButtonGroup>
#include <QGridLayout>

#include <QTimer>
#include <QDateTime>

#include "fixsetdigger.h"
#include "abnormaldigger.h"

#include "HmiUI/keyboard.h"
#include "HmiUI/delaychoice.h"
#include "HmiUI/HmiMap/hmimap.h"
#include "HmiUI/HmiMap/stoppoints.h"

#include "HmiFunction/hmicmdst.h"
#include "HmiFunction/cmddealclass.h"
#include "HmiFunction/posinfo.h"
#include "HmiFunction/paraparsing.h"
#include "HmiFunction/worklistdeal.h"
#include "HmiFunction/stoplistdeal.h"
#include "HmiFunction/commonuifunction.h"

QT_BEGIN_NAMESPACE
namespace Ui { class uiDigger; }
QT_END_NAMESPACE

class uiDigger : public QWidget
{
    Q_OBJECT

public:
    uiDigger(QWidget *parent = NULL);
    ~uiDigger();

private slots:
    void on_stateSetBtn_clicked();

    void on_loginOutBtn_clicked();

    void on_otherSetBtn_clicked();

    void on_loginOutBackBtn_clicked();

    void on_loginOutOkBtn_clicked();

    void on_otherSetBackBtn_clicked();

    void on_outWorkBackBtn_clicked();

    void on_stateSetBackBtn_clicked();

    void on_workSetBackBtn_clicked();

    void on_readyBackBtn_clicked();

    void on_faultBackBtn_clicked();

    void on_faultOkBtn_clicked();

    void on_delay10Btn_clicked();

    void on_delay30Btn_clicked();

    void on_delay60Btn_clicked();

    void on_delayBackBtn_clicked();

    void on_loadStateBackBtn_clicked();

    void on_addOilBackBtn_clicked();

    void on_addOilOkBtn_clicked();

    void on_outWorkOkBtn_clicked();

    void on_faultBtn_clicked();

    void on_emptyLoadBtn_clicked();

    void on_fullLoadBtn_clicked();

    void on_readyBtn_clicked();

    void on_readyOkBtn_clicked();

    void on_delayBtn_clicked();

    void on_voiceAdjustBtn_clicked();

    void on_brightAdjustBtn_clicked();

    void on_editionBtn_clicked();

    void on_brightBackBtn_clicked();

    void on_brightMinusBtn_clicked();

    void on_brightPlusBtn_clicked();

    void on_editionBackBtn_clicked();

    void on_voiceBackBtn_clicked();

    void on_halfLoadBtn_clicked();

    void on_emptyBackBtn_clicked();

    void on_emptyOkBtn_clicked();

    void on_halfOkBtn_clicked();

    void on_halfBackBtn_clicked();

    void on_fullOkBtn_clicked();

    void on_fullBackBtn_clicked();
	
private slots:
    void g2hPosSlot(struct pubVehiclePosST);  //网关发送hmi位置信息

    void g2hReqWorkOnStatuSlot(struct pubVehicleWorkStatuST); //平台发送网关运营状态变更

    void g2hResWorkOnStatuSlot(struct pubVehicleResWorkStatuST); //收到平台运营状态变更确认

    void g2hReqWorkOptSlot(struct pubVehicleReqWorkOptST); //网关请求现场作业

    void g2hReqWorkRealOptSlot(struct pubVehicleReqWorkOptST); //网关请求现场作业

    void g2hResWorkOptSlot(struct pubVehicleResWorkOptST); //辅助车辆（网关已收到矿卡现场作业），矿卡（接收到辅助车辆相应作业信息）

    void g2hCollWarnningSlot(struct pubVehicleCollWarnningST); //预警

    void g2hDownMapFileSlot(int, QString); //接收网关下载地图命令

    void g2hVersionSlot(struct versionST); //版本号

    void g2hDownTaskFileSlot(bool, QString, QStringList); //下载文件

    void g2hStartUpSlot(struct startUpST); //网关发送开机命令

    void g2hShutDownAckSlot(); //网关关机应答

    void g2hDevNameSlot(struct pubVehicleDevNameST); //网关检权获取名称后发送给hmi

    void g2hMatAckSlot(short mater); //物料变更

    void g2hReqTaskSlot(struct pubVehicleReqTaskST ); //请求任务

    void g2hResTaskSlot(struct pubVehicleResTaskST ); //接收平台响应任务

    void g2hReqRoadSlot(struct pubVehicleReqRoadST ); //申请路权

    void g2hResRoadSlot(struct pubVehicleResRoadST ); //平台回复路权申请

    void g2hFreeRoadSlot(struct pubVehicleReqRoadST ); //平台下发释放路权

    void g2hTemporaryTaskSlot(char ); //平台下发临时调度作业

    void g2hMasterWarnningSlot(struct pubVehicleMasterWarnningST); //主控发送预警

    void g2hGatewayOffline(bool);//网关掉线

	void insertWarnningSlots(int num);

    void removeWarnningSlots(int num);

    void titleTimerSlot();

    void messageEndTimerSlot();

    void blinkTimerSlot();

    void collectionTimerSlot();

    void mineInfoTimerSlot();

    void aroundVehicleUpdateTimerSlot();

    void on_messageUpBtn_clicked();

    void on_messageDownBtn_clicked();

    void on_collectionBackBtn_clicked();

    void on_startCollectionBtn_clicked();

    void on_updateDataBtn_clicked();

    void on_leftBtn_clicked();

    void on_rightBtn_clicked();

    void on_suspendRestartBtn_clicked();

    void on_stopBtn_clicked();

    void on_collectionOverBackBtn_clicked();

    void on_saveBtn_clicked();

    void on_deleteBtn_clicked();

    void on_updateDataOkBtn_clicked();

    void on_updateDataBackBtn_clicked();

    void on_deleteDataOkBtn_clicked();

    void on_deleteDataBackBtn_clicked();

    void on_unLoadWorkSetBtn_clicked();

    void on_abnormalOkBtn_clicked();

    void on_abnormalBtn_clicked();

    void on_abnormalBackBtn_clicked();

    void on_unLoadRunWorkBtn_clicked();

//    void on_unLoadOverOutBtn_clicked();

    void on_emergenceBtn_clicked();

    void on_voiceMinusBtn_clicked();

    void on_voicePlusBtn_clicked();

    void on_useCurBoundaryBtn_clicked();

    void on_diggerWorkBackBtn_clicked();

    void on_collectionBtn_clicked();

    void on_updateOldDataBackBtn_clicked();

    void on_updateOldDataOkBtn_clicked();

    void on_collectionValueOkBtn_clicked();

    void on_collectionValueBackBtn_clicked();

    void on_collectionUploadBtn_clicked();

signals:
    void  h2gSetWorkStatuSignal(pubVehicleWorkStatuST statu); //设置运营状态

    void  h2gUpFileSignal(fileUpST); //文件上传,fileDir不用部分为0

    void  h2gShutDownSignal();  //发送网关关机命令

    void  h2gReqWorkOptSignal(pubVehicleReqWorkOptST opt); //相应现场调度0305

    void h2gSetLoadSignal(char);//设置物料类型

    void  h2gReqMaterSignal(short mater); //设置物料类型

    void h2gResWorkOptSignal(pubVehicleResWorkOptST);
    void addDbInfoSignal(QString path);//加载地图
    void initViewSignal(QMatrix);

//    void  h2gReqWorkOptSignal(struct pubVehicleReqWorkOptST); //发送请求作业操作命令，主要驶推出作业

private:
    Ui::uiDigger *ui;
    HmiMap* map;
    DelayChoice* delayChoice;
    KeyBoard* keyBoard;
    FixSetDigger* fixSet;
    AbnormalDigger* abnormalChoice;

    CmdDealClass* hmicmd;

    QTimer* titleTimer;//显示信息更新定时器
    QTimer* messageEndTimer;//信息下刷定时器
    QTimer* blinkTimer;//闪烁定时器
    bool blinkFlag;//闪烁标志位
    QTimer* collectionTimer;//采集定时器
    QTimer* mineInfoTimer;//矿车信息更新显示定时器
    QTimer* aroundVehicleUpdateTimer;//周边车辆更新定时器

    struct pubVehicleDevNameST localImeiName;//本机信息
    struct pubVehicleResTaskST taskInfo; //目标信息
    struct pubVehicleResWorkOptST workStateInfo;//当前车辆作业信息

    union versionUN versiondata;//版本信息

    QList<struct pubVehicleCollWarnningST> aroundVehicleList;//周边车辆链表
    QList<struct collectlatlngheading*> collectionList;
    StopPoints *stopPointItem=NULL;
    char areaside;//0x01:左侧为行驶区;0x02:右侧为行驶区
    char areaType;//0:道路边界 2：装载 3卸载区

    QMap<struct pubVehicleDevNameStateST, int>   imeiStopNumMap;     //imei-停靠车号对应表
    QMap<int, struct pubVehicleDevNameStateST>   stopNumImeiMap;     //停靠车号-imei对应表

    double sidefixvalue;//偏执角度
    signed char focusrotation;//转角
    int currentStopNum;//当前停靠车位号

};
#endif // UIDIGGER_H