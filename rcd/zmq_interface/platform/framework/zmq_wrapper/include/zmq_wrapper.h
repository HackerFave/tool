#pragma once

#include <map>
#include <thread>
#include <vector>

#include "zmq_addon.hpp"

namespace datax {

using SubscribeFunc = std::function<void(const char*, const void*, uint32_t)>;
using MessageFunc = std::function<void(const void*, uint32_t)>;

class ZmqProxy {
public:
    static ZmqProxy &GetInstance(void);

private:
    ZmqProxy(void);
    ~ZmqProxy(void);
    ZmqProxy(const ZmqProxy &proxy);
    const ZmqProxy &operator=(const ZmqProxy &proxy);
};

class ZmqWrapper {
public:
    ZmqWrapper(std::string, std::string);
    ~ZmqWrapper(void);

    void Subscribe(std::string topic, SubscribeFunc func);
    void UnSubscribe(const std::string topic);

    void SubscribeAll(SubscribeFunc func);
    void UnSubscribeAll(void);

    void RegisterReceiver(std::string topic, MessageFunc func);
    //UnRegisterReceiver();

    void SubscribeWithMessage(std::string topic, SubscribeFunc func);
    void UnSubscribeWithMessage(const std::string topic);

    size_t Post(std::string topic, const void *payload, size_t payload_len);
    size_t PostWithMessage(std::string topic, const void *payload, size_t payload_len);

    void run(void);

private:
    void MessageHandler(void);

    ZmqProxy &proxy = ZmqProxy::GetInstance();

    std::map<std::string, MessageFunc> msgback_;
    std::map<std::string, SubscribeFunc> subback_;
    std::map<std::string, SubscribeFunc> sub_msg_back_;
    SubscribeFunc allback_ = nullptr;
    zmq::socket_t m_suber;
    zmq::socket_t m_puber;
    std::thread thread_;
    zmq::context_t zmq_context_;
    int m_exit_flag = 0;
};

}  // namespace datax