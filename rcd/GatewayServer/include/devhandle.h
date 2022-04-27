#ifndef DEVHANDLE_H
#define DEVHANDLE_H

#include "include/rcdmsg.hpp"
#include <QByteArray>
#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <string>

#define GATEWAY_HEART_TIMEOUT_MS (2000)
#define REQ_CONTROL_CMD_TIEMOUT_MS (2000)
#define REQ_DELAY_CMD_TIMEOUT_MS (1000)

#define CAN_LOST_MAX_COUNTER 5

class DevHandle : public QObject
{
    Q_OBJECT
public:
    explicit DevHandle(QObject *parent = nullptr, QTcpSocket *socket = nullptr);
    virtual ~DevHandle();

private:
    void HandleCmd(uint16_t cmd, const uint8_t *data, int dataLen);
    void HandleAuth(const uint8_t *data, int dataLen);
    void HandleCommAck(const uint8_t *data, int dataLen);
    void HandleReportCan(const uint8_t *data, int dataLen);
    void CheckCanFailed(int pkgCount);
    void HandleDelayCmd(const uint8_t *data, int dataLen);
    void HandleRespControlCmd(const uint8_t *data, int dataLen);
    void HandleReportHmiMsg(const uint8_t *data, int dataLen);

    void SendReqControlCmdToDev(uint8_t opt);
    void SendReqSendHmiMsgToDev(const uint8_t *data, int dataLen);
    void SendControlDataToDev(const uint8_t *data, int dataLen);
    void SendDataToDev(const uint8_t *data, int dataLen);
    void SendCommAckToDev(uint16_t cmd);
    void SendRespAuthToDev(uint8_t result);
    void SendReqDelayToDev();
    void SendControlLightToDev(const uint8_t *data, int dataLen);

    inline uint32_t GetCurTimeMs();

signals:
    void Disconnected(qintptr socketDesc);
    void DevReqAuth(qintptr socketDesc, const std::string &imei, int type);
    void ReqControlCmdResult(const std::string &imei, int result);
    void VehicleStatusSignal(const std::string &imei, int type, int status);
    void VehicleRawDataSignal(const QByteArray &rawData);
    void VehicleDelaySignal(const std::string &imei, uint32_t delayUs);
    void VehicleReportHmiMsgSignal(const std::string &imei, int len, QByteArray &body);

public slots:
    void ReadDataFromDev();
    void DevDisConnected();
    void HeartTimeOutSlots();
    void ControlCmdTimeOutSlots();
    void DelayCmdTimeOutSlots();
    void HandleReqControlCmdSlot(const std::string &imei, uint8_t opt);
    void HandleReqSendHmiMsgCmdSlot(const std::string &imei, const QByteArray &);
    void HandleControlData(const QByteArray &data);
    void HandleControlLight(const QByteArray &data);

private:
    QTcpSocket *devSocket = nullptr;
    std::string devImei;
    uint8_t devType;
    qintptr socketDesc;
    QTimer heartTimer;

    int canLostCounter = 0;
    QTimer reqControlCmdTimer;

    QTimer delayTimer;
};

#endif // DEVHANDLE_H
