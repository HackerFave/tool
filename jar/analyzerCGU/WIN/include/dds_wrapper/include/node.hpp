#pragma once
#include "node_impl.hpp"

using namespace eprosima::fastdds::dds;

template <typename T>
struct HasType
{
    template <typename Class>
    static constexpr bool Test(decltype(&Class::type) *) { return true; }
    template <typename>
    static constexpr bool Test(...) { return false; }
    static constexpr bool value = Test<T>(nullptr);
};
template <typename T>
constexpr bool HasType<T>::value;

class Node
{

public:
    explicit Node(std::string name) : m_name(name)
    {
        m_node_impl.reset(new NodeImpl(m_name));
    };

    ~Node(void){};

    // void Getguid(std::array<uint8_t, 24>& guid_byte_array)
    // {
    //     return m_node_impl->Getguid(guid_byte_array);
    // }

    template <typename MsgPubSubType>
    auto CreateReader(std::string topic, const QosProfile &qos = QOS_PROFILE_DEFAULT)
        -> std::shared_ptr<TGSub<typename MsgPubSubType::type>>
    {
        return m_node_impl->template CreateReader<MsgPubSubType>(topic, qos);
    }

    template <typename MsgPubSubType>
    auto CreateWriter(std::string topic, const QosProfile &qos = QOS_PROFILE_DEFAULT)
        -> std::shared_ptr<TGPub<typename MsgPubSubType::type>>
    {
        return m_node_impl->template CreateWriter<MsgPubSubType>(topic, qos);
    }

    template <typename RequestPubSubType,
              typename ResponsePubSubType>
    auto CreateService(const std::string &serviceName,
                       const typename TGService<typename RequestPubSubType::type, typename ResponsePubSubType::type>::TGServiceCallback &func)
        -> std::shared_ptr<TGService<typename RequestPubSubType::type,
                                     typename ResponsePubSubType::type>>
    {
        return m_node_impl->template CreateService<RequestPubSubType, ResponsePubSubType>(serviceName, func);
    }

    template <typename RequestPubSubType,
              typename ResponsePubSubType>
    auto CreateClient(const std::string &serviceName)
        -> std::shared_ptr<TGClient<typename RequestPubSubType::type,
                                    typename ResponsePubSubType::type>>
    {
        return m_node_impl->template CreateClient<RequestPubSubType, ResponsePubSubType>(serviceName);
    }

private:
    std::string m_name;
    std::unique_ptr<NodeImpl> m_node_impl = nullptr;
};
