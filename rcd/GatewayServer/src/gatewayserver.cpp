#include "gatewayserver.h"

#include <cstring>
#include <memory>

#include "devmanage.h"

GateWayServer::GateWayServer(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(parent);
    gatewaySub.ConnectTo(ZMQ_ADDR_UI);
    gatewaySub.ConnectTo(ZMQ_ADDR_CONTROL);
    gatewayPub = new ZmqPublisher(ZMQ_ADDR_Gateway);
}

GateWayServer::~GateWayServer()
{
    controlFastDev = nullptr;
    delete gatewayPub;
    delete server;
}

void GateWayServer::Start(int port)
{
    bool ret = server->listen(QHostAddress::Any, port);
    qDebug() << "Start Gateway Tcp Listen:" << port << " ,result:" << ret;
    connect(server, SIGNAL(newConnection()), this, SLOT(HandleNewConnect()));
    gatewaySub.Subscribe(ZMQ_TOPIC_REQ_REMOTE_CONTROL,
                         std::bind(&GateWayServer::HandleZmqReqRemoteControl,
                                   this, std::placeholders::_1,
                                   std::placeholders::_2, std::placeholders::_3));
    gatewaySub.Subscribe(ZMQ_TOPIC_REQ_SEND_HMIMSG,
                         std::bind(&GateWayServer::HandleZmqReqSendHmiMsg,
                                   this, std::placeholders::_1,
                                   std::placeholders::_2, std::placeholders::_3));
}

void GateWayServer::HandleNewConnect()
{
    QTcpSocket *tmp = server->nextPendingConnection();
    if (tmp == nullptr)
    {
        qDebug() << "Connecttion is null.";
        return;
    }
    else
    {
        qDebug() << " Get New Connect.";
        InitNewClient(tmp);
    }
}

void GateWayServer::HandleDevDisConnected(qintptr socketDesc)
{
    devM.DelDev(socketDesc);
}

void GateWayServer::HandleDevReqAuth(qintptr socketDesc,
                                     const std::string &imei, int type)
{
    devM.UpdateDev(socketDesc, imei, type);
}

void GateWayServer::HandleReqControlCmdResult(const std::string &imei,
                                              int result)
{
    if ((result == RESP_REMOTE_CONTROL_VEHICLE_RECV) && (lastReqControlCmd == REQ_REMOTE_CONTROL_START))
    {

        controlFastDev = devM.GetDevHandle(imei);
        gatewaySub.Subscribe(ZMQ_TOPIC_CONTROL_DATA, std::bind(&GateWayServer::HandleZmqControlData,
                                                               this, std::placeholders::_1,
                                                               std::placeholders::_2, std::placeholders::_3));

        gatewaySub.Subscribe(ZMQ_TOPIC_CONTROL_LIGHT, std::bind(&GateWayServer::HandleZmqControlLight,
                                                               this, std::placeholders::_1,
                                                               std::placeholders::_2, std::placeholders::_3));

        connect(this, SIGNAL(EmitControlData(const QByteArray &)),
                controlFastDev.get(), SLOT(HandleControlData(const QByteArray &)));
        connect(this, SIGNAL(EmitControlLight(const QByteArray &)),
                controlFastDev.get(), SLOT(HandleControlLight(const QByteArray &)));
        qDebug() << "Subscribe " << ZMQ_TOPIC_CONTROL_DATA << ", "
                                << ZMQ_TOPIC_CONTROL_LIGHT << " on device[" << imei.c_str() << "]";
    }
    else
    {
        gatewaySub.UnSubscribe(ZMQ_TOPIC_CONTROL_DATA);
        if (controlFastDev != nullptr)
        {
            disconnect(this, SIGNAL(EmitControlData(const QByteArray &)),
                       controlFastDev.get(), SLOT(HandleControlData(const QByteArray &)));
            disconnect(this, SIGNAL(EmitControlLight(const QByteArray &)),
                       controlFastDev.get(), SLOT(HandleControlLight(const QByteArray &)));
        }
        qDebug() << "UnSubscribe " << ZMQ_TOPIC_CONTROL_DATA << ", "
                                << ZMQ_TOPIC_CONTROL_LIGHT << " on device[" << imei.c_str() << "]";
        controlFastDev = nullptr;
    }

    RespRemoteControl resp;
    std::memmove(resp.imei, imei.c_str(), sizeof(resp.imei));
    resp.result = result;
    gatewayPub->Publish(ZMQ_TOPIC_RESP_REMOTE_CONTROL, reinterpret_cast<const void *>(&resp), sizeof(RespRemoteControl));
}

