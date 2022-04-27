// this file is a temp version for debugging

#ifndef __TOPIC_H_
#define __TOPIC_H_

// topic for test
#define TOPIC_IPC_MESSAGE_TEST_TOPIC  "ipc/message/test_topic"

// gps_info.fbs
#define TOPIC_TCP_GNSS_GPS_INFO             "tcp/gnss/gps_info"       //GpsImuInfo
#define TOPIC_TCP_GNSS_GPS_STATUS           "tcp/gnss/gps_status"     //GpsStatus

//gnssServer
#define TOPIC_HUAWEI_GNSS_BROADCAST_DATA    "ipc/gnss/HuaWeiGnssBroadcastData"//MsgVector
#define TOPIC_GNSS_BROADCAST_DATA           "tcp/gnss/GnssBroadcastData"//MsgGnssBroadcastData 

//voiceServer
#define TOPIC_HMI_SEND_VOICE_PLAY           "tcp/voice/HmiSendVoicePlay"//MsgVoiceName

// v2xServer
#define TOPIC_CV2X_COll_WARNING             "tcp/v2x/msg_8401"
#define TOPIC_CV2X_SEND_GNSS_RTK_DATA       "tcp/v2x/send_gnss_rkt_data"
#define TOPIC_CV2X_RECV_RSU_CLOUD_DATA      "tcp/v2x/recv_rsu_cloud_data"
//这条原来为1.CGU收到之后重新组一个8306给HMI 现在由HMI订阅此消息当作8306
//2.CGU收到以后判断时间发一个8307给HMI和CCU
#define TOPIC_CV2X_AUX_RESP_JOB_SCHED_DATA  "tcp/v2x/aux_resp_job_sched_data" //Msg8305_V2X
#define TOPIC_CV2X_TRUCK_REQ_JOB_SCHED_DATA "tcp/v2x/truck_req_job_sched_data"

//cloudServer
#define CLOUD_SEND_V2X_RSU_DATA_TOPIC       "ipc/cloud/msg_0A02"    //char * to V2xServer
#define CLOUD_SEND_REQ_WORK_STATUS_ACK      "tcp/cloud/msg_8C01"    //Msg8C01
#define CLOUD_SEND_OPR_STATUS_CHANGE        "tcp/cloud/msg_8F02"    //Msg8F02
#define CLOUD_SEND_MATER_CHANGE             "tcp/cloud/msg_8F04"    //Msg8F04
#define CLOUD_SEND_RECV_V2X_BROAD           "ipc/cloud/cmsg_v2x"    //char * to V2xServer
#define CLOUD_SEND_CHANGE_TEMP_TASK_FILE    "tcp/cloud/msg_8F0D"    //Msg8F04
#define CLOUD_SEND_DOWNLOAD_FILE            "tcp/cloud/msg_8F0A"    //Msg8F0A
#define CLOUD_SEND_REMOTE_CONTROL           "tcp/cloud/msg_8F09"    //Msg8F09
#define CLOUD_SEND_DOCKING_GROUP            "tcp/cloud/msg_8B01"    //Msg8B01
#define CLOUD_SEND_WAITING_IN_LINE          "tcp/cloud/msg_8B03"    //Msg8B03
#define CLOUD_SEND_CLEAR_FAULT              "tcp/cloud_hmi/msg_0F0E"//Msg0F0E 0F0E这条HMI也会发 hmi||cloud -> ccu
#define CLOUD_SEND_RESP_EXIT                "tcp/cloud/msg_8105"    //Msg8105
#define CLOUD_SEND_RESP_SIGN_IN             "tcp/cloud/msg_8B10"    //Msg8B10
#define CLOUD_SEND_RESP_SIGN_OUT            "tcp/cloud/msg_8B11"    //Msg8B11
#define CLOUD_SEND_REQ_LOAD_JOB             "tcp/cloud/msg_8307"    //Msg8307
#define CLOUD_SEND_REQ_UNLOAD_JOB           "tcp/cloud/msg_8308"    //Msg8308
#define CLOUD_SEND_UPLOAD_TRIGGER           "tcp/cloud_hmi/msg_0502"//Msg0502
#define CLOUD_SEND_HMI_UPLOAD_RESULT        "tcp/cloud/msg_0F0B"    //Msg0F0B
#define CLOUD_SEND_CGU_VERSION              "tcp/cloud/cgu_version" //MsgCguVersion
#define CLOUD_SEND_DEV_NAME                 "tcp/cloud/msg_1040"    //Msg1040
#define CLOUD_SEND_RES_TASK_FILE            "tcp/cloud/msg_8F0C"    //Msg8F0C
#define CLOUD_SEND_TASKFILE_TO_MAPENGINE    "tcp/cloud/msg_taskfileOK"    //Message
#define CLOUD_SEND_RES_MOVEAUTHORITY        "tcp/cloud/msg_8F0F"    //Msg8F0F
#define CLOUD_SEND_RES_REGIONALBOUNDARY     "tcp/cloud/msg_8F10"    //Msg8F10
#define CLOUD_SEND_HMI_WARNING_STATUS       "tcp/cloud/warnstat"    //MsgVechWarnStat

