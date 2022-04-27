#ifndef GATEWAYSERVER_H
#define GATEWAYSERVER_H

#include "devhandle.h"
#include "devmanage.h"
#include "zmq/zmqwrap.hpp"
#include <QByteArray>
#include <QObject>
#include <QTcpServer>
#include <string>

class GateWayServer : public QObject
{
    Q_OBJECT
public:
    explicit GateWayServer(QObject *parent = nullptr);
    ~GateWayServer();
    void Start(int port);

public slots:
    void HandleNewConnect();
    void HandleDevDisConnected(qintptr socketDesc);
    void HandleDevReqAuth(qintptr socketDesc, const std::string &imei, int type);
    void HandleReqControlCmdResult(const std::string &imei, int result);
    void HandleVehicleStatus(const std::string &imei, int type, int status);
    void HandleVehicleRawData(const QByteArray &rawData);
    void HandleVehicleDelay(const std::string &imei, uint32_t delayUs);
    void HandleVehicleReportHmiMsg(const std::string &imei, int len, QByteArray &body);

signals:
    void EmitReqControlCmdSignal(const std::string &imei, uint8_t opt);
    void EmitReqSendHmiMsgCmdSignal(const std::string &imei, const QByteArray &data);
    void EmitControlData(const QByteArray &data);
    void EmitControlLight(const QByteArray& data);

private:
    void HandleZmqReqRemoteControl(const std::string &topic, const void *data, int len);
    void HandleZmqReqSendHmiMsg(const std::string &topic, const void *data, int len);
    void HandleZmqControlData(const std::string &topic, const void *data, int len);
    void HandleZmqControlLight(const std::string &topic, const void *data, int len);

    void InitNewClient(QTcpSocket *client);

private:
    std::shared_ptr<DevHandle> controlFastDev = nullptr;
    QTcpServer *server = nullptr;
    DevManage devM;
    ZmqSubscriber gatewaySub;
    ZmqPublisher *gatewayPub = nullptr;
    int lastReqControlCmd;
};

#endif // GATEWAYSERVER_H
