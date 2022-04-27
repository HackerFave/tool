#pragma once

#include <fastdds/dds/core/policy/QosPolicies.hpp>
#include <fastdds/rtps/resources/ResourceManagement.h>

using namespace eprosima::fastdds::dds;

struct QosProfile
{
    // LivelinessQosPolicyKind liveliness = AUTOMATIC_LIVELINESS_QOS;
    ReliabilityQosPolicyKind reliability = RELIABLE_RELIABILITY_QOS;
    HistoryQosPolicyKind history = KEEP_LAST_HISTORY_QOS;
    int history_depth = 1;
    DurabilityQosPolicyKind durability = VOLATILE_DURABILITY_QOS;
};

class QosProfileConf
{
public:
    QosProfileConf();
    virtual ~QosProfileConf();

    static QosProfile CreateQosProfile(const ReliabilityQosPolicyKind &reliability,
                                       const HistoryQosPolicyKind &history,
                                       const int history_depth,
                                       const DurabilityQosPolicyKind &durability)
    {
        QosProfile qos_profile;
        qos_profile.reliability = reliability;
        qos_profile.history = history;
        qos_profile.history_depth = history_depth;
        qos_profile.durability = durability;
        return qos_profile;
    }
};

#define QOS_PROFILE_DEFAULT     QosProfileConf::CreateQosProfile(BEST_EFFORT_RELIABILITY_QOS, KEEP_LAST_HISTORY_QOS, 5,    VOLATILE_DURABILITY_QOS)
#define QOS_PROFILE_EVENT       QosProfileConf::CreateQosProfile(RELIABLE_RELIABILITY_QOS,    KEEP_LAST_HISTORY_QOS, 5,    VOLATILE_DURABILITY_QOS)
#define QOS_PROFILE_EVENT_LOCAL QosProfileConf::CreateQosProfile(RELIABLE_RELIABILITY_QOS,    KEEP_LAST_HISTORY_QOS, 5,    TRANSIENT_LOCAL_DURABILITY_QOS)
#define QOS_PROFILE_PERIOD      QosProfileConf::CreateQosProfile(BEST_EFFORT_RELIABILITY_QOS, KEEP_LAST_HISTORY_QOS, 50,   VOLATILE_DURABILITY_QOS)
#define QOS_PROFILE_PERIOD_BIG  QosProfileConf::CreateQosProfile(BEST_EFFORT_RELIABILITY_QOS, KEEP_LAST_HISTORY_QOS, 1000, VOLATILE_DURABILITY_QOS)

// align ros2
#define QOS_PROFILE_ROSMESSAGE  QosProfileConf::CreateQosProfile(RELIABLE_RELIABILITY_QOS,    KEEP_LAST_HISTORY_QOS, 1,    VOLATILE_DURABILITY_QOS)
#define QOS_PROFILE_DISCOVERY   QosProfileConf::CreateQosProfile(RELIABLE_RELIABILITY_QOS,    KEEP_LAST_HISTORY_QOS, 1,    TRANSIENT_LOCAL_DURABILITY_QOS)