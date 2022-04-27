#ifndef HMICMDPACKAGE_H
#define HMICMDPACKAGE_H

#include "System.h"
#include "hmicmd.h"
#include "../../include/jt808/jt808.hpp"

// Hmi消息包编解码处理类

class HmiCmdPackage
{
public:
    HmiCmdPackage();
    int     hmiDecodeG2H(uint8_t *buf, int len);

    //h->g通用
    static  int  hmiEncodeComAck(uint8_t *buf, short cmd);
    //h->g矿卡
    static  int  hmiEncodeReqWorkOpt(uint8_t *buf, struct pubVehicleReqWorkOpt0F21ST &opt);

private:

    int     g2hComAckDeal(Jt808MsgST &msg);
    int     g2hPosDeal(Jt808MsgST &msg);
    int     g2hReqWorkOnStatusDeal(Jt808MsgST &msg);
    int     g2hResWorkOnStatuDeal(Jt808MsgST &msg);
    int     g2hReqWorkOptDeal(Jt808MsgST &msg);
    int     g2hResWorkOptDeal(Jt808MsgST &msg);
    int     g2hResWorkLOptDeal(Jt808MsgST &msg);
    int     g2hResWorkExcavatorOptDeal(Jt808MsgST &msg);
    int     g2hResWorkCrusherOptDeal(Jt808MsgST &msg);
    int     g2hCollWarningDeal(Jt808MsgST &msg);
    int     g2hVehicleAroundDeal(Jt808MsgST &msg);
    int     g2hVersionDeal(Jt808MsgST &msg);
    int     g2hDownFileDeal(Jt808MsgST &msg);
    int     g2hStartUpDeal(Jt808MsgST &msg);
    int     g2hShutDownAckDeal(Jt808MsgST &msg);
    int     g2hDevNameDeal(Jt808MsgST &msg);
    int     g2hParaQueryDeal(Jt808MsgST &msg);
    int     g2hParaNotifyDeal(Jt808MsgST &msg);
    int     g2hUpFileAckDeal(Jt808MsgST &msg);
    int     g2hResTestDeal(Jt808MsgST &msg);

     //g->h矿卡
    int     g2hReqManDeal(Jt808MsgST &msg);
    int     g2hReqRemoteDeal(Jt808MsgST &msg);
    int     g2hResRemoteDeal(Jt808MsgST &msg);
    int     g2hMasterInfoDeal(Jt808MsgST &msg);
    int     g2hReqTaskDeal(Jt808MsgST &msg);
    int     g2hResTaskDeal(Jt808MsgST &msg);
    int     g2hReqRoadDeal(Jt808MsgST &msg);
    int     g2hResRoadDeal(Jt808MsgST &msg);
    int     g2hFreeRoadDeal(Jt808MsgST &msg);
    int     g2hTemporaryTaskDeal(Jt808MsgST &msg);
    int     g2hMasterWarnningDeal(Jt808MsgST &msg);
    int     g2hDrivePermissionDeal(Jt808MsgST &msg);
    int     g2hAreaBoundaryDeal(Jt808MsgST &msg);
    int     g2hClearErrorDeal(Jt808MsgST &msg);

    //功能函数
    void    warnninOpt(char *buf, int len);


public:

    QVector<ModuleDir>   m_moduleDirs;
    QVector<int>         m_cmds;
    QVector<QVariant>    m_datas;
    uint16_t    m_hmicmd;

};

#endif // HMICMDPACKAGE_H
