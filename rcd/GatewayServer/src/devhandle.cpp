#include "devhandle.h"
#include "devmanage.h"
#include "jt808/jt808.hpp"
#include "include/msgcmd.hpp"
#include <QDebug>

std::atomic<uint16_t> JT808::serialNum(0);

DevHandle::DevHandle(QObject *parent, QTcpSocket *socket)
    : QObject(parent), devSocket(socket)
{
    connect(devSocket, SIGNAL(readyRead()), this, SLOT(ReadDataFromDev()));
    connect(devSocket, SIGNAL(disconnected()), this, SLOT(DevDisConnected()));

    socketDesc = devSocket->socketDescriptor();

    heartTimer.setInterval(GATEWAY_HEART_TIMEOUT_MS);
    connect(&heartTimer, SIGNAL(timeout()), this, SLOT(HeartTimeOutSlots()));
    heartTimer.start();

    reqControlCmdTimer.setInterval(REQ_CONTROL_CMD_TIEMOUT_MS);
    reqControlCmdTimer.setSingleShot(true);
    connect(&reqControlCmdTimer, SIGNAL(timeout()), this, SLOT(ControlCmdTimeOutSlots()));

    delayTimer.setInterval(REQ_DELAY_CMD_TIMEOUT_MS);
    connect(&delayTimer, SIGNAL(timeout()), this, SLOT(DelayCmdTimeOutSlots()));
    qDebug() << " Get New Client.Socket:" << socketDesc;
}

DevHandle::~DevHandle()
{
    qDebug() << " DevHandle release.Imei:" << devImei.c_str();
    heartTimer.stop();
    reqControlCmdTimer.stop();
    delayTimer.stop();
    devSocket->close();
}

void DevHandle::SendReqControlCmdToDev(uint8_t opt)
{
    qDebug() << "SendReqControlCmdToDev";
    Jt808MsgST sendMsg;
    sendMsg.msgId = RCD_CONTROL_CMD;
    sendMsg.body[0] = opt;
    sendMsg.bodyLen = 1;
    uint8_t buf[256] = {0x00};
    int ret = JT808::enCodeMsg(JT808::defaultMsg, sendMsg, buf);
    SendDataToDev(buf, ret);
    reqControlCmdTimer.start();
}

void DevHandle::SendReqSendHmiMsgToDev(const uint8_t *data, int dataLen)
{
    qDebug() << "SendReqSendHmiMsgToDev";
    Jt808MsgST sendMsg;
    sendMsg.msgId = RCD_HMIMSG_CMD;
    std::memmove(sendMsg.body, data, dataLen);
    sendMsg.bodyLen = dataLen;
    uint8_t buf[256] = {0x00};
    int ret = JT808::enCodeMsg(JT808::defaultMsg, sendMsg, buf);
    SendDataToDev(buf, ret);
}

void DevHandle::SendControlDataToDev(const uint8_t *data, int dataLen)
{
    Jt808MsgST sendMsg;
    sendMsg.msgId = RCD_CONTROL_DATA;
    std::memmove(sendMsg.body, data, dataLen);
    sendMsg.bodyLen = dataLen;
    uint8_t buf[256] = {0x00};
    int ret = JT808::enCodeMsg(JT808::defaultMsg, sendMsg, buf);
    SendDataToDev(buf, ret);
}

void DevHandle::SendControlLightToDev(const uint8_t *data, int dataLen) {
    Jt808MsgST sendMsg;
    sendMsg.msgId = RCD_CONTROL_LIGHT;
    std::memmove(sendMsg.body, data, dataLen);
    sendMsg.bodyLen = dataLen;
    uint8_t buf[256] = {0x00};
    int ret = JT808::enCodeMsg(JT808::defaultMsg, sendMsg, buf);
    SendDataToDev(buf, ret);
}

void DevHandle::HandleCmd(uint16_t cmd, const uint8_t *data, int dataLen)
{
    //qDebug("Get Cmd:%04x\n", cmd);
    heartTimer.start();
    switch (cmd)
    {
    case GATEWAY_REQ_AUTH:
    {
        HandleAuth(data, dataLen);
        break;
    }
    case GATEWAY_COM_ACK:
    {
        HandleCommAck(data, dataLen);
        break;
    }
    case GATEWAY_REPORT_VEHICLE_DATA:
    {
        HandleReportCan(data, dataLen);
        break;
    }
    case RCD_DELAY_CMD:
    {
        HandleDelayCmd(data, dataLen);
        break;
    }
    case GATEWAY_RESP_CONTROL_CMD:
        HandleRespControlCmd(data, dataLen);
        break;
    case GATEWAY_REPORT_HMI_MSG:
        HandleReportHmiMsg(data, dataLen);
        break;
    default:
    {
        qDebug() << "Not Support Cmd.";
        break;
    }
    }
}