// cgu_protocol.fbs


#define TOPIC_DECISION_MSG0503              "tcp/decision/msg_0503"             //Msg0503

// can_info.fbs
#define TOPIC_CAN_WIRE_CONTROL_FAULT        "tcp/can/wire_control_fault"        //WireControlVehicleFault
#define TOPIC_CAN_WIRE_CONTROL_MOVATION     "tcp/can/wire_control_movation"     //WireControlVehicleMovation
#define TOPIC_CAN_WIRE_CONTROL_STATUS       "tcp/can/wire_control_status"       //WireControlVehicleStatus
#define TOPIC_CAN_WIRE_CONTROL_VCU_VER      "tcp/can/wire_control_vcu_ver"      //WireControlVcuVersion
#define TOPIC_CAN_DEV_STATUS                "tcp/can/can_about_dev_status"      //CanAboutDevStatus

// version.fbs
#define TOPIC_VERSION_VCU_REQ               "tcp/version/vcu/req"               //request for VCU Version
#define TOPIC_VERSION_VCU                   "tcp/version/vcu"                   //MsgVersion

#define TOPIC_VERSION_ICU_REQ               "tcp/version/icu/req"               //request for ICU Version
#define TOPIC_VERSION_ICU                   "tcp/version/icu"                   //MsgVersion

#define TOPIC_VERSION_CCU_REQ               "tcp/version/ccu/req"               //request for CCU Version
#define TOPIC_VERSION_CCU                   "tcp/version/ccu"                   //MsgVersion

// control.fbs
#define TOPIC_CONTROL_IMPLEMENT             "tcp/control/control_implement"     //ControlOutputForImplement
#define TOPIC_CONTROL_DECISION              "tcp/control/control_decision"      //ControlOutputForDecision

//decision.fbs
#define TOPIC_DECISION_OUTPUT               "tcp/decision/output"               //Output
#define TOPIC_DECISION_VEHICLE_INFO         "tcp/decision/vehicle_info"         //RoadTaskVehicleInfo

#define TOPIC_DECISION_STARTUP_CMD          "tcp/decision/msg_1030"             //Msg1030
#define TOPIC_DECISION_SHUTDOWN_CMD         "tcp/decision/msg_1050"             //Msg1050
#define TOPIC_DECISION_MSG2062              "tcp/decision/msg_2062"             //Msg2062
// #define TOPIC_DECISION_DISPATCH_REQ         "tcp/decision/msg_0305"             //Msg0305 deleted
#define TOPIC_DECISION_MANUAL_DRV_REQ       "tcp/decision/msg_0404"             //Msg0404
#define TOPIC_DECISION_TASK_EVENT           "tcp/decision/msg_0500"             //Msg0500
#define TOPIC_DECISION_MSG0501              "tcp/decision/msg_0501"             //Msg0501

#define TOPIC_DECISION_FAULT_STATUS         "tcp/decision/msg_8F0E"             //Msg8F0E ccu -> HMI&&cloud
#define TOPIC_DECISION_TEST_RESULT          "tcp/decision/msg_8F1C"             //Msg8F1C


// prediction.fbs

#define TOPIC_PREDICTION_SENSE_RESULT       "tcp/prediction/sense_result"       //SenseResultInfo  // msg_0402

