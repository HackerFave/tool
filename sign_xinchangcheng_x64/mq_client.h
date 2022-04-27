#ifndef MQ_CLIENT_H
#define MQ_CLIENT_H

#include <amqp_ssl_socket.h>
#include <iostream>
#include <time.h>
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <vector>
/**
 * g++ -o rcd_consume rcd_consume.cpp  -lSimpleAmqpClient -lflatbuffers -lboost_chrono -I ../pubinc/fbs/
**/

//#include "tgmsg_generated.h"
//#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <flatbuffers/minireflect.h>
#include <iostream>
#include <string>
#include <thread>
#include <boost/optional.hpp>
using namespace boost;

using namespace std;
class mq_client
{
public:
    mq_client(const std::string &ca,const std::string &client_cert, const std::string & privateKey,const std::string & ip,const std::string & port);
private:
    //    void send_message_to_mq(void);
    //    void get_message_from_mq(void);
private:
    std::string _ca;
    std::string _client_cert;
    std::string _privateKey;
    std::string _ip;
    std::string _port;
    AmqpClient::Channel::OpenOpts tmpOpenOpts;
    AmqpClient::Channel::OpenOpts::TLSParams _tls;
};

#endif // MQ_CLIENT_H
