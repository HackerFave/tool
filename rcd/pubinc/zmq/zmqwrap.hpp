/**
 * @file zmqwrap.hpp
 * @author lujiefeng
 * @brief compile whit -lzmq -lpthread -std=c++17
 * @version 0.1
 * @date 2020-08-20
 * 
 * @copyright Copyright () 2020
 * 
 */

#pragma once

#include "zmqtopic.hpp"
#include <functional>
#include <map>
#include <mutex>
#include <thread>
#include <vector>
#include <zmq.hpp>
#include <zmq_addon.hpp>

#define ZMQ_ADDR_UI "ipc://@UIPub"
#define ZMQ_ADDR_CONTROL "ipc://@ControlPub"
#define ZMQ_ADDR_Gateway "ipc://@GatewayPub"

#define DISALLOW_COPY_AND_ASSIGN(classname) \
    classname(const classname &) = delete;  \
    classname &operator=(const classname &) = delete;

class ZmqContext
{
public:
    static ZmqContext *Instance()
    {
        static ZmqContext *ptr = nullptr;
        if (ptr == nullptr)
        {
            static std::once_flag flag;
            std::call_once(flag, [&]() { ptr = new ZmqContext(); });
        }
        return ptr;
    }
    zmq::context_t &GetContext()
    {
        return ctx;
    }
    ~ZmqContext() {}

private:
    zmq::context_t ctx;
    ZmqContext() {}
    DISALLOW_COPY_AND_ASSIGN(ZmqContext)
};

#define ZmqContextRef() ZmqContext::Instance()->GetContext()

class ZmqPublisher
{
public:
    ZmqPublisher(const std::string &addr)
    {
        pubSocket = zmq::socket_t(ZmqContextRef(), zmq::socket_type::pub);
        pubSocket.bind(addr);
    }
    ~ZmqPublisher() {}

    int Publish(const std::string &topic, const void *data, int len)
    {
        std::vector<zmq::const_buffer> pubMsg;
        pubMsg.push_back(zmq::buffer(topic));
        pubMsg.push_back(zmq::const_buffer(data, len));
        auto result = zmq::send_multipart(pubSocket, pubMsg);
        return result.has_value() ? result.value() : 0;
    }

private:
    zmq::socket_t pubSocket;
};

/*
* Topic handle CallBack Func . (std::string &topic,const void * data,int dataLen)
*/
using TopicCbFunc = std::function<void(std::string &, const void *, int)>;

class ZmqSubscriber
{
public:
    ZmqSubscriber()
    {
        subSocket = zmq::socket_t(ZmqContextRef(), zmq::socket_type::sub);
        recvMsgThread = std::thread(&ZmqSubscriber::RecvZmqMessage, this);
    }
    ~ZmqSubscriber() {}

    /*
    * Connect To Publisher. called before Subscribe topic
    * you can connect to multiple publishers
    */
    void ConnectTo(const std::string &addr)
    {
        subSocket.connect(addr);
    }

    void SubscribeAll(TopicCbFunc func)
    {
        subSocket.set(zmq::sockopt::subscribe, "");
        allTopicCbFunc = func;
    }

    void Subscribe(const std::string &topic, TopicCbFunc func)
    {
        subSocket.set(zmq::sockopt::subscribe, topic);
        cbFuncMap.insert_or_assign(topic, std::move(func));
    }

    void UnSubscribeAll()
    {
        for (auto &it : cbFuncMap)
        {
            subSocket.set(zmq::sockopt::unsubscribe, it.first);
        }
        allTopicCbFunc = nullptr;
    }

    void UnSubscribe(const std::string &topic)
    {
        subSocket.set(zmq::sockopt::unsubscribe, topic);
    }

private:
    void RecvZmqMessage()
    {
        while (true)
        {
            try
            {
                std::vector<zmq::message_t> recvMsg;
                auto ret = zmq::recv_multipart(subSocket, std::back_inserter(recvMsg));
                if (ret.has_value() && recvMsg.size() == 2)
                {
                    std::string topic = recvMsg.at(0).to_string();
                    TopicCbFunc tmpFunc = allTopicCbFunc != nullptr ? allTopicCbFunc : cbFuncMap.at(topic);
                    tmpFunc(topic, recvMsg.at(1).data(), recvMsg.at(1).size());
                }
            }
            catch(std::exception &e)
            {

            }
        }
    }

private:
    zmq::socket_t subSocket;
    TopicCbFunc allTopicCbFunc;
    std::map<std::string, TopicCbFunc> cbFuncMap;
    std::thread recvMsgThread;
};