//diagnosis.fbs
#define TOPIC_DIAGNOSIS_FAULT_CODE          "tcp/diagnosis/fault_code"          //WarningOutput

//at.fbs
#define TOPIC_AT_CSQ                        "tcp/at/csq"                        //ATMsg

//hmi
#define TOPIC_HMI_MSG0F1C                   "tcp/hmi/Msg0F1C"                   //HMI触发测试任务Msg0F1C
#define TOPIC_HMI_MSG8403                   "tcp/hmi/Msg8403"                   //HMI发送给网关设置矿卡空满载状态Msg8403
#define TOPIC_HMI_MSG8F0B                   "tcp/hmi/Msg8F0B"                   //Hmi生成文件后，需要车上传平台，通过该命令将文件上传网关Msg8F0B
#define TOPIC_HMI_MSG0F01                   "tcp/hmi/Msg0F01"                   //HMI发送网关营运状态变更请求命令 Msg0F01
#define TOPIC_HMI_MSG0F10                   "tcp/hmi/Msg0F10"                   //HMI触发登录
#define TOPIC_HMI_MSG0F11                   "tcp/hmi/Msg0F11"                   //HMI触发登出
//新增hmi
#define TOPIC_HMI_MsgTx_0104                 "tcp/hmi/MsgTx_0104"                                  // 说明：服务器参数查询，消息体为空 适配：所有车辆
#define TOPIC_HMI_MsgTx_0F03                 "tcp/hmi/MsgTx_0F03"                                  // 说明：物料变更申请  适配：辅助车辆-电铲
#define TOPIC_HMI_MsgTx_0F09                 "tcp/hmi/MsgTx_0F09"                                  // 说明：行车报警 适配：矿卡/宽体车
#define TOPIC_HMI_MsgTx_8305                 "tcp/hmi/MsgTx_8305"                                // 说明：作业场调度控制命令上报 适配：辅助车辆-电铲
#define TOPIC_HMI_MsgTx_8B02                 "tcp/hmi/MsgTx_8B02"                                 // 说明：发送digger就位命令 适配：辅助车辆-推土机digger
#define TOPIC_HMI_MsgTx_8B07_list            "tcp/hmi/MsgTx_8B07_list"                                 // 装载区停靠位位置上报：分组信息
#define TOPIC_HMI_MsgTx_8B07                 "tcp/hmi/MsgTx_8B07"                                // 说明：装载区停靠位位置上报 适配：辅助车辆-电铲
#define TOPIC_HMI_MsgTx_8B08_list            "tcp/hmi/MsgTx_8B08_list"                                // 装载区停靠位状态操作指令上报；分组信息
#define TOPIC_HMI_MsgTx_8B08                 "tcp/hmi/MsgTx_8B08"                                // 说明：装载区停靠位状态操作指令上报 适配：辅助车辆-电铲
#define TOPIC_HMI_MsgTx_0002                 "tcp/hmi/MsgTx_0002"//HMI发出的心跳 5s一包 CGU订阅判断连接状态
//add hmi topic to rcd
#define TOPIC_HMI_TO_RCD                     "tcp/hmi/MsgToRcd"

//rsu
#define TOPIC_RSU_SEND_TO_CLOUD             "ipc/rsu/send_to_cloud"
#define TOPIC_RSU_SEND_TO_V2X               "ipc/rsu/send_to_v2x"

//huawei
#define TOPIC_HW_REPORT_RECV_V2X_MSG        "ipc/huawei/report_recv_v2x_msg"

//rcd
#define TOPIC_RCD_REQ_REMOTE_CONTROL        "tcp/rcd/req_remote_control"


/******************************* req-rep *******************************/

//AT topic for req-rep
#define TOPIC_AT_REQ_REP_MSG                    "at_req_rep_msg"
//call function name define
#define FUNC_AT_GET_IMEI                        "getImei"
#define FUNC_AT_GET_ICCID                       "getIccid"

//v2x topic for req-rep
#define TOPIC_V2X_REQ_REP_MSG                   "v2x_req_rep_msg"
//call function name define
#define FUNC_V2X_SEND_RTK_DATA                  "V2xSendRTKData"
#define FUNC_V2X_SEND_RSU_CLOUD_DATA            "V2xSendRsuCloudData_v2x"
#define FUNC_V2X_LINK_STATUS                    "LinkStatus"

