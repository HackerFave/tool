#ifndef ZmqMsgPackage_H
#define ZmqMsgPackage_H

#include "System.h"
#include <QVariant>

typedef struct
{
    uint8_t moduleType;
    uint8_t cmd;
    uint16_t length;
    char forwardData[0];
} IpcForwardData;

// Zmq消息包编解码处理类
class ZmqMsgPackage
{
public:
    ZmqMsgPackage();

    bool     c2uButtonKey(const void *data, int len);
    bool     g2uVehicleRawData(const void *data, int len);
    bool     g2uVehicleStatus(const void *data, int len);
    bool     g2uVehicleDelay(const void *data, int len);
    bool     g2uRespRemotel(const void *data, int len);
    bool     u2uForward(const void *data, int len);

    static  bool  encodeIpcForward(QByteArray &dest, ModuleDir &moduleDir, int &cmd, QVariant &data);
    static  bool  encodeReqRemote(QByteArray &dest, QString &imei,uint8_t cmd);
    static  bool  encodeReqSendHmiMsg(QByteArray &dest, QString &imei,QByteArray &data);

public:

    QVector<ModuleDir>   m_moduleDirs;
    QVector<int>         m_cmds;
    QVector<QVariant>    m_datas;
    QString              m_imei;

};

#endif // ZmqMsgPackage_H
