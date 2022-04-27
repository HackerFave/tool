
#ifndef TOPICCOMMON_H
#define TOPICCOMMON_H

#include <string>
#include "msg_topic.h"

using namespace std;
namespace topic{
enum Topic_ID{
    UNKNOW = 0,
    // gps_info.fbs
    TOPIC_TCP_GNSS_GPS_INFO_ENUM = 1,
    TOPIC_TCP_GNSS_GPS_STATUS_ENUM = 2,
    //test
    TOPIC_IPC_MESSAGE_TEST_TOPIC_ENUM = 5,

    //gnssServer
    TOPIC_HUAWEI_GNSS_BROADCAST_DATA_ENUM = 8,   /* "ipc/gnss/HuaWeiGnssBroadcastData"*///MsgVector
    TOPIC_GNSS_BROADCAST_DATA_ENUM =9,         /* "tcp/gnss/GnssBroadcastData"/*///MsgGnssBroadcastData

    //voiceServer
    TOPIC_HMI_SEND_VOICE_PLAY_ENUM =12,         /* "tcp/voice/HmiSendVoicePlay"//MsgVector*/

    // v2xServer
    TOPIC_CV2X_COll_WARNING_ENUM=15,            /* "tcp/v2x/msg_8401"*/
    TOPIC_CV2X_SEND_GNSS_RTK_DATA_ENUM=16 ,     /* "tcp/v2x/send_gnss_rkt_data"*/
    TOPIC_CV2X_RECV_RSU_CLOUD_DATA_ENUM=17,     /*"tcp/v2x/recv_rsu_cloud_data"*/
    TOPIC_CV2X_AUX_RESP_JOB_SCJED_DATA_ENUM=18,  /*"tcp/v2x/aux_resp_job_sched_data"*/
    TOPIC_CV2X_TRUCK_REQ_JOB_SCHED_DATA_ENUM =19,/*"tcp/v2x/truck_req_job_sched_data"*/

    // can_info.fbs
    TOPIC_CAN_WIRE_CONTROL_FAULT_ENUM =22,       /*"tcp/can/wire_control_fault" */       //WireControlVehicleFault
    TOPIC_CAN_WIRE_CONTROL_MOVATION_ENUM =23,    /*"tcp/can/wire_control_movation" */    //WireControlVehicleMovation
    TOPIC_CAN_WIRE_CONTROL_STATUS_ENUM  =24,    /* "tcp/can/wire_control_status" */      //WireControlVehicleStatus
    TOPIC_CAN_WIRE_CONTROL_VCU_VER_ENUM =25 ,    /*"tcp/can/wire_control_vcu_ver"   */   //WireControlVcuVersion
    TOPIC_CAN_DEV_STATUS_ENUM   =26 ,           /* "tcp/can/can_about_dev_status"   */   //CanAboutDevStatus

    // control.fbs
    TOPIC_CONTROL_IMPLEMENT_ENUM =29,           /* "tcp/control/control_implement" */    //ControlOutputForImplement
    TOPIC_CONTROL_DECISION_ENUM  =30,           /* "tcp/control/control_decision" */     //ControlOutputForDecision

    //devision.fbs
    TOPIC_DECISION_OUTPUT_ENUM  =33 ,           /* "tcp/decision/output" */              //Output
    TOPIC_DECISION_VEHICLE_INFO_ENUM =34,       /* "tcp/decision/vehicle_info" */        //RoadTaskVehicleInfo
    TOPIC_DECISION_FAULT_STATUS_ENUM =35,        /*"tcp/decision/fault_status" */        //TestTaskResult

    //diagnosis.fbs
    TOPIC_DIAGNOSIS_FAULT_CODE_ENUM =38,         /*"tcp/diagnosis/fault_code" */         //WarningOutput
    //at.fbs
    TOPIC_AT_CSQ_ENUM = 41,                        /* "tcp/at/csq"*/                        //ATMsg
    //hmi
    TOPIC_HMI_MSG0F1C_ENUM=44,                   /*"tcp/hmi/Msg0F1C" */                  //HMI触发测试任务Msg0F1C
    TOPIC_HMI_MSG8403_ENUM=45,                  /* "tcp/hmi/Msg8403"*/                   //HMI发送给网关设置矿卡空满载状态Msg8403
    TOPIC_HMI_MSG8F0B_ENUM=46,                  /* "tcp/hmi/Msg8F0B" */                  //Hmi生成文件后，需要车上传平台，通过该命令将文件上传网关Msg0F0B
    TOPIC_HMI_MSG0F01_ENUM=47,                   /*"tcp/hmi/Msg0F01" */                  //HMI发送网营运状态变更请求命令 Msg0F01关
    TOPIC_HMI_MSG0F10_ENUM=48,                   /*"tcp/hmi/Msg0F10"*/                   //HMI触发登录
    TOPIC_HMI_MSG0F11_ENUM=49,                   /*"tcp/hmi/Msg0F11"*/                   //HMI触发登出
    //新增hmi
    TOPIC_HMI_MsgTx_0104_ENUM=52,                 /*"tcp/hmi/MsgTx_0104" */            // 说明：服务器参数查询，消息体为空 适配：所有车辆
    TOPIC_HMI_MsgTx_0F03_ENUM=53,                /* "tcp/hmi/MsgTx_0F03"  */           // 说明：物料变更申请  适配：辅助车辆-电铲
    TOPIC_HMI_MsgTx_0F09_ENUM=54,                /* "tcp/hmi/MsgTx_0F09"   */           // 说明：行车报警 适配：矿卡/宽体车
    TOPIC_HMI_MsgTx_8305_ENUM=55,                 /*"tcp/hmi/MsgTx_8305" */            // 说明：作业场调度控制命令上报 适配：辅助车辆-电铲
    TOPIC_HMI_MsgTx_8B02_ENUM=56,                 /*"tcp/hmi/MsgTx_8B02" */             // 说明：发送digger就位命令 适配：辅助车辆-推土机digger
    TOPIC_HMI_MsgTx_8B07_list_ENUM=57,            /*"tcp/hmi/MsgTx_8B07_list" */        // 装载区停靠位位置上报：分组信息
    TOPIC_HMI_MsgTx_8B07_ENUM=58,                /* "tcp/hmi/MsgTx_8B07"  */            // 说明：装载区停靠位位置上报 适配：辅助车辆-电铲
    TOPIC_HMI_MsgTx_8B08_list_ENUM=59,            /*"tcp/hmi/MsgTx_8B08_list" */         // 装载区停靠位状态操作指令上报；分组信息
    TOPIC_HMI_MsgTx_8B08_ENUM=60,                 /*"tcp/hmi/MsgTx_8B08"     */        // 说明：装载区停靠位状态操作指令上报 适配：辅助车辆-电铲
    TOPIC_HMI_MsgTx_0002_ENUM = 61,               /*tcp/hmi/MsgTx_0002*/               //说明：HMI发出心跳，5s一包

