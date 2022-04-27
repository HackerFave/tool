/**
 * @file rcdmsg.hpp
 * @author lujiefeng
 * @brief 
 * @version 0.1
 * @date 2020-08-24
 * 
 * @copyright Copyright () 2020
 * 
 */
#include <cstdint>
#include "zmq/zmqmsg.hpp"

#pragma once

#pragma pack(1)
typedef struct
{
    uint16_t cmdId;
} CommAck;

typedef struct
{
    uint8_t imei[IMEI_LENGTH];
    uint8_t type;
} GateWayReqAuth;

typedef struct
{
    uint8_t result;
}RcdRespAuth;

typedef struct
{
    uint8_t opt;
}RcdControlCmd;

#pragma pack()
