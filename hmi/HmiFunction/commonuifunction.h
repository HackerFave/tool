#ifndef COMMONUIFUNCTION_H
#define COMMONUIFUNCTION_H
#include<QMutex>
#include<QWidget>
#include<QTableWidget>
#include<QTextBrowser>
#include<QDateTime>
#include <QObject>
#include<QLabel>
#include<QStackedWidget>
#include"paraparsing.h"
#include"hmicmdst.h"
#include"cmddealclass.h"
#include "HmiUI/HmiMap/hmimap.h"
#include "HmiUI/screensaver.h"
#include "worklistdeal.h"
#define __I
#define __O
#define __IO
class commonUIFunction : public QObject
{
    Q_OBJECT
public:
    commonUIFunction();
    static commonUIFunction *instance();
    void g2hVersionSlot(__IO QTableWidget *editionTable,__I struct versionST versiondataSt,__O union versionUN *versiondata);
    void g2hReqTaskSlot(__IO QTextBrowser *messageBrowser,__I struct pubVehicleReqTaskST reqTask); //请求任务
    void g2hGatewayOffline(__IO QTextBrowser *messageBrowser,bool status);
    void setHmiCMD(__I CmdDealClass*hmicmd);
    void g2hTemporaryTaskSlot(__IO QTextBrowser *messageBrowser,__I char type); //平台下发临时调度作业
    void g2hResTaskSlot(__IO QTextBrowser *messageBrowser,__IO QLabel *titleDestLabel,
                        __I struct pubVehicleResTaskST resTask,
                        __O struct pubVehicleResTaskST &taskInfo); //接收平台响应任务
    void g2hDownPermissionFileSlot(bool flag, QStringList permissionList,quint32 endPointNum,
                                                     QTextBrowser *messageBrowser,
                                                     HmiMap *map);
    void messageShow(QTextBrowser *messageBrowser,const QString & str);
    void g2hResWorkOnStatuSlot(QTextBrowser *messageBrowser,struct pubVehicleResWorkStatuST cloudReq);
    void on_brightAdjustBtn_clicked(QStackedWidget *controlStackedWidget,QWidget *controlBrightPage);
    void g2hResWorkOptSlot(QTextBrowser *messageBrowser,struct pubVehicleResWorkOptST opt); //辅助车辆（网关已收到矿卡现场作业），矿卡（接收到辅助车辆相应作业信息）
    void g2hReqWorkRealOptSlot(QTextBrowser *messageBrowser,struct pubVehicleReqWorkOptST reqWork); //网关请求现场作业
    void g2hReqWorkOptSlot(QTextBrowser *messageBrowser,struct pubVehicleReqWorkOptST reqWork); //网关请求现场作业


public slots:
    void h2gSendHmiVerCMD();
private:
    static commonUIFunction *handle;
    static QMutex commonMutex;
    CmdDealClass *hmicmd;
signals:
    void h2gSendHmiVerSignal(struct versionST version);
};

#endif // COMMONUIFUNCTION_H