//cloud topic for req-rep
#define TOPIC_CLOUD_REQ_REP_MSG                 "cloud_req_rep_msg"
//call function name define
#define FUNC_CLOUD_GET_LOCAL_NAME               "GetLocalName"
#define FUNC_CLOUD_SEND_V2X_MSG                 "CloudSendV2xMsg"
#define FUNC_CLOUD_GET_ALL_DEV_NAME             "GetAllDevName"//FuncMsg -> FuncCloudGetAllDevName
#define FUNC_CLOUD_GET_CGU_VERSION              "GetCGUVersion"//FuncMsg -> MsgCguVersion

//rsu topic for req-rep
#define TOPIC_RSU_REQ_REP_MSG                   "rsu_req_rep_msg"
#define FUNC_RSU_CLOUD_SEND_RSU_V2X_DATA        "CloudSendRsuV2xData"
#define FUNC_RSU_V2X_SEND_RSU_CLOUD_DATA        "V2xSendRsuCloudData_rsu"
#define FUNC_RSU_LINK_STATUS                    "LinkStatus"

//gnssRtk topic req-rep
#define TOPIC_GNSSRTK_REQ_REP_MSG               "gnssrtk_req_rep_msg"
#define FUNC_GNSSRTK_LINK_STATUS                "LinkStatus"

//huawei topic req-rep
#define TOPIC_HUAWEI_REQ_REP_MSG                "huawei_req_rep_msg"
#define FUNC_HW_SEND_MSG_TO_HUAWEI              "SendV2XMsgToHuawei"

/******************************* req-rep end *******************************/


//map_info.fbs
#define TOPIC_MAP_ROADINFO_INIT              "ipc/map/roadinfo/init"
#define TOPIC_MAP_LANEINFO_INIT              "ipc/map/laneinfo/init"
#define TOPIC_MAP_BOUNDARYINFO_INIT          "ipc/map/boundaryinfo/init"

#define TOPIC_MAP_ROADINFO_UPDATE            "ipc/map/roadinfo/update"
#define TOPIC_MAP_LANEINFO_UPDATE            "ipc/map/laneinfo/update"
#define TOPIC_MAP_BOUNDARYINFO_UPDATE        "ipc/map/boundaryinfo/update"

#define TOPIC_MAP_ROADINFO_RESPONSE          "ipc/map/roadinfo/response"
#define TOPIC_MAP_LANEINFO_RESPONSE          "ipc/map/laneinfo/response"
#define TOPIC_MAP_BOUNDARYINFO_RESPONSE      "ipc/map/boundaryinfo/response"

#define TOPIC_MAP_TASK          			 "ipc/map/task"

#define TOPIC_MAP_ROADINFO_APPLY             "ipc/map/roadinfo/apply"
#define TOPIC_MAP_LANEINFO_APPLY             "ipc/map/laneinfo/apply"
#define TOPIC_MAP_BOUNDARYINFO_APPLY         "ipc/map/boundaryinfo/apply"

/***********************RCD驾驶舱*************************/
#define ZMQ_TOPIC_ALL ""

#define ZMQ_TOPIC_CONTROL_DATA                "ControlData"
#define ZMQ_TOPIC_CONTROL_DEVLINKSTATUS       "ControlDevLinkStatus"
#define ZMQ_TOPIC_BUTTON                      "ControlButton"
#define ZMQ_TOPIC_CONTROL_LIGHT               "ControlLight"

#define ZMQ_TOPIC_REQ_REMOTE_CONTROL          "ReqRemoteControl"
#define ZMQ_TOPIC_RESP_REMOTE_CONTROL         "RespRemoteControl"

#define ZMQ_TOPIC_VEHICLE_STATUS              "VehicleStatus"
#define ZMQ_TOPIC_VEHICLE_RAWDATA             "VehicleRawData"

#define ZMA_TOPIC_VEHICLE_DELAY               "Ping"

#define ZMQ_TOPIC_REPORT_HMI_MSG              "HmiMsg"
#define ZMQ_TOPIC_REQ_SEND_HMIMSG             "ReqSendHmiMsg"

#endif  // __TOPIC_H_
