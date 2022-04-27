#ifndef __TGPUB_H__
#define __TGPUB_H__

#include <exception>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/rtps/common/Locator.h>
#include "qos/qos_profile_conf.hpp"

using namespace eprosima::fastdds::dds;
using namespace eprosima::fastrtps::rtps;

template <typename PubMsgType>
class TGPub
{
public:
    TGPub(DomainParticipant *participant, Topic *topic, const QosProfile &qos = QOS_PROFILE_DEFAULT, Publisher *publisher = nullptr)
        : participant_(participant), pubTopic_(topic), publisher_(publisher)
    {
        if (publisher_ == nullptr)
        {
            publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT);
            publisherIsSelf = true;
        }
        DataWriterQos wqos;
        wqos.endpoint().history_memory_policy = PREALLOCATED_WITH_REALLOC_MEMORY_MODE;
        wqos.reliability().kind = qos.reliability;
        wqos.history().kind = qos.history;
        wqos.history().depth = qos.history_depth;
        wqos.durability().kind = qos.durability;
        writer_ = publisher_->create_datawriter(pubTopic_, wqos);
        if (writer_ == nullptr)
        {
            throw std::runtime_error("create_datawriter error");
        }
    }

    ~TGPub()
    {
        if (writer_ != nullptr)
        {
            publisher_->delete_datawriter(writer_);
        }
        if (publisherIsSelf && publisher_ != nullptr)
        {
            participant_->delete_publisher(publisher_);
        }
    }

    PubMsgType *GetZeroCopyInstance()
    {
        if (zeroCopySample == nullptr)
        {
            auto ret = writer_->loan_sample(zeroCopySample);
            if (ret != ReturnCode_t::RETCODE_OK)
            {
                zeroCopySample = nullptr;
            }
        }
        return static_cast<PubMsgType *>(zeroCopySample);
    }

    bool WriteZeroCopyMsg()
    {
        bool ret = writer_->write(zeroCopySample);
        zeroCopySample = nullptr;
        return ret;
    }

    bool Write(const PubMsgType &msg)
    {
        return writer_->write((const_cast<PubMsgType *>(&msg)));
    }

    bool RpcWrite(const PubMsgType &msg, const SampleIdentity &sampleId)
    {
        WriteParams tmpParams;
        tmpParams.related_sample_identity(sampleId);
        return writer_->write((const_cast<PubMsgType *>(&msg)), tmpParams);
    }

    const GUID_t &GetWriterGuid() const
    {
        return writer_->guid();
    }

private:
    DomainParticipant *participant_;
    Publisher *publisher_;
    Topic *pubTopic_;
    DataWriter *writer_;
    bool publisherIsSelf = false;
    void *zeroCopySample = nullptr;
};

#endif
