#pragma once
#include "zmq_wrapper.h"
#include <mutex>

namespace datax {

class Node {
public:
    Node(void);
    ~Node(void);

    void Subscribe(std::string topic, SubscribeFunc func);
    void UnSubscribe(std::string topic);
    void SubscribeAll(SubscribeFunc func);
    void UnSubscribeAll(void);
    void SubscribeWithMessage(std::string topic, SubscribeFunc func);
    void UnSubscribeWithMessage(std::string topic);

    size_t Post(std::string topic, const void *payload, size_t payload_len);
    size_t PostWithMessage(std::string topic, const void *payload, size_t payload_len);

    std::shared_ptr<ZmqWrapper> GetIpcPool(void);
    std::shared_ptr<ZmqWrapper> GetInprocPool(void);
    std::shared_ptr<ZmqWrapper> GetTcpPool(void);

private:
    void CheckStartServer(std::shared_ptr<ZmqWrapper> pool, int &flag);
    void CheckStopServer(std::shared_ptr<ZmqWrapper> pool, int &flag);
    std::shared_ptr<ZmqWrapper> GetWrapper(std::string topic);

    std::mutex mutex_;
    int m_ipc_flag = 0;
    int m_inproc_flag = 0;
    int m_tcp_flag = 0;
    std::shared_ptr<ZmqWrapper> m_ipc;
    std::shared_ptr<ZmqWrapper> m_inproc;
    std::shared_ptr<ZmqWrapper> m_tcp;

};

}  // namespace datax
