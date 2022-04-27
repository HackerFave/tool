
#ifndef TOPICCOMMON_H
#define TOPICCOMMON_H

#include <string>
//#include "msg_topic.h"
using namespace std;
namespace topic{
enum Topic_ID{
    UNKNOW = 0,
    TOPIC_TCP_GNSS_GPS_INFO = 1,
    TOPIC_TCP_GNSS_GPS_STATUS = 2,
    TOPIC_IPC_MESSAGE_TEST_TOPIC = 3
};

inline const char* enumToString(Topic_ID operatior) {
    switch(operatior) {
#define interface(name) \
    case name: \
    return #name; \
    break;

    interface(TOPIC_TCP_GNSS_GPS_INFO);
    interface(TOPIC_TCP_GNSS_GPS_STATUS);
    interface(TOPIC_IPC_MESSAGE_TEST_TOPIC);
#undef interface
    default:
        return "UNKNOW";
    }
    return "UNKNOW";
}

inline Topic_ID enumFromString(const std::string & str) {
#define interface(operatior, v) \
    if(str == #v) { \
    return operatior; \
}
    interface(TOPIC_TCP_GNSS_GPS_INFO, tcp/gnss/gps_info);
    interface(TOPIC_TCP_GNSS_GPS_STATUS, tcp/gnss/gps_status);
    interface(TOPIC_IPC_MESSAGE_TEST_TOPIC, ipc/message/test_topic);
    return UNKNOW;
#undef interface
}
}
#endif // TOPICCOMMON_H