void GateWayServer::HandleVehicleStatus(const std::string &imei, int type,
                                        int status)
{
    if ((status != VEHICLE_STATUS_CONNECTED) && (controlFastDev != nullptr) && (controlFastDev == devM.GetDevHandle(imei)))
    {
        gatewaySub.UnSubscribe(ZMQ_TOPIC_CONTROL_DATA);

        disconnect(this, SIGNAL(EmitControlData(const QByteArray &)),
                   controlFastDev.get(), SLOT(HandleControlData(const QByteArray &)));
        disconnect(this, SIGNAL(EmitControlLight(const QByteArray &)),
                   controlFastDev.get(), SLOT(HandleControlLight(const QByteArray &)));
        controlFastDev = nullptr;
        qDebug() << "del fast dev.";
    }

    VehicleStatus vstatus;
    std::memmove(vstatus.imei, imei.c_str(), imei.size());
    vstatus.type = static_cast<uint8_t>(type);
    vstatus.status = static_cast<uint8_t>(status);
    qDebug() << "Get Status Signal:" << vstatus.status << " " << imei.c_str();
    gatewayPub->Publish(ZMQ_TOPIC_VEHICLE_STATUS, reinterpret_cast<const void *>(&vstatus), sizeof(VehicleStatus));
}

void GateWayServer::HandleVehicleRawData(const QByteArray &rawData)
{
    gatewayPub->Publish(ZMQ_TOPIC_VEHICLE_RAWDATA, rawData.data(), rawData.size());
}

void GateWayServer::HandleVehicleDelay(const std::string &imei, uint32_t delayUs)
{
    VehicleDelay delay = {{0x00},0x00};
    std::memmove(delay.imei, imei.c_str(), IMEI_LENGTH);
    delay.delayUs = delayUs;
    gatewayPub->Publish(ZMA_TOPIC_VEHICLE_DELAY, &delay, sizeof(delay));
}

void GateWayServer::HandleVehicleReportHmiMsg(const std::string &imei, int len, QByteArray &body)
{
    HmiMsg msg = {{0x00},0x00,{0x00}};
    std::memmove(msg.imei, imei.c_str(), IMEI_LENGTH);
    msg.bodyLen = len;
    std::memmove(msg.body, body.data(), len);
    gatewayPub->Publish(ZMQ_TOPIC_REPORT_HMI_MSG, &msg, sizeof(msg));
}

void GateWayServer::HandleZmqReqRemoteControl(const std::string &,
                                              const void *data, int )
{
    ReqRemoteControl reqControl = {{0x00},0x00};
    std::memmove(&reqControl, data, sizeof(ReqRemoteControl));
    std::string imei(reinterpret_cast<char *>(reqControl.imei), IMEI_LENGTH);
    qDebug() << "Get ReqRemoteControl Cmd.Imei: " << imei.c_str()
             << " ,Opt:" << reqControl.opt;
    auto devHandleTmp = devM.GetDevHandle(imei);
    if (devHandleTmp == nullptr)
    {
        qDebug() << "Not Found Dev:" << imei.c_str();
        RespRemoteControl resp;
        std::memmove(resp.imei, imei.c_str(), imei.size());
        resp.result = RESP_REMOTE_CONTROL_VEHICLE_DISCONNECT;
        gatewayPub->Publish(ZMQ_TOPIC_RESP_REMOTE_CONTROL, reinterpret_cast<const void *>(&resp), sizeof(RespRemoteControl));
        return;
    }
    lastReqControlCmd = reqControl.opt;
    emit EmitReqControlCmdSignal(imei, reqControl.opt);
}

