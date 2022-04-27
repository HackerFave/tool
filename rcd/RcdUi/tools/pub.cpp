#include "../../pubinc/zmq/zmqwrap.hpp"
#include <chrono>
#include <iostream>
#include <thread>

//g++ -o pub pub.cpp -lzmq -lpthread -std=c++17

int main()
{
    ZmqPublisher gwPub(ZMQ_ADDR_Gateway);
    ZmqPublisher controlPub(ZMQ_ADDR_CONTROL);
    while (1)
    {	
	std::cout << ZMQ_TOPIC_VEHICLE_RAWDATA << std::endl;
        gwPub.Publish(ZMQ_TOPIC_VEHICLE_RAWDATA, "012345678954321", 15);
        std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << ZMQ_TOPIC_VEHICLE_STATUS << std::endl;
        gwPub.Publish(ZMQ_TOPIC_VEHICLE_STATUS, "987654321", 10);
        std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << ZMQ_TOPIC_CONTROL_DEVLINKSTATUS << std::endl;
        controlPub.Publish(ZMQ_TOPIC_CONTROL_DEVLINKSTATUS, "asdf", 4);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}
