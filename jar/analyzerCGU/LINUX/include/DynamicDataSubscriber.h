/**
 * @file DynamicDataSubscriber.h
 * @brief
 * @author LuBow 
 * @date 2022/2/15
 */
#ifndef DYNAMICDATASUBSCRIBER_DynamicDataSubscriber_H
#define DYNAMICDATASUBSCRIBER_DynamicDataSubscriber_H

#include "HandleDynamicData.hpp"

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantListener.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastrtps/subscriber/SampleInfo.h>
#include <fastrtps/rtps/common/Types.h>

#include <fastrtps/types/TypeIdentifier.h>
#include <fastrtps/types/TypeObject.h>

#include <fastrtps/attributes/SubscriberAttributes.h>

#include <map>
#include <string>
#include <mutex>

class DynamicDataSubscriber
{
public:
    using JsonStringCallBack = std::function<void(const std::string &)>;
    DynamicDataSubscriber();

    virtual ~DynamicDataSubscriber();

    bool init();

    void run();

    void getJsonString(std::string& json_string);
    void RegisterCallBack(const JsonStringCallBack &func);

private:

    eprosima::fastdds::dds::DomainParticipant* mp_participant;

    eprosima::fastdds::dds::Subscriber* mp_subscriber;

    std::map<eprosima::fastdds::dds::DataReader*, eprosima::fastdds::dds::Topic*> topics_;

    std::map<eprosima::fastdds::dds::DataReader*, eprosima::fastrtps::types::DynamicType_ptr> readers_;

    std::map<eprosima::fastdds::dds::DataReader*, eprosima::fastrtps::types::DynamicData_ptr> datas_;

    eprosima::fastrtps::SubscriberAttributes att_;

    eprosima::fastdds::dds::DataReaderQos qos_;
    static eprosima::fastrtps::types::HandleDynamicData  dynamic_data_;
    static std::mutex dynamic_mutex_;
    static JsonStringCallBack func_;

public:

    class SubListener
            :  public eprosima::fastdds::dds::DomainParticipantListener
    {
    public:

        SubListener(
                DynamicDataSubscriber* sub)
                : n_matched(0)
                , n_samples(0)
                , subscriber_(sub)
        {
        }

        ~SubListener() override
        {
        }

        void on_data_available(
                eprosima::fastdds::dds::DataReader* reader) override;

        void on_subscription_matched(
                eprosima::fastdds::dds::DataReader* reader,
                const eprosima::fastdds::dds::SubscriptionMatchedStatus& info) override;

        void on_type_information_received(
                eprosima::fastdds::dds::DomainParticipant* participant,
                const eprosima::fastrtps::string_255 topic_name,
                const eprosima::fastrtps::string_255 type_name,
                const eprosima::fastrtps::types::TypeInformation& type_information) override;

        int n_matched;

        uint32_t n_samples;

        DynamicDataSubscriber* subscriber_;

    }
    m_listener;

};

#endif //DYNAMICDATASUBSCRIBER_DynamicDataSubscriber_H
