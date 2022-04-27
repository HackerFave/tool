#ifndef __TGRPC_H__
#define __TGRPC_H__

#include "qos/qos_profile_conf.hpp"
#include "tgpub.hpp"
#include "tgsub.hpp"

#include <exception>
#include <unordered_map>
#include <future>

using namespace eprosima::fastdds::dds;
using namespace eprosima::fastrtps::rtps;

template <typename Request, typename Response>
class TGService
{
public:
    using SharedRequest = typename std::shared_ptr<Request>;
    using SharedResponse = typename std::shared_ptr<Response>;
    using TGServiceCallback = std::function<void(const Request &,
                                                 Response &)>;

public:
    explicit TGService(DomainParticipant *participant, Topic *requestTopic, Topic *responseTopic, const TGServiceCallback &service_callback)
        : participant_(participant), requestTopic_(requestTopic), responseTopic_(responseTopic),
          serviceCB_(service_callback)
    {
        puber = std::make_shared<TGPub<Response>>(participant_, responseTopic_, QOS_PROFILE_EVENT_LOCAL);
        suber = std::make_shared<TGSub<Request>>(participant_, requestTopic_, QOS_PROFILE_EVENT_LOCAL);
        suber->SetRPCCallBack([&](const Request &requestMsg, const SampleIdentity &requestId)
                              {
            if(serviceCB_ != nullptr)
            {
                Response responseMsg;
                serviceCB_(requestMsg,responseMsg);
                puber->RpcWrite(responseMsg,requestId);
            } });
    }
    ~TGService()
    {
    }

private:
    DomainParticipant *participant_;
    Topic *requestTopic_;
    Topic *responseTopic_;
    std::shared_ptr<TGPub<Response>> puber;
    std::shared_ptr<TGSub<Request>> suber;
    SampleIdentity sampleId;
    TGServiceCallback serviceCB_;
};

template <typename Request, typename Response>
class TGClient
{
public:
    using SharedRequest = typename std::shared_ptr<Request>;
    using SharedResponse = typename std::shared_ptr<Response>;
    using Promise = std::promise<SharedResponse>;
    using SharedPromise = std::shared_ptr<Promise>;
    using SharedFuture = std::shared_future<SharedResponse>;

public:
    explicit TGClient(DomainParticipant *participant, Topic *requestTopic, Topic *responseTopic)
        : participant_(participant), requestTopic_(requestTopic), responseTopic_(responseTopic)
    {
        puber = std::make_shared<TGPub<Request>>(participant_, requestTopic_, QOS_PROFILE_EVENT_LOCAL);
        writerGuid = puber->GetWriterGuid();
        suber = std::make_shared<TGSub<Response>>(participant_, responseTopic_, QOS_PROFILE_EVENT_LOCAL);
        suber->SetRPCCallBack([&](const Response &responseMsg, const SampleIdentity &sampleId)
                              { 
                                     if(sampleId.writer_guid() != writerGuid)
                                     {
                                        return;
                                     }
                                     uint64_t sequence_number = sampleId.sequence_number().to64long();
                                     if(callPendings.count(sequence_number) == 0)
                                     {
                                         return;
                                     } 
                                     auto promise = callPendings[sequence_number];
                                     callPendings.erase(sequence_number);
                                     promise->set_value(std::make_shared<Response>(std::move(responseMsg))); });
    }
    ~TGClient() {}
    std::shared_ptr<Response> Call(const Request &request, int timeOutSeconds = 5)
    {
        SampleIdentity tmp;
        auto curT = std::chrono::steady_clock::now().time_since_epoch();
        SequenceNumber_t seqNum = SequenceNumber_t(std::chrono::duration_cast<std::chrono::milliseconds>(curT).count());
        tmp.writer_guid(writerGuid);
        tmp.sequence_number(seqNum);
        puber->RpcWrite(request, tmp);
        SharedPromise callPromise = std::make_shared<Promise>();
        callPendings[seqNum.to64long()] = callPromise;
        SharedFuture responseFuture(callPromise->get_future());
        auto status = responseFuture.wait_for(std::chrono::seconds(timeOutSeconds));
        if (status == std::future_status::ready)
        {
            return responseFuture.get();
        }
        return nullptr;
    }

private:
    DomainParticipant *participant_;
    Topic *requestTopic_;
    Topic *responseTopic_;
    std::shared_ptr<TGPub<Request>> puber;
    std::shared_ptr<TGSub<Response>> suber;
    GUID_t writerGuid;
    std::unordered_map<uint64_t, SharedPromise> callPendings;
};
#endif