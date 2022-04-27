#include "mq_client.h"
mq_client::mq_client(const std::string &ca,const std::string &client_cert,
                     const std::string & privateKey,const std::string & ip,const std::string & port):
    _ca(ca),
    _client_cert(client_cert),
    _privateKey(privateKey),
    _ip(ip),
    _port(port)

{    
    std::cout<<_ca <<_client_cert<<_privateKey<<_ip<<_port<<std::endl;
#if 1
    tmpOpenOpts.auth = AmqpClient::Channel::OpenOpts::BasicAuth("admin", "123456");
    tmpOpenOpts.heart_beat = 60;
    tmpOpenOpts.host = _ip;
    tmpOpenOpts.port =atoi(_port.c_str());
    _tls.client_key_path = _privateKey;
    _tls.client_cert_path = _client_cert;
    _tls.ca_cert_path = _ca;
    _tls.verify_peer =true;
    _tls.verify_hostname = true;
    tmpOpenOpts.tls_params = _tls;
    std::cout<<_privateKey << _client_cert <<_ca<<_ip<<_tls.verify_peer<<_tls.verify_hostname<<std::endl;
    try
    {
        AmqpClient::Channel::ptr_t connection = AmqpClient::Channel::Open(tmpOpenOpts);
        std::cout << "connection" << std::endl;
        // connection->DeclareQueue("test-rcd", false, true, false, false);
        // connection->BindQueue("test-rcd", "test-rcd", "cts.#");
        // std::string consumer_tag = connection->BasicConsume("test-rcd", "");

        connection->DeclareQueue("MQ.861617051838026", false, true, false, false);
        connection->BindQueue("MQ.861617051838026", "cts_car_issued_exchange", "vehicle.861617051838026.#");
        std::string consumer_tag = connection->BasicConsume("MQ.861617051838026");
        // auto tmp_tag = connection->BasicConsume("test-rcd", "");
        // if(connection->CheckQueueExists("MQ.861617051838026")) {
        //     std::cout << "not exist\n";
        // } else {
        //     std::cout << "exist\n";
        // }
        while (1)
        {
            AmqpClient::Envelope::ptr_t evelope = connection->BasicConsumeMessage();
            std::string exchangesName = evelope->Exchange();
            std::string routingKey = evelope->RoutingKey();
            std::string body = evelope->Message()->Body();
            std::cout << "time:" << evelope->Message()->Timestamp()
                      << " exchangesName:" << exchangesName
                      << " RoutingKey:" << routingKey
                      << " messageid:" << evelope->Message()->MessageId()
                      << " Appid:" << evelope->Message()->AppId()
                      << " Body Size:" << body.size()
                      << " Body:[ ";
            // for (int i = 0; i < body.size(); i++)
            // {
            //     printf("0x%02x ", body.data()[i]);
            // }
            std::cout << " ]" << std::endl;
            // PrintFB(reinterpret_cast<const uint8_t *>(body.data()));
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
#endif

    //    char const *hostname;
    //    amqp_socket_t *socket;
    //    amqp_connection_state_t conn;
    //    int timeout = 1000;
    //    struct timeval tval ;
    //    tval.tv_sec = timeout;
    //    tval.tv_usec = 0;


    //    conn = amqp_new_connection();

    //    socket = amqp_ssl_socket_new(conn);
    //    if (!socket) {
    //        return;
    //    }
    //    int nRet = 0;

    //    //加载机构颁发证书
    //    nRet = amqp_ssl_socket_set_cacert(socket, _ca.c_str());
    //    //校验证书的有效性，设置0，代表不验证证书的合法性，这个时候加载客户端证书和密钥不会生效，仅仅是通过SSL通道加密传输数据，不是明文
    //    amqp_ssl_socket_set_verify_peer(socket, 1);
    //    //因为没有域名，所以不校验
    //    amqp_ssl_socket_set_verify_hostname(socket, 0);
    //    //加载客户端证书和密钥
    //    nRet = amqp_ssl_socket_set_key(socket, client_cert.c_str(), privateKey.c_str());


    //    amqp_socket_open_noblock(socket, _ip.c_str(), atoi(_port.c_str()), &tval);
    //    amqp_rpc_reply_t  replyLogin = amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN,
    //                                              "admin", "123456");
    //    if (AMQP_RESPONSE_NORMAL != replyLogin.reply_type)
    //    {
    //        amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    //        amqp_destroy_connection(conn);
    //        return;
    //    }
}

//void PrintFB(const uint8_t *buf)
//{
//    auto tmpMsg = TGMsg::GetMsgFrame(buf);
//    std::cout << "MsgType:" << tmpMsg->msg_type() << std::endl;
//    auto s = flatbuffers::FlatBufferToString(buf, TGMsg::MsgFrameTypeTable(), false, false);
//    std::cout << s.c_str() << std::endl;
//    switch (tmpMsg->msg_type())
//    {
//    case TGMsg::MsgUnion_Rcd_Status:
//    {
//        auto msgT = tmpMsg->msg_as_Rcd_Status();
//        std::cout
//                << "io:" << static_cast<int>(msgT->io())
//                << " wheel:" << static_cast<int>(msgT->wheel()) << std::endl;
//        break;
//    }
//    case TGMsg::MsgUnion_Rcd_ControlData:
//    {
//        auto msgT = tmpMsg->msg_as_Rcd_ControlData();
//        auto tmpVehicleCmd = msgT->vehicleCmd();
//        auto tmpLightCmd = msgT->lightCmd();
//        if (tmpVehicleCmd)
//        {
//            std::cout << " VehicleCmd steeringWheelAngle:" << tmpVehicleCmd->steeringWheelAngle();
//        }
//        if (tmpLightCmd)
//        {
//            std::cout << " Get LightCmd:";
//        }
//        std::cout << std::endl;
//        break;
//    }
//    case TGMsg::MsgUnion_Vehicle_ReportCan:
//    {
//        auto msgCan = tmpMsg->msg_as_Vehicle_ReportCan()->canRaw();
//        if (!msgCan)
//        {
//            break;
//        }
//        for (auto i : *msgCan)
//        {
//            for (int j = 0; j < i->data()->size(); j++)
//            {
//                printf("0x%02x ", i->data()->Get(j));
//            }
//            printf("\n");
//        }
//        break;
//    }
//    default:
//        break;
//    }
//}
