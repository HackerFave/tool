#include "zmq_wrapper.h"
#include "datax_global.h"
#include "message_generated.h"

#define _ALIGNMENT_ 8
#define ZMQ_PROXY_PUB_CHANNEL   "inproc://pub_channel"
#define ZMQ_PROXY_SUB_CHANNEL   "inproc://sub_channel"


namespace datax {

ZmqProxy::ZmqProxy(void)
{
    std::thread io_run_thread([&]() {
      zmq::socket_t frontend(datax::DataxGlobal::Instance()->ZmqContext(), /*ZMQ_ROUTER*/ ZMQ_XSUB);
      zmq::socket_t backend(datax::DataxGlobal::Instance()->ZmqContext(), /*ZMQ_DEALER*/ ZMQ_XPUB);
      frontend.bind(ZMQ_PROXY_PUB_CHANNEL);
      backend.bind(ZMQ_PROXY_SUB_CHANNEL);
      zmq::proxy(frontend, backend);
    });
    io_run_thread.detach();
}

ZmqProxy::~ZmqProxy(void) {}

ZmqProxy &ZmqProxy::GetInstance(void)
{
    static ZmqProxy proxy;
    return proxy;
}


ZmqWrapper::ZmqWrapper(std::string sub_channel, std::string pub_channel)
    : m_suber(DataxGlobal::Instance()->ZmqContext(), zmq::socket_type::sub)
    , m_puber(DataxGlobal::Instance()->ZmqContext(), zmq::socket_type::pub) {
      // throw expection
    m_suber.connect(sub_channel);
    m_puber.connect(pub_channel);
}

ZmqWrapper::~ZmqWrapper() {
    m_exit_flag = 1;
    if(thread_.joinable()){
        thread_.join();
    }
}

// start server
void ZmqWrapper::run(void) {
  thread_ = std::thread(&ZmqWrapper::MessageHandler, this);
}

void ZmqWrapper::MessageHandler(void) {
    std::string topic;

    while(1)
    {
      if(m_exit_flag == 1) {
          break;
      }
      std::vector<zmq::message_t> recv_msgs;
      const auto ret = zmq::recv_multipart(
          m_suber, std::back_inserter(recv_msgs), zmq::recv_flags::dontwait);

      if (ret.has_value()) {
        std::string topic = recv_msgs[0].to_string();
        if(msgback_.find(topic) != msgback_.end()) {
          // callback process
          for(auto it=recv_msgs.begin()+1; it!=recv_msgs.end(); ++it) {
            if (msgback_[topic] != nullptr) {
              auto temp_buf = aligned_sptr<MessageT>(
                  _ALIGNMENT_, _ALIGNMENT_ * ((*it).size() / _ALIGNMENT_ + 1));
              std::memmove(temp_buf.get(), (*it).data(), (*it).size());
              msgback_[topic](reinterpret_cast<const void*>(temp_buf.get()),
                               (*it).size());
            }
          }
        }
        if(subback_.find(topic) != subback_.end()) {
          // callback process
          for(auto it=recv_msgs.begin()+1; it!=recv_msgs.end(); ++it) {
            if (subback_[topic] != nullptr) {
              auto temp_buf = aligned_sptr<MessageT>(
                  _ALIGNMENT_, _ALIGNMENT_ * ((*it).size() / _ALIGNMENT_ + 1));
              std::memmove(temp_buf.get(), (*it).data(), (*it).size());
              subback_[topic](topic.c_str(),
                              reinterpret_cast<const void*>(temp_buf.get()),
                              (*it).size());
            }
          }
        }
        if(sub_msg_back_.find(topic) != sub_msg_back_.end()) {
          // callback process
          for(auto it=recv_msgs.begin()+1; it!=recv_msgs.end(); ++it) {
            if (sub_msg_back_[topic] != nullptr) {
              auto temp_buf = aligned_sptr<MessageT>(
                  _ALIGNMENT_, _ALIGNMENT_ * ((*it).size() / _ALIGNMENT_ + 1));
              std::memmove(temp_buf.get(), (*it).data(), (*it).size());
              flatbuffers::Verifier verifier(reinterpret_cast<unsigned char*>(temp_buf.get()),(*it).size());
              if(!VerifyMessageBuffer(verifier)) {
                  continue;
              }
              auto msg = GetMessage(temp_buf.get());
              sub_msg_back_[topic](topic.c_str(),
                              reinterpret_cast<const void*>(msg->data()->Data()),
                              (*it).size());
            }
          }
        }
        if(allback_ != nullptr) {
          // callback process
          for(auto it=recv_msgs.begin()+1; it!=recv_msgs.end(); ++it) {
            auto temp_buf = aligned_sptr<MessageT>(
                _ALIGNMENT_, _ALIGNMENT_ * ((*it).size() / _ALIGNMENT_ + 1));
            std::memmove(temp_buf.get(), (*it).data(), (*it).size());
            allback_(topic.c_str(),
                     reinterpret_cast<const void*>(temp_buf.get()),
                     (*it).size());
          }
        }
      }
      //test if we need a little sleep
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void ZmqWrapper::Subscribe(std::string topic, SubscribeFunc func) {
    subback_.insert(std::make_pair(topic, func));
    m_suber.set(zmq::sockopt::subscribe, topic);
}

void ZmqWrapper::UnSubscribe(const std::string topic) {
    m_suber.set(zmq::sockopt::unsubscribe, topic);
    subback_.erase(topic);
}

void ZmqWrapper::SubscribeAll(SubscribeFunc func) {
    allback_ = func;
    m_suber.set(zmq::sockopt::subscribe, "");
}

void ZmqWrapper::UnSubscribeAll(void) {
    m_suber.set(zmq::sockopt::unsubscribe, "");
    allback_ = nullptr;
}

void ZmqWrapper::RegisterReceiver(std::string topic, MessageFunc func) {
    msgback_.insert(std::make_pair(topic, func));
    m_suber.set(zmq::sockopt::subscribe, topic);
}

void ZmqWrapper::SubscribeWithMessage(std::string topic, SubscribeFunc func) {
    sub_msg_back_.insert(std::make_pair(topic, func));
    m_suber.set(zmq::sockopt::subscribe, topic);
}

void ZmqWrapper::UnSubscribeWithMessage(const std::string topic) {
    m_suber.set(zmq::sockopt::unsubscribe, topic);
    sub_msg_back_.erase(topic);
}

size_t ZmqWrapper::Post(std::string topic, const void *payload, size_t payload_len) {
    zmq::const_buffer cbuf(payload, static_cast<size_t>(payload_len));
    m_puber.send(zmq::buffer(topic), zmq::send_flags::sndmore);
    return m_puber.send(cbuf, zmq::send_flags::dontwait).value();
}

size_t ZmqWrapper::PostWithMessage(std::string topic, const void *payload, size_t payload_len) {
    MessageT msg;
    msg.timestamp = 12345;
    msg.node_name = topic;
    msg.data.assign((u_int8_t*)payload, (u_int8_t*)payload+payload_len);
    msg.size = payload_len;

    flatbuffers::FlatBufferBuilder builder;
    builder.Finish(CreateMessage(builder, &msg));
    return Post(topic, builder.GetBufferPointer(), builder.GetSize());
}

}  // namespace datax
