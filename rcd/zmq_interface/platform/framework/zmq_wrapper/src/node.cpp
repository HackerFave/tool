#include "node.h"

namespace datax {

Node::Node(void) {
    m_ipc = std::make_shared<datax::ZmqWrapper>("ipc://@message_pool/sub_channel", "ipc://@message_pool/pub_channel");
    m_inproc = std::make_shared<datax::ZmqWrapper>("inproc://sub_channel", "inproc://pub_channel");
    m_tcp = std::make_shared<datax::ZmqWrapper>("tcp://192.168.20.20:7000", "tcp://192.168.20.20:7001");
}

Node::~Node(void) {

}

void Node::CheckStartServer(std::shared_ptr<ZmqWrapper> pool, int &flag) {
    std::unique_lock<std::mutex> lock(mutex_);
    if(!flag) {
        pool->run();
        flag = 1;
    }
}

void Node::CheckStopServer(std::shared_ptr<ZmqWrapper> pool, int &flag) {
    std::unique_lock<std::mutex> lock(mutex_);
    if(flag) {
        pool->run();
        flag = 0;
    }
}

std::shared_ptr<ZmqWrapper> Node::GetWrapper(std::string topic) {
    std::shared_ptr<ZmqWrapper> pool = nullptr;
    if(topic.compare(0, 3, "ipc") == 0) {
        pool = m_ipc;
        CheckStartServer(pool, m_ipc_flag);
    } else if(topic.compare(0, 6, "inproc") == 0) {
        pool = m_inproc;
        CheckStartServer(pool, m_inproc_flag);
    } else {// if(topic.compare(0, 3, "tcp") == 0)
        pool = m_tcp;
        CheckStartServer(pool, m_tcp_flag);
    }
    return pool;
}

void Node::Subscribe(std::string topic, SubscribeFunc func) {
    std::shared_ptr<ZmqWrapper> pool = GetWrapper(topic);
    pool->Subscribe(topic, func);
}

void Node::UnSubscribe(std::string topic) {
    std::shared_ptr<ZmqWrapper> pool = GetWrapper(topic);
    pool->UnSubscribe(topic);
}

void Node::SubscribeAll(SubscribeFunc func) {
    m_ipc->SubscribeAll(func);
    CheckStartServer(m_ipc, m_ipc_flag);
    m_inproc->SubscribeAll(func);
    CheckStartServer(m_inproc, m_inproc_flag);
    m_tcp->SubscribeAll(func);
    CheckStartServer(m_tcp, m_tcp_flag);
}

void Node::UnSubscribeAll(void) {
    m_ipc->UnSubscribeAll();
    m_inproc->UnSubscribeAll();
    m_tcp->UnSubscribeAll();
}

void Node::SubscribeWithMessage(std::string topic, SubscribeFunc func) {
    std::shared_ptr<ZmqWrapper> pool = GetWrapper(topic);
    pool->SubscribeWithMessage(topic, func);
}

void Node::UnSubscribeWithMessage(std::string topic) {
    std::shared_ptr<ZmqWrapper> pool = GetWrapper(topic);
    pool->UnSubscribeWithMessage(topic);
}

size_t Node::Post(std::string topic, const void *payload, size_t payload_len) {
    std::shared_ptr<ZmqWrapper> pool = GetWrapper(topic);
    return pool->Post(topic, payload, payload_len);
}

size_t Node::PostWithMessage(std::string topic, const void *payload, size_t payload_len) {
    std::shared_ptr<ZmqWrapper> pool = GetWrapper(topic);
    return pool->PostWithMessage(topic, payload, payload_len);
}

std::shared_ptr<ZmqWrapper> Node::GetIpcPool(void) {
    CheckStartServer(m_ipc, m_ipc_flag);
    return m_ipc;
}

std::shared_ptr<ZmqWrapper> Node::GetInprocPool(void) {
    CheckStartServer(m_inproc, m_inproc_flag);
    return m_inproc;
}

std::shared_ptr<ZmqWrapper> Node::GetTcpPool(void) {
    CheckStartServer(m_tcp, m_tcp_flag);
    return m_tcp;
}

}  // namespace datax