void GateWayServer::HandleZmqReqSendHmiMsg(const std::string &,
                                              const void *data, int )
{
    HmiMsg reqSendMsg = {{0x00},0x00,{0x00}};
    std::memmove(&reqSendMsg, data, sizeof(HmiMsg));
    std::string imei(reinterpret_cast<char *>(reqSendMsg.imei), IMEI_LENGTH);
    qDebug() << "Get ReqSendHmiMgs Cmd.Imei: " << imei.c_str()
             << " ,Len:" << reqSendMsg.bodyLen;

    auto devHandleTmp = devM.GetDevHandle(imei);
    if (devHandleTmp == nullptr)
    {
        qWarning() << "Not Found Dev:" << imei.c_str();
        return;
    }
//    lastReqControlCmd = reqControl.opt;
    QByteArray tmpData(reinterpret_cast<const char *>(reqSendMsg.body), sizeof(reqSendMsg.bodyLen));
    emit EmitReqSendHmiMsgCmdSignal(imei, tmpData);
}

void GateWayServer::HandleZmqControlData(const std::string &,
                                         const void *data, int len)
{
    QByteArray tmpB(reinterpret_cast<const char *>(data), len);
    emit EmitControlData(tmpB);
}

void GateWayServer::HandleZmqControlLight(const std::string &, const void *data, int len) {
    QByteArray tmpB(reinterpret_cast<const char *>(data), len);
    emit EmitControlLight(tmpB);
}

void GateWayServer::InitNewClient(QTcpSocket *client)
{
    auto tmpHandle = std::make_shared<DevHandle>(this, client);
    devM.AddDev(client->socketDescriptor(), tmpHandle);

    connect(tmpHandle.get(), SIGNAL(Disconnected(qintptr)),
            this, SLOT(HandleDevDisConnected(qintptr)));
    connect(tmpHandle.get(), SIGNAL(DevReqAuth(qintptr, const std::string &, int)),
            this, SLOT(HandleDevReqAuth(qintptr, const std::string &, int)));
    connect(tmpHandle.get(), SIGNAL(ReqControlCmdResult(const std::string &, int)),
            this, SLOT(HandleReqControlCmdResult(std::string, int)));
    connect(tmpHandle.get(), SIGNAL(VehicleStatusSignal(const std::string &, int, int)),
            this, SLOT(HandleVehicleStatus(std::string, int, int)));
    connect(tmpHandle.get(), SIGNAL(VehicleRawDataSignal(const QByteArray &)),
            this, SLOT(HandleVehicleRawData(const QByteArray &)));
    connect(tmpHandle.get(), SIGNAL(VehicleDelaySignal(const std::string &, uint32_t)),
            this, SLOT(HandleVehicleDelay(const std::string &, uint32_t)));
    connect(tmpHandle.get(), SIGNAL(VehicleReportHmiMsgSignal(const std::string &, int, QByteArray &)),
            this, SLOT(HandleVehicleReportHmiMsg(const std::string &, int, QByteArray &)));

    connect(this, SIGNAL(EmitReqControlCmdSignal(const std::string &, uint8_t)),
            tmpHandle.get(), SLOT(HandleReqControlCmdSlot(const std::string &, uint8_t)));
    connect(this, SIGNAL(EmitReqSendHmiMsgCmdSignal(const std::string &,const QByteArray &)),
            tmpHandle.get(), SLOT(HandleReqSendHmiMsgCmdSlot(const std::string &,const QByteArray &)));
}