    //cloudServer
    CLOUD_SEND_V2X_RSU_DATA_TOPIC_ENUM=63,       /*"ipc/cloud/msg_0A02"*/    //char * to V2xServer
    CLOUD_SEND_REQ_WORK_STATUS_ACK_ENUM=64,     /* "tcp/cloud/msg_8C01"  */  //Msg8C01
    CLOUD_SEND_OPR_STATUS_CHANGE_ENUM=65,       /* "tcp/cloud/msg_8F02"*/    //Msg8F02
    CLOUD_SEND_MATER_CHANGE_ENUM=66,             /*"tcp/cloud/msg_8F04"*/    //Msg8F04
    CLOUD_SEND_RECV_V2X_BROAD_ENUM=67,           /*"ipc/cloud/cmsg_v2x"*/    //char * to V2xServer
    CLOUD_SEND_CHANGE_TEMP_TASK_FILE_ENUM=68,    /*"tcp/cloud/msg_8F0D"*/    //Msg8F04
    CLOUD_SEND_DOWNLOAD_FILE_ENUM=69,            /*"tcp/cloud/msg_8F0A" */   //Msg8F0A
    CLOUD_SEND_REMOTE_CONTROL_ENUM=70,          /* "tcp/cloud/msg_8F09" */   //Msg8F09
    CLOUD_SEND_DOCKING_GROUP_ENUM=71,            /*"tcp/cloud/msg_8B01"*/    //Msg8B01
    CLOUD_SEND_WAITING_IN_LINE_ENUM=72,         /* "tcp/cloud/msg_8B03"*/    //Msg8B03
    CLOUD_SEND_CLEAR_FAULT_ENUM=73,             /* "tcp/cloud_hmi/msg_0F0E"*/ //Msg0F0E 0F0E这条HMI也会发
    CLOUD_SEND_RESP_EXIT_ENUM=74,                /*"tcp/cloud/msg_8105" */   //Msg8105
    CLOUD_SEND_RESP_SIGN_IN_ENUM=75,            /* "tcp/cloud/msg_8B10"*/    //Msg8B10
    CLOUD_SEND_RESP_SIGN_OUT_ENUM=76,           /* "tcp/cloud/msg_8B11" */   //Msg8B11
    CLOUD_SEND_REQ_LOAD_JOB_ENUM=77,            /* "tcp/cloud/msg_8307"*/    //Msg8307
    CLOUD_SEND_REQ_UNLOAD_JOB_ENUM=78,          /* "tcp/cloud/msg_8308" */   //Msg8308
    CLOUD_SEND_UPLOAD_TRIGGER_ENUM=79,          /* "tcp/cloud_hmi/msg_0502"*/ //Msg0502
    CLOUD_SEND_HMI_UPLOAD_RESULT_ENUM=80,        /*"tcp/cloud/msg_0F0B"*/    //Msg0F0B
    CLOUD_SEND_CGU_VERSION_ENUM=81,              /*"tcp/cloud/cgu_version"*/ //MsgCguVersion
    CLOUD_SEND_DEV_NAME_ENUM=82,                 /*"tcp/cloud/msg_1040"*/    //Msg1040
    CLOUD_SEND_HMI_WARNING_STATUS_ENUM = 83,      /*"tcp/cloud/warnstat"*/    //MsgVechWarnStat
    CLOUD_SEND_RES_TASK_FILE_ENUM = 84,          /*"tcp/cloud/msg_8F0C"*/    //Msg8F0C

    // cgu_protocol.fbs

    TOPIC_DECISION_MSG0503_ENUM=85,              /*"tcp/decision/msg_0503" */            //Msg0503

    // version.fbs
    TOPIC_VERSION_VCU_REQ_ENUM = 88,              /*"tcp/version/vcu/req"*/               //request for VCU Version
    TOPIC_VERSION_VCU_ENUM = 89,                  /*"tcp/version/vcu"*/                   //MsgVersion
    TOPIC_VERSION_ICU_REQ_ENUM = 90,              /*"tcp/version/icu/req"*/               //request for ICU Version
    TOPIC_VERSION_ICU_ENUM = 91,                  /*"tcp/version/icu"*/                   //MsgVersion
    TOPIC_VERSION_CCU_REQ_ENUM = 92,              /*"tcp/version/ccu/req"*/               //request for CCU Version
    TOPIC_VERSION_CCU_ENUM = 93,                  /*"tcp/version/ccu"*/                   //MsgVersion


    //decision.fbs

    TOPIC_DECISION_STARTUP_CMD_ENUM=94,          /*"tcp/decision/msg_1030" */            //Msg1030
    TOPIC_DECISION_SHUTDOWN_CMD_ENUM=95,        /* "tcp/decision/msg_1050"*/             //Msg1050
    TOPIC_DECISION_DISPATCH_REQ_ENUM=96,         /*"tcp/decision/msg_0305" */            //Msg0305
    TOPIC_DECISION_MANUAL_DRV_REQ_ENUM=97,     /* "tcp/decision/msg_0404" */            //Msg0404
    TOPIC_DECISION_TASK_EVENT_ENUM=98,           /*"tcp/decision/msg_0500"*/             //Msg0500
    TOPIC_DECISION_MSG0501_ENUM=99,           /*  "tcp/decision/msg_0501" */            //Msg0501
    TOPIC_DECISION_TEST_RESULT_ENUM=100 ,         /*"tcp/decision/msg_8F1C" */            //Msg8F1C