void DevHandle::HandleAuth(const uint8_t *data, int dataLen)
{
    GateWayReqAuth reqAuth = {{0x00},0x00};
    std::memmove(&reqAuth, data, dataLen);
    devImei.assign(reinterpret_cast<const char *>(reqAuth.imei), IMEI_LENGTH);
    devType = reqAuth.type;
    qDebug() << "ReqAuth , IMEI:" << devImei.c_str() << " Type:" << devType << " , and emit [connected]";
    emit DevReqAuth(devSocket->socketDescriptor(), devImei, devType);
    emit VehicleStatusSignal(devImei, devType, VEHICLE_STATUS_CONNECTED);
    SendRespAuthToDev(1);
    delayTimer.start();
}

void DevHandle::HandleCommAck(const uint8_t *data, int dataLen)
{
    CommAck ack;
    std::memmove(&ack, data, dataLen);
    qDebug("Get Device[%s] CommAck,ack cmdId[%04x]\n", devImei.c_str(), ack.cmdId);
}

void DevHandle::HandleReportCan(const uint8_t *data, int )
{
    int count = data[0];
    int index = 1;

    int newDataLen = sizeof(VehicleRawData) + (count * sizeof(CanRawData));
    char *tmpPtr = new char[newDataLen];
    VehicleRawData *rawData = reinterpret_cast<VehicleRawData *>(tmpPtr);
    rawData->length = count;
    std::memmove(rawData->imei, devImei.c_str(), IMEI_LENGTH);
    rawData->type = static_cast<uint8_t>(devType);
    qDebug() << " Get " << count << " frams";
    for (int i = 0; i < count; i++)
    {
        std::memmove(&rawData->rawData[i].canId, &data[index], sizeof(rawData->rawData[0].canId));
        index += 4;
        int canDataLen = data[index++];
        std::memmove((&rawData->rawData[i].data), &data[index], canDataLen);
        index += canDataLen;
    }

    QByteArray canDataByte(tmpPtr, newDataLen);
    delete[] tmpPtr;
    emit VehicleRawDataSignal(canDataByte);
}

void DevHandle::CheckCanFailed(int pkgCount)
{
    if (pkgCount != 0)
    {
        if (canLostCounter != 0)
        {
            qDebug() << "Device[" << devImei.c_str() << "],Get Can pkg is not zero. and Emit [connected]";
            emit VehicleStatusSignal(devImei, devType, VEHICLE_STATUS_CONNECTED);
        }
        canLostCounter = 0;
    }
    else
    {
        canLostCounter++;
        if (canLostCounter >= CAN_LOST_MAX_COUNTER)
        {
            qDebug() << "Device[" << devImei.c_str() << "],lost Can pkg . and Emit [no can]";
            emit VehicleStatusSignal(devImei, devType, VEHICLE_STATUS_NO_CAN);
        }
    }
}

void DevHandle::HandleDelayCmd(const uint8_t *data, int dataLen)
{
    uint32_t t2 = GetCurTimeMs();
    uint32_t t1 = 0;
    std::memmove(&t1, data, dataLen);
    uint32_t delayTime = (t2 - t1) / 2 * 1000;
    emit VehicleDelaySignal(devImei, delayTime);
    qDebug() << "Get Resp Delay Cmd,t1:" << t1 << " t2:" << t2 << " Ping:" << delayTime;
}

void DevHandle::HandleRespControlCmd(const uint8_t *data, int )
{
    reqControlCmdTimer.stop();
    int result = RESP_REMOTE_CONTROL_VEHICLE_REJECT;
    if (data[1] == 2)
    {
        result = RESP_REMOTE_CONTROL_VEHICLE_RECV;
    }
    emit ReqControlCmdResult(devImei, result);
}

void DevHandle::HandleReportHmiMsg(const uint8_t *data, int len)
{
    QByteArray body(reinterpret_cast<const char *>(data), len);
    emit VehicleReportHmiMsgSignal(devImei, len, body);
}

void DevHandle::SendDataToDev(const uint8_t *data, int dataLen)
{
    devSocket->write(reinterpret_cast<const char *>(data), dataLen);
}

void DevHandle::SendCommAckToDev(uint16_t cmd)
{
    CommAck ack;
    ack.cmdId = cmd;

    Jt808MsgST sendMsg;
    sendMsg.msgId = RCD_COM_ACK;
    std::memmove(sendMsg.body, &ack, sizeof(CommAck));
    sendMsg.bodyLen = sizeof(CommAck);
    uint8_t buf[256] = {0x00};
    int ret = JT808::enCodeMsg(JT808::defaultMsg, sendMsg, buf);
    SendDataToDev(buf, ret);
    qDebug("Resp CommAck to Device[%s],ack cmdId[%04x]\n", devImei.c_str(), cmd);
}

