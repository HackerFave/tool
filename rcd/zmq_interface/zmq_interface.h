#ifndef ZMQ_INTERFACE_H
#define ZMQ_INTERFACE_H

#include "zmq_interface_global.h"
#include "define_zmqmodule.h"
extern "C"
{
ZMQ_INTERFACE_EXPORT dataInterface * initInstance();
ZMQ_INTERFACE_EXPORT void setPostData(const void *payload, size_t payload_len, int id);
}


#endif // ZMQ_INTERFACE_H