    // prediction.fbs
    TOPIC_PREDICTION_SENSE_RESULT_ENUM=103,      /* "tcp/prediction/sense_result" */      //SenseResultInfo  // msg_0402
    //rsu
    TOPIC_RSU_SEND_TO_CLOUD_ENUM=106,            /* "ipc/rsu/send_to_cloud"*/
    TOPIC_RSU_SEND_TO_V2X_ENUM=107,               /*"ipc/rsu/send_to_v2x"*/


    /******************************* req-rep *******************************/

    //AT topic for req-rep
    TOPIC_AT_REQ_REP_MSG_ENUM=110,                   /* "at_req_rep_msg"*/
    //call function name define
    FUNC_AT_GET_IMEI_ENUM=113,                       /* "getImei"*/
    FUNC_AT_GET_ICCID_ENUM=114,                      /* "getIccid"*/

    //v2x topic for req-rep
    TOPIC_V2X_REQ_REP_MSG_ENUM=116,                  /*"v2x_req_rep_msg"*/
    //call function name define
    FUNC_V2X_SEND_RTK_DATA_ENUM=119,                 /* "V2xSendRTKData"*/
    FUNC_V2X_SEND_RSU_CLOUD_DATA_ENUM=120,           /* "V2xSendRsuCloudData_v2x"*/
    FUNC_V2X_LINK_STATUS_ENUM=121,                   /* "LinkStatus"*/

    //cloud topic for req-rep
    TOPIC_CLOUD_REQ_REP_MSG_ENUM=124,                 /*"cloud_req_rep_msg"*/
    //call function name define
    FUNC_CLOUD_GET_LOCAL_NAME_ENUM=127,              /* "GetLocalName"*/
    FUNC_CLOUD_SEND_V2X_MSG_ENUM=128,                /* "CloudSendV2xMsg"*/
    FUNC_CLOUD_GET_ALL_DEV_NAME_ENUM=129,             /*"GetAllDevName"*/

    //rsu topic for req-rep
    TOPIC_RSU_REQ_REP_MSG_ENUM=132,                  /* "rsu_req_rep_msg"*/
    FUNC_RSU_CLOUD_SEND_RSU_V2X_DATA_ENUM=133,       /* "CloudSendRsuV2xData"*/
    FUNC_RSU_V2X_SEND_RSU_CLOUD_DATA_ENUM=134,        /*"V2xSendRsuCloudData_rsu"*/
    FUNC_RSU_LINK_STATUS_ENUM=135,                    /*"LinkStatus"*/

    //gnssRtk topic req-rep
    TOPIC_GNSSRTK_REQ_REP_MSG_ENUM=138,               /*"gnssrtk_req_rep_msg"*/
    FUNC_GNSSRTK_LINK_STATUS_ENUM=139,               /* "LinkStatus"*/
    FUNC_CLOUD_GET_CGU_VERSION_ENUM=140,               /*"GetCGUVersion"*/

    /***********************RCD驾驶舱*************************/

    ZMQ_TOPIC_ALL_ENUM,                                      /*""*/

    ZMQ_TOPIC_CONTROL_DATA_ENUM,              /* "ControlData"*/
    ZMQ_TOPIC_CONTROL_DEVLINKSTATUS_ENUM,     /*  "ControlDevLinkStatus"*/
    ZMQ_TOPIC_BUTTON_ENUM,                      /*"ControlButton"*/
    ZMQ_TOPIC_CONTROL_LIGHT_ENUM,               /*"ControlLight"*/

    ZMQ_TOPIC_REQ_REMOTE_CONTROL_ENUM,         /* "ReqRemoteControl"*/
    ZMQ_TOPIC_RESP_REMOTE_CONTROL_ENUM,         /*"RespRemoteControl"*/

    ZMQ_TOPIC_VEHICLE_STATUS_ENUM,              /*"VehicleStatus"*/
    ZMQ_TOPIC_VEHICLE_RAWDATA_ENUM,            /* "VehicleRawData"*/

    ZMA_TOPIC_VEHICLE_DELAY_ENUM,             /*  "Ping"*/