void DevHandle::SendRespAuthToDev(uint8_t result)
{
    RcdRespAuth resp;
    resp.result = result;

    Jt808MsgST sendMsg;
    sendMsg.msgId = RCD_RESP_AUTH;
    std::memmove(sendMsg.body, &resp, sizeof(RcdRespAuth));
    sendMsg.bodyLen = sizeof(RcdRespAuth);
    uint8_t buf[256] = {0x00};
    int ret = JT808::enCodeMsg(JT808::defaultMsg, sendMsg, buf);
    SendDataToDev(buf, ret);
    qDebug("Resp Auth to Device[%s],result:%d\n", devImei.c_str(), result);
}

void DevHandle::SendReqDelayToDev()
{
    Jt808MsgST sendMsg;
    sendMsg.msgId = RCD_DELAY_CMD;
    uint32_t curTimeUs = GetCurTimeMs();
    std::memmove(sendMsg.body, &curTimeUs, sizeof(curTimeUs));
    sendMsg.bodyLen = sizeof(curTimeUs);
    uint8_t buf[128] = {0x00};
    int ret = JT808::enCodeMsg(JT808::defaultMsg, sendMsg, buf);
    SendDataToDev(buf, ret);
    qDebug() << "Req delay cmd:" << curTimeUs;
}

uint32_t DevHandle::GetCurTimeMs()
{
    auto curTimePoint = std::chrono::steady_clock::now();
    auto timeMsTp = std::chrono::time_point_cast<std::chrono::milliseconds>(curTimePoint);
    return timeMsTp.time_since_epoch().count();
}

void DevHandle::ReadDataFromDev()
{
    while (true)
    {
        char data[1024] = {0x00};
        int ret = devSocket->readLine(data, sizeof(data));
        if (ret <= 0)
        {
            return;
        }
        Jt808MsgST recvMsg;
        if (JT808::deCodeMsg(JT808::defaultMsg, reinterpret_cast<uint8_t *>(data), ret, recvMsg) == -1)
        {
            continue;
        }
        HandleCmd(recvMsg.msgId, recvMsg.body, recvMsg.bodyLen);
    }
}

void DevHandle::DevDisConnected()
{
    qDebug() << "DevDisConnected: " << socketDesc << " ,Imei:" << devImei.c_str();
    if (!devImei.empty())
    {
        emit VehicleStatusSignal(devImei, devType, VEHICLE_STATUS_DISCONNECTED);
    }
    emit Disconnected(socketDesc);
}

void DevHandle::HeartTimeOutSlots()
{
    qDebug() << "HeartTimer out";
    qWarning() << "Device[" << devImei.c_str() << "] lost CanReport. close tcp";
    devSocket->close();
}

void DevHandle::ControlCmdTimeOutSlots()
{
    qDebug() << "ControlCmdTimer out";
    qWarning() << "Device[" << devImei.c_str() << "] Recv Resp ControlCmd TimeOut.";
    emit ReqControlCmdResult(devImei, RESP_REMOTE_CONTROL_VEHICLE_TIMEOUT);
}

void DevHandle::DelayCmdTimeOutSlots()
{
    SendReqDelayToDev();
}

void DevHandle::HandleReqControlCmdSlot(const std::string &imei, uint8_t opt)
{
    qDebug() << "Send ReqControl start";
    if (!devImei.empty() && !imei.empty() && (devImei.compare(imei) == 0))
    {
        SendReqControlCmdToDev(opt);
    }
}

void DevHandle::HandleReqSendHmiMsgCmdSlot(const std::string &imei,const QByteArray &data)
{
    qDebug() << "Send ReqSendHmiMsg start";
    if (!devImei.empty() && !imei.empty() && (devImei.compare(imei) == 0))
    {
        SendReqSendHmiMsgToDev(reinterpret_cast<const uint8_t *>(data.data()), data.size());
    }
}

void DevHandle::HandleControlData(const QByteArray &data)
{
    qDebug() << "Send ControlData to Dev ";
    SendControlDataToDev(reinterpret_cast<const uint8_t *>(data.data()), data.size());
}

void DevHandle::HandleControlLight(const QByteArray &data) {
    qDebug() << "Send ControlLight to Dev ";
    SendControlLightToDev(reinterpret_cast<const uint8_t *>(data.data()), data.size());
}
