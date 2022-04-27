/**
 * @file msgcmd.hpp
 * @author lujiefeng
 * @brief 
 * @version 0.1
 * @date 2020-08-24
 * 
 * @copyright Copyright () 2020
 * 
 */

#pragma once

//网关上报命令
#define GATEWAY_COM_ACK 0x0100
#define GATEWAY_REQ_AUTH 0x0101
#define GATEWAY_REPORT_VEHICLE_DATA 0x0102
#define GATEWAY_RESP_CONTROL_CMD 0x0103
#define GATEWAY_REPORT_HMI_MSG 0x0104

//驾驶舱下发命令
#define RCD_COM_ACK 0x0200
#define RCD_RESP_AUTH 0x0201
#define RCD_CONTROL_CMD 0x0202
#define RCD_CONTROL_DATA 0x0203
#define RCD_DELAY_CMD 0x0204
#define RCD_HMIMSG_CMD 0x0205
#define RCD_CONTROL_LIGHT 0x0206