    ZMQ_TOPIC_REPORT_HMI_MSG_ENUM,              /*"HmiMsg"*/
    ZMQ_TOPIC_REQ_SEND_HMIMSG_ENUM,            /* "ReqSendHmiMsg"*/

};

inline const char* enumToString(Topic_ID operatior) {
    switch(operatior) {
#define interface(name) \
    case name: \
    return #name; \
    break;

    interface(TOPIC_TCP_GNSS_GPS_INFO_ENUM);
    interface(TOPIC_TCP_GNSS_GPS_STATUS_ENUM);
    interface(TOPIC_IPC_MESSAGE_TEST_TOPIC_ENUM);
    interface(TOPIC_HUAWEI_GNSS_BROADCAST_DATA_ENUM);
    interface(TOPIC_GNSS_BROADCAST_DATA_ENUM);
    interface(TOPIC_HMI_SEND_VOICE_PLAY_ENUM);
    interface(TOPIC_CV2X_COll_WARNING_ENUM);
    interface(TOPIC_CV2X_SEND_GNSS_RTK_DATA_ENUM);
    interface(TOPIC_CV2X_RECV_RSU_CLOUD_DATA_ENUM);
    interface(TOPIC_CV2X_AUX_RESP_JOB_SCJED_DATA_ENUM);
    interface(TOPIC_CV2X_TRUCK_REQ_JOB_SCHED_DATA_ENUM);
    interface(TOPIC_CAN_WIRE_CONTROL_FAULT_ENUM);
    interface(TOPIC_CAN_WIRE_CONTROL_MOVATION_ENUM);
    interface(TOPIC_CAN_WIRE_CONTROL_STATUS_ENUM);
    interface(TOPIC_CAN_WIRE_CONTROL_VCU_VER_ENUM);
    interface(TOPIC_CAN_DEV_STATUS_ENUM);
    interface(TOPIC_CONTROL_IMPLEMENT_ENUM);
    interface(TOPIC_CONTROL_DECISION_ENUM);
    interface(TOPIC_DECISION_OUTPUT_ENUM);
    interface(TOPIC_DECISION_VEHICLE_INFO_ENUM);
    interface(TOPIC_DECISION_FAULT_STATUS_ENUM);
    interface(TOPIC_DIAGNOSIS_FAULT_CODE_ENUM);
    interface(TOPIC_AT_CSQ_ENUM);
    interface(TOPIC_HMI_MSG0F1C_ENUM);
    interface(TOPIC_HMI_MSG8403_ENUM);
    interface(TOPIC_HMI_MSG8F0B_ENUM);
    interface(TOPIC_HMI_MSG0F01_ENUM);
    interface(TOPIC_HMI_MSG0F10_ENUM);
    interface(TOPIC_HMI_MSG0F11_ENUM);
    interface(TOPIC_HMI_MsgTx_0104_ENUM);
    interface(TOPIC_HMI_MsgTx_0F03_ENUM);
    interface(TOPIC_HMI_MsgTx_0F09_ENUM);
    interface(TOPIC_HMI_MsgTx_8305_ENUM);
    interface(TOPIC_HMI_MsgTx_8B02_ENUM);
    interface(TOPIC_HMI_MsgTx_8B07_list_ENUM);
    interface(TOPIC_HMI_MsgTx_8B07_ENUM);
    interface(TOPIC_HMI_MsgTx_8B08_list_ENUM);
    interface(TOPIC_HMI_MsgTx_8B08_ENUM);
    interface(TOPIC_HMI_MsgTx_0002_ENUM);
    interface(FUNC_CLOUD_GET_CGU_VERSION_ENUM);

    interface(ZMQ_TOPIC_ALL_ENUM);                                      /*""*/
    interface(ZMQ_TOPIC_CONTROL_DATA_ENUM);              /* "ControlData"*/
    interface(ZMQ_TOPIC_CONTROL_DEVLINKSTATUS_ENUM);     /*  "ControlDevLinkStatus"*/
    interface(ZMQ_TOPIC_BUTTON_ENUM);                      /*"ControlButton"*/
    interface(ZMQ_TOPIC_CONTROL_LIGHT_ENUM);               /*"ControlLight"*/
    interface(ZMQ_TOPIC_REQ_REMOTE_CONTROL_ENUM);         /* "ReqRemoteControl"*/
    interface(ZMQ_TOPIC_RESP_REMOTE_CONTROL_ENUM);         /*"RespRemoteControl"*/
    interface(ZMQ_TOPIC_VEHICLE_STATUS_ENUM);              /*"VehicleStatus"*/
    interface(ZMQ_TOPIC_VEHICLE_RAWDATA_ENUM);            /* "VehicleRawData"*/
    interface(ZMA_TOPIC_VEHICLE_DELAY_ENUM);             /*  "Ping"*/
    interface(ZMQ_TOPIC_REPORT_HMI_MSG_ENUM);              /*"HmiMsg"*/
    interface(ZMQ_TOPIC_REQ_SEND_HMIMSG_ENUM);            /* "ReqSendHmiMsg"*/

#undef interface
    default:
        return "UNKNOW";
    }
    return "UNKNOW";
}

inline const char* enumToOtherString(Topic_ID operatior) {
    switch(operatior) {
#define interface(operatior, v) \
    case operatior: \
    return #v; \
    break;
    interface(TOPIC_TCP_GNSS_GPS_INFO_ENUM, tcp/gnss/gps_info);
    interface(TOPIC_TCP_GNSS_GPS_STATUS_ENUM, tcp/gnss/gps_status);
    interface(TOPIC_IPC_MESSAGE_TEST_TOPIC_ENUM, ipc/message/test_topic);
    interface(TOPIC_HUAWEI_GNSS_BROADCAST_DATA_ENUM,ipc/gnss/HuaWeiGnssBroadcastData);
    interface(TOPIC_GNSS_BROADCAST_DATA_ENUM,tcp/gnss/GnssBroadcastData);
    interface(TOPIC_HMI_SEND_VOICE_PLAY_ENUM,tcp/voice/HmiSendVoicePlay);
    interface(TOPIC_CV2X_COll_WARNING_ENUM,tcp/v2x/msg_8401);
    interface(TOPIC_CV2X_SEND_GNSS_RTK_DATA_ENUM,tcp/v2x/send_gnss_rkt_data);
    interface(TOPIC_CV2X_RECV_RSU_CLOUD_DATA_ENUM,tcp/v2x/recv_rsu_cloud_data);
    interface(TOPIC_CV2X_AUX_RESP_JOB_SCJED_DATA_ENUM,tcp/v2x/aux_resp_job_sched_data);
    interface(TOPIC_CV2X_TRUCK_REQ_JOB_SCHED_DATA_ENUM,tcp/v2x/truck_req_job_sched_data);
    interface(TOPIC_CAN_WIRE_CONTROL_FAULT_ENUM,tcp/can/wire_control_fault);
    interface(TOPIC_CAN_WIRE_CONTROL_MOVATION_ENUM,tcp/can/wire_control_movation);
    interface(TOPIC_CAN_WIRE_CONTROL_STATUS_ENUM,tcp/can/wire_control_status);
    interface(TOPIC_CAN_WIRE_CONTROL_VCU_VER_ENUM,tcp/can/wire_control_vcu_ver);
    interface(TOPIC_CAN_DEV_STATUS_ENUM,tcp/can/can_about_dev_status);
    interface(TOPIC_CONTROL_IMPLEMENT_ENUM,tcp/control/control_implement);
    interface(TOPIC_CONTROL_DECISION_ENUM,tcp/control/control_decision);
    interface(TOPIC_DECISION_OUTPUT_ENUM,tcp/decision/output);
    interface(TOPIC_DECISION_VEHICLE_INFO_ENUM,tcp/decision/vehicle_info);
    interface(TOPIC_DECISION_FAULT_STATUS_ENUM,tcp/decision/fault_status);
    interface(TOPIC_DIAGNOSIS_FAULT_CODE_ENUM,tcp/diagnosis/fault_code);
    interface(TOPIC_AT_CSQ_ENUM,tcp/at/csq);
    interface(TOPIC_HMI_MSG0F1C_ENUM,tcp/hmi/Msg0F1C);
    interface(TOPIC_HMI_MSG8403_ENUM,tcp/hmi/Msg8403);
    interface(TOPIC_HMI_MSG8F0B_ENUM,tcp/hmi/Msg8F0B);
    interface(TOPIC_HMI_MSG0F01_ENUM,tcp/hmi/Msg0F01);
    interface(TOPIC_HMI_MSG0F10_ENUM,tcp/hmi/Msg0F10);
    interface(TOPIC_HMI_MSG0F11_ENUM,tcp/hmi/Msg0F11);
    interface(TOPIC_HMI_MsgTx_0104_ENUM,tcp/hmi/MsgTx_0104);
    interface(TOPIC_HMI_MsgTx_0F03_ENUM,tcp/hmi/MsgTx_0F03);
    interface(TOPIC_HMI_MsgTx_0F09_ENUM,tcp/hmi/MsgTx_0F09);
    interface(TOPIC_HMI_MsgTx_8305_ENUM,tcp/hmi/MsgTx_8305);
    interface(TOPIC_HMI_MsgTx_8B02_ENUM,tcp/hmi/MsgTx_8B02);
    interface(TOPIC_HMI_MsgTx_8B07_list_ENUM,tcp/hmi/MsgTx_8B07_list);
    interface(TOPIC_HMI_MsgTx_8B07_ENUM,tcp/hmi/MsgTx_8B07);
    interface(TOPIC_HMI_MsgTx_8B08_list_ENUM,tcp/hmi/MsgTx_8B08_list);
    interface(TOPIC_HMI_MsgTx_8B08_ENUM,tcp/hmi/MsgTx_8B08);
    interface(TOPIC_HMI_MsgTx_0002_ENUM,tcp/hmi/MsgTx_0002);

    //cloudServer
    interface(CLOUD_SEND_V2X_RSU_DATA_TOPIC_ENUM,ipc/cloud/msg_0A02);    //char * to V2xServer
    interface(CLOUD_SEND_REQ_WORK_STATUS_ACK_ENUM ,tcp/cloud/msg_8C01);   //Msg8C01
    interface(CLOUD_SEND_OPR_STATUS_CHANGE_ENUM,tcp/cloud/msg_8F02);    //Msg8F02
    interface(CLOUD_SEND_MATER_CHANGE_ENUM,tcp/cloud/msg_8F04);    //Msg8F04
    interface(CLOUD_SEND_RECV_V2X_BROAD_ENUM,ipc/cloud/cmsg_v2x);    //char * to V2xServer
    interface(CLOUD_SEND_CHANGE_TEMP_TASK_FILE_ENUM,tcp/cloud/msg_8F0D);   //Msg8F04
    interface(CLOUD_SEND_DOWNLOAD_FILE_ENUM,tcp/cloud/msg_8F0A);   //Msg8F0A
    interface(CLOUD_SEND_REMOTE_CONTROL_ENUM,tcp/cloud/msg_8F09);  //Msg8F09
    interface(CLOUD_SEND_DOCKING_GROUP_ENUM,tcp/cloud/msg_8B01);   //Msg8B01
    interface(CLOUD_SEND_WAITING_IN_LINE_ENUM,tcp/cloud/msg_8B03);    //Msg8B03
    interface(CLOUD_SEND_CLEAR_FAULT_ENUM,tcp/cloud_hmi/msg_0F0E); //Msg0F0E 0F0E这条HMI也会发
    interface(CLOUD_SEND_RESP_EXIT_ENUM,tcp/cloud/msg_8105);   //Msg8105
    interface(CLOUD_SEND_RESP_SIGN_IN_ENUM,tcp/cloud/msg_8B10);    //Msg8B10
    interface(CLOUD_SEND_RESP_SIGN_OUT_ENUM,tcp/cloud/msg_8B11);   //Msg8B11
    interface(CLOUD_SEND_REQ_LOAD_JOB_ENUM,tcp/cloud/msg_8307);    //Msg8307
    interface(CLOUD_SEND_REQ_UNLOAD_JOB_ENUM,tcp/cloud/msg_8308);   //Msg8308
    interface(CLOUD_SEND_UPLOAD_TRIGGER_ENUM,tcp/cloud_hmi/msg_0502); //Msg0502
    interface(CLOUD_SEND_HMI_UPLOAD_RESULT_ENUM,tcp/cloud/msg_0F0B);    //Msg0F0B
    interface(CLOUD_SEND_CGU_VERSION_ENUM,tcp/cloud/cgu_version); //MsgCguVersion
    interface(CLOUD_SEND_DEV_NAME_ENUM,tcp/cloud/msg_1040);   //Msg1040
    interface(CLOUD_SEND_HMI_WARNING_STATUS_ENUM,tcp/cloud/warnstat);   //MsgVechWarnStat
    interface(CLOUD_SEND_RES_TASK_FILE_ENUM,tcp/cloud/msg_8F0C);   //Msg8F0C

    // cgu_protocol.fbs

    interface(TOPIC_DECISION_MSG0503_ENUM,tcp/decision/msg_0503);            //Msg0503

    // version.fbs
    interface(TOPIC_VERSION_VCU_REQ_ENUM,tcp/version/vcu/req);
    interface(TOPIC_VERSION_VCU_ENUM,tcp/version/vcu);
    interface(TOPIC_VERSION_ICU_REQ_ENUM,tcp/version/icu/req);
    interface(TOPIC_VERSION_ICU_ENUM,tcp/version/icu);
    interface(TOPIC_VERSION_CCU_REQ_ENUM,tcp/version/ccu/req);
    interface(TOPIC_VERSION_CCU_ENUM,tcp/version/ccu);

    //decision.fbs

    interface(TOPIC_DECISION_STARTUP_CMD_ENUM,tcp/decision/msg_1030);            //Msg1030
    interface(TOPIC_DECISION_SHUTDOWN_CMD_ENUM,tcp/decision/msg_1050);             //Msg1050
    interface(TOPIC_DECISION_DISPATCH_REQ_ENUM,tcp/decision/msg_0305);            //Msg0305
    interface(TOPIC_DECISION_MANUAL_DRV_REQ_ENUM,tcp/decision/msg_0404);           //Msg0404
    interface(TOPIC_DECISION_TASK_EVENT_ENUM,tcp/decision/msg_0500);             //Msg0500
    interface(TOPIC_DECISION_MSG0501_ENUM,tcp/decision/msg_0501);            //Msg0501
    interface(TOPIC_DECISION_TEST_RESULT_ENUM,tcp/decision/msg_8F1C);            //Msg8F1C


    // prediction.fbs
    interface(TOPIC_PREDICTION_SENSE_RESULT_ENUM,tcp/prediction/sense_result);      //SenseResultInfo  // msg_0402
    //rsu
    interface(TOPIC_RSU_SEND_TO_CLOUD_ENUM,ipc/rsu/send_to_cloud);
    interface(TOPIC_RSU_SEND_TO_V2X_ENUM,ipc/rsu/send_to_v2x);


    /******************************* req-rep *******************************/

    //AT topic for req-rep
    interface(TOPIC_AT_REQ_REP_MSG_ENUM,at_req_rep_msg);
    //call function name define
    interface(FUNC_AT_GET_IMEI_ENUM,getImei);
    interface(FUNC_AT_GET_ICCID_ENUM,getIccid);

    //v2x topic for req-rep
    interface(TOPIC_V2X_REQ_REP_MSG_ENUM,v2x_req_rep_msg);
    //call function name define
    interface(FUNC_V2X_SEND_RTK_DATA_ENUM,V2xSendRTKData);
    interface(FUNC_V2X_SEND_RSU_CLOUD_DATA_ENUM,V2xSendRsuCloudData_v2x);
    interface(FUNC_V2X_LINK_STATUS_ENUM,LinkStatus);

    //cloud topic for req-rep
    interface(TOPIC_CLOUD_REQ_REP_MSG_ENUM,cloud_req_rep_msg);
    //call function name define
    interface(FUNC_CLOUD_GET_LOCAL_NAME_ENUM,GetLocalName)
            interface(FUNC_CLOUD_SEND_V2X_MSG_ENUM,CloudSendV2xMsg);
    interface(FUNC_CLOUD_GET_ALL_DEV_NAME_ENUM,GetAllDevName);

    //rsu topic for req-rep
    interface(TOPIC_RSU_REQ_REP_MSG_ENUM,rsu_req_rep_msg);
    interface(FUNC_RSU_CLOUD_SEND_RSU_V2X_DATA_ENUM,CloudSendRsuV2xData);
    interface(FUNC_RSU_V2X_SEND_RSU_CLOUD_DATA_ENUM,V2xSendRsuCloudData_rsu);
    interface(FUNC_RSU_LINK_STATUS_ENUM,LinkStatus);

    //gnssRtk topic req-rep
    interface(TOPIC_GNSSRTK_REQ_REP_MSG_ENUM,gnssrtk_req_rep_msg);
    interface(FUNC_GNSSRTK_LINK_STATUS_ENUM,LinkStatus);
    interface(FUNC_CLOUD_GET_CGU_VERSION_ENUM,GetCGUVersion);//版本号


    /*************RCD************/
    interface(ZMQ_TOPIC_ALL_ENUM,);                                      /*""*/
    interface(ZMQ_TOPIC_CONTROL_DATA_ENUM,ControlData);              /* "ControlData"*/
    interface(ZMQ_TOPIC_CONTROL_DEVLINKSTATUS_ENUM,ControlDevLinkStatus);     /*  "ControlDevLinkStatus"*/
    interface(ZMQ_TOPIC_BUTTON_ENUM,ControlButton);                      /*"ControlButton"*/
    interface(ZMQ_TOPIC_CONTROL_LIGHT_ENUM,ControlLight);               /*"ControlLight"*/
    interface(ZMQ_TOPIC_REQ_REMOTE_CONTROL_ENUM,ReqRemoteControl);         /* "ReqRemoteControl"*/
    interface(ZMQ_TOPIC_RESP_REMOTE_CONTROL_ENUM,RespRemoteControl);         /*"RespRemoteControl"*/
    interface(ZMQ_TOPIC_VEHICLE_STATUS_ENUM,VehicleStatus);              /*"VehicleStatus"*/
    interface(ZMQ_TOPIC_VEHICLE_RAWDATA_ENUM,VehicleRawData);            /* "VehicleRawData"*/
    interface(ZMA_TOPIC_VEHICLE_DELAY_ENUM,Ping);             /*  "Ping"*/
    interface(ZMQ_TOPIC_REPORT_HMI_MSG_ENUM,HmiMsg);              /*"HmiMsg"*/
    interface(ZMQ_TOPIC_REQ_SEND_HMIMSG_ENUM,ReqSendHmiMsg);            /* "ReqSendHmiMsg"*/
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
    interface(TOPIC_TCP_GNSS_GPS_INFO_ENUM, tcp/gnss/gps_info);
    interface(TOPIC_TCP_GNSS_GPS_STATUS_ENUM, tcp/gnss/gps_status);
    interface(TOPIC_IPC_MESSAGE_TEST_TOPIC_ENUM, ipc/message/test_topic);
    interface(TOPIC_HUAWEI_GNSS_BROADCAST_DATA_ENUM,ipc/gnss/HuaWeiGnssBroadcastData);
    interface(TOPIC_GNSS_BROADCAST_DATA_ENUM,tcp/gnss/GnssBroadcastData);
    interface(TOPIC_HMI_SEND_VOICE_PLAY_ENUM,tcp/voice/HmiSendVoicePlay);
    interface(TOPIC_CV2X_COll_WARNING_ENUM,tcp/v2x/msg_8401);
    interface(TOPIC_CV2X_SEND_GNSS_RTK_DATA_ENUM,tcp/v2x/send_gnss_rkt_data);
    interface(TOPIC_CV2X_RECV_RSU_CLOUD_DATA_ENUM,tcp/v2x/recv_rsu_cloud_data);
    interface(TOPIC_CV2X_AUX_RESP_JOB_SCJED_DATA_ENUM,tcp/v2x/aux_resp_job_sched_data);
    interface(TOPIC_CV2X_TRUCK_REQ_JOB_SCHED_DATA_ENUM,tcp/v2x/truck_req_job_sched_data);
    interface(TOPIC_CAN_WIRE_CONTROL_FAULT_ENUM,tcp/can/wire_control_fault);
    interface(TOPIC_CAN_WIRE_CONTROL_MOVATION_ENUM,tcp/can/wire_control_movation);
    interface(TOPIC_CAN_WIRE_CONTROL_STATUS_ENUM,tcp/can/wire_control_status);
    interface(TOPIC_CAN_WIRE_CONTROL_VCU_VER_ENUM,tcp/can/wire_control_vcu_ver);
    interface(TOPIC_CAN_DEV_STATUS_ENUM,tcp/can/can_about_dev_status);
    interface(TOPIC_CONTROL_IMPLEMENT_ENUM,tcp/control/control_implement);
    interface(TOPIC_CONTROL_DECISION_ENUM,tcp/control/control_decision);
    interface(TOPIC_DECISION_OUTPUT_ENUM,tcp/decision/output);
    interface(TOPIC_DECISION_VEHICLE_INFO_ENUM,tcp/decision/vehicle_info);
    interface(TOPIC_DECISION_FAULT_STATUS_ENUM,tcp/decision/fault_status);
    interface(TOPIC_DIAGNOSIS_FAULT_CODE_ENUM,tcp/diagnosis/fault_code);
    interface(TOPIC_AT_CSQ_ENUM,tcp/at/csq);
    interface(TOPIC_HMI_MSG0F1C_ENUM,tcp/hmi/Msg0F1C);
    interface(TOPIC_HMI_MSG8403_ENUM,tcp/hmi/Msg8403);
    interface(TOPIC_HMI_MSG8F0B_ENUM,tcp/hmi/Msg8F0B);
    interface(TOPIC_HMI_MSG0F01_ENUM,tcp/hmi/Msg0F01);
    interface(TOPIC_HMI_MSG0F10_ENUM,tcp/hmi/Msg0F10);
    interface(TOPIC_HMI_MSG0F11_ENUM,tcp/hmi/Msg0F11);
    interface(TOPIC_HMI_MsgTx_0104_ENUM,tcp/hmi/MsgTx_0104);
    interface(TOPIC_HMI_MsgTx_0F03_ENUM,tcp/hmi/MsgTx_0F03);
    interface(TOPIC_HMI_MsgTx_0F09_ENUM,tcp/hmi/MsgTx_0F09);
    interface(TOPIC_HMI_MsgTx_8305_ENUM,tcp/hmi/MsgTx_8305);
    interface(TOPIC_HMI_MsgTx_8B02_ENUM,tcp/hmi/MsgTx_8B02);
    interface(TOPIC_HMI_MsgTx_8B07_list_ENUM,tcp/hmi/MsgTx_8B07_list);
    interface(TOPIC_HMI_MsgTx_8B07_ENUM,tcp/hmi/MsgTx_8B07);
    interface(TOPIC_HMI_MsgTx_8B08_list_ENUM,tcp/hmi/MsgTx_8B08_list);
    interface(TOPIC_HMI_MsgTx_8B08_ENUM,tcp/hmi/MsgTx_8B08);
    interface(TOPIC_HMI_MsgTx_0002_ENUM,tcp/hmi/MsgTx_0002);

    //cloudServer
    interface(CLOUD_SEND_V2X_RSU_DATA_TOPIC_ENUM,ipc/cloud/msg_0A02);    //char * to V2xServer
    interface(CLOUD_SEND_REQ_WORK_STATUS_ACK_ENUM ,tcp/cloud/msg_8C01);   //Msg8C01
    interface(CLOUD_SEND_OPR_STATUS_CHANGE_ENUM,tcp/cloud/msg_8F02);    //Msg8F02
    interface(CLOUD_SEND_MATER_CHANGE_ENUM,tcp/cloud/msg_8F04);    //Msg8F04
    interface(CLOUD_SEND_RECV_V2X_BROAD_ENUM,ipc/cloud/cmsg_v2x);    //char * to V2xServer
    interface(CLOUD_SEND_CHANGE_TEMP_TASK_FILE_ENUM,tcp/cloud/msg_8F0D);   //Msg8F04
    interface(CLOUD_SEND_DOWNLOAD_FILE_ENUM,tcp/cloud/msg_8F0A);   //Msg8F0A
    interface(CLOUD_SEND_REMOTE_CONTROL_ENUM,tcp/cloud/msg_8F09);  //Msg8F09
    interface(CLOUD_SEND_DOCKING_GROUP_ENUM,tcp/cloud/msg_8B01);   //Msg8B01
    interface(CLOUD_SEND_WAITING_IN_LINE_ENUM,tcp/cloud/msg_8B03);    //Msg8B03
    interface(CLOUD_SEND_CLEAR_FAULT_ENUM,tcp/cloud_hmi/msg_0F0E); //Msg0F0E 0F0E这条HMI也会发
    interface(CLOUD_SEND_RESP_EXIT_ENUM,tcp/cloud/msg_8105);   //Msg8105
    interface(CLOUD_SEND_RESP_SIGN_IN_ENUM,tcp/cloud/msg_8B10);    //Msg8B10
    interface(CLOUD_SEND_RESP_SIGN_OUT_ENUM,tcp/cloud/msg_8B11);   //Msg8B11
    interface(CLOUD_SEND_REQ_LOAD_JOB_ENUM,tcp/cloud/msg_8307);    //Msg8307
    interface(CLOUD_SEND_REQ_UNLOAD_JOB_ENUM,tcp/cloud/msg_8308);   //Msg8308
    interface(CLOUD_SEND_UPLOAD_TRIGGER_ENUM,tcp/cloud_hmi/msg_0502); //Msg0502
    interface(CLOUD_SEND_HMI_UPLOAD_RESULT_ENUM,tcp/cloud/msg_0F0B);    //Msg0F0B
    interface(CLOUD_SEND_CGU_VERSION_ENUM,tcp/cloud/cgu_version); //MsgCguVersion
    interface(CLOUD_SEND_DEV_NAME_ENUM,tcp/cloud/msg_1040);   //Msg1040
    interface(CLOUD_SEND_HMI_WARNING_STATUS_ENUM,tcp/cloud/warnstat);   //MsgVechWarnStat
    interface(CLOUD_SEND_RES_TASK_FILE_ENUM,tcp/cloud/msg_8F0C);   //Msg8F0C

    // cgu_protocol.fbs

    interface(TOPIC_DECISION_MSG0503_ENUM,tcp/decision/msg_0503);            //Msg0503

    // version.fbs
    interface(TOPIC_VERSION_VCU_REQ_ENUM,tcp/version/vcu/req);
    interface(TOPIC_VERSION_VCU_ENUM,tcp/version/vcu);
    interface(TOPIC_VERSION_ICU_REQ_ENUM,tcp/version/icu/req);
    interface(TOPIC_VERSION_ICU_ENUM,tcp/version/icu);
    interface(TOPIC_VERSION_CCU_REQ_ENUM,tcp/version/ccu/req);
    interface(TOPIC_VERSION_CCU_ENUM,tcp/version/ccu);

    //decision.fbs

    interface(TOPIC_DECISION_STARTUP_CMD_ENUM,tcp/decision/msg_1030);            //Msg1030
    interface(TOPIC_DECISION_SHUTDOWN_CMD_ENUM,tcp/decision/msg_1050);             //Msg1050
    interface(TOPIC_DECISION_DISPATCH_REQ_ENUM,tcp/decision/msg_0305);            //Msg0305
    interface(TOPIC_DECISION_MANUAL_DRV_REQ_ENUM,tcp/decision/msg_0404);           //Msg0404
    interface(TOPIC_DECISION_TASK_EVENT_ENUM,tcp/decision/msg_0500);             //Msg0500
    interface(TOPIC_DECISION_MSG0501_ENUM,tcp/decision/msg_0501);            //Msg0501
    interface(TOPIC_DECISION_TEST_RESULT_ENUM,tcp/decision/msg_8F1C);            //Msg8F1C


    // prediction.fbs
    interface(TOPIC_PREDICTION_SENSE_RESULT_ENUM,tcp/prediction/sense_result);      //SenseResultInfo  // msg_0402
    //rsu
    interface(TOPIC_RSU_SEND_TO_CLOUD_ENUM,ipc/rsu/send_to_cloud);
    interface(TOPIC_RSU_SEND_TO_V2X_ENUM,ipc/rsu/send_to_v2x);


    /******************************* req-rep *******************************/

    //AT topic for req-rep
    interface(TOPIC_AT_REQ_REP_MSG_ENUM,at_req_rep_msg);
    //call function name define
    interface(FUNC_AT_GET_IMEI_ENUM,getImei);
    interface(FUNC_AT_GET_ICCID_ENUM,getIccid);

    //v2x topic for req-rep
    interface(TOPIC_V2X_REQ_REP_MSG_ENUM,v2x_req_rep_msg);
    //call function name define
    interface(FUNC_V2X_SEND_RTK_DATA_ENUM,V2xSendRTKData);
    interface(FUNC_V2X_SEND_RSU_CLOUD_DATA_ENUM,V2xSendRsuCloudData_v2x);
    interface(FUNC_V2X_LINK_STATUS_ENUM,LinkStatus);

    //cloud topic for req-rep
    interface(TOPIC_CLOUD_REQ_REP_MSG_ENUM,cloud_req_rep_msg);
    //call function name define
    interface(FUNC_CLOUD_GET_LOCAL_NAME_ENUM,GetLocalName)
            interface(FUNC_CLOUD_SEND_V2X_MSG_ENUM,CloudSendV2xMsg);
    interface(FUNC_CLOUD_GET_ALL_DEV_NAME_ENUM,GetAllDevName);

    //rsu topic for req-rep
    interface(TOPIC_RSU_REQ_REP_MSG_ENUM,rsu_req_rep_msg);
    interface(FUNC_RSU_CLOUD_SEND_RSU_V2X_DATA_ENUM,CloudSendRsuV2xData);
    interface(FUNC_RSU_V2X_SEND_RSU_CLOUD_DATA_ENUM,V2xSendRsuCloudData_rsu);
    interface(FUNC_RSU_LINK_STATUS_ENUM,LinkStatus);

    //gnssRtk topic req-rep
    interface(TOPIC_GNSSRTK_REQ_REP_MSG_ENUM,gnssrtk_req_rep_msg);
    interface(FUNC_GNSSRTK_LINK_STATUS_ENUM,LinkStatus);
    interface(FUNC_CLOUD_GET_CGU_VERSION_ENUM,GetCGUVersion);//版本号

    /*************RCD************/
    interface(ZMQ_TOPIC_ALL_ENUM,);                                      /*""*/
    interface(ZMQ_TOPIC_CONTROL_DATA_ENUM,ControlData);              /* "ControlData"*/
    interface(ZMQ_TOPIC_CONTROL_DEVLINKSTATUS_ENUM,ControlDevLinkStatus);     /*  "ControlDevLinkStatus"*/
    interface(ZMQ_TOPIC_BUTTON_ENUM,ControlButton);                      /*"ControlButton"*/
    interface(ZMQ_TOPIC_CONTROL_LIGHT_ENUM,ControlLight);               /*"ControlLight"*/
    interface(ZMQ_TOPIC_REQ_REMOTE_CONTROL_ENUM,ReqRemoteControl);         /* "ReqRemoteControl"*/
    interface(ZMQ_TOPIC_RESP_REMOTE_CONTROL_ENUM,RespRemoteControl);         /*"RespRemoteControl"*/
    interface(ZMQ_TOPIC_VEHICLE_STATUS_ENUM,VehicleStatus);              /*"VehicleStatus"*/
    interface(ZMQ_TOPIC_VEHICLE_RAWDATA_ENUM,VehicleRawData);            /* "VehicleRawData"*/
    interface(ZMA_TOPIC_VEHICLE_DELAY_ENUM,Ping);             /*  "Ping"*/
    interface(ZMQ_TOPIC_REPORT_HMI_MSG_ENUM,HmiMsg);              /*"HmiMsg"*/
    interface(ZMQ_TOPIC_REQ_SEND_HMIMSG_ENUM,ReqSendHmiMsg);            /* "ReqSendHmiMsg"*/

    return UNKNOW;
#undef interface
}
}
#endif // TOPICCOMMON_H
