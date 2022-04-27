
#include "../../pubinc/zmq/zmqwrap.hpp"
#include <chrono>
#include <iostream>
#include <thread>

void ReqTopicHandler(const std::string &topic, const void *data, int len)
{
    std::string outData(reinterpret_cast<const char *>(data), len);
    std::cout << "Topic:" << topic << " len:" << len << " Msg:" << outData << std::endl;
}

//g++ -o sub sub.cpp -lzmq -lpthread -std=c++17

int main()
{
    ZmqSubscriber test1;
    test1.ConnectTo(ZMQ_ADDR_UI);
    test1.Subscribe(ZMQ_TOPIC_REQ_REMOTE_CONTROL, ReqTopicHandler);
    int count = 0;
    while (1)
    {
        count++;
        std::cout << "Count:" << count << std::endl;
        if (count == 10)
        {
            test1.ConnectTo(ZMQ_ADDR_CONTROL);
            test1.Subscribe(ZMQ_TOPIC_CONTROL_DEVLINKSTATUS, ReqTopicHandler);
            test1.Subscribe(ZMQ_TOPIC_RESP_REMOTE_CONTROL, ReqTopicHandler);
        }
        else if (count == 15)
        {
            test1.UnSubscribeAll();
        }
        else if (count == 20)
        {
            test1.UnSubscribe(ZMQ_TOPIC_REQ_REMOTE_CONTROL);
        }
        else if (count == 25)
        {
            test1.UnSubscribeAll();
        }
        else if (count == 30)
        {
            test1.SubscribeAll(ReqTopicHandler);
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}
