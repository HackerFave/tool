#include "dataSend.h"
#include <QDebug>
#include "define_zmqmodule.h"
#include <unistd.h>
#include <string>
using namespace std;
dataSend * dataSend::P_dataSend  = nullptr;
dataSend::dataSend()
{

    initPoint();
    qRegisterMetaType<uint32_t>("uint32_t&");
    qRegisterMetaType<uint32_t>("uint32_t");
#if 0
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_TCP_GNSS_GPS_INFO_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_TCP_GNSS_GPS_STATUS_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_IPC_MESSAGE_TEST_TOPIC_ENUM),
                           CallbackFunc);

    /*gnssServer*/
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_HUAWEI_GNSS_BROADCAST_DATA_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_GNSS_BROADCAST_DATA_ENUM),
                           CallbackFunc);
    /*voiceServer*/
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_HMI_SEND_VOICE_PLAY_ENUM),
                           CallbackFunc);

    // v2xServer
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_CV2X_COll_WARNING_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_CV2X_SEND_GNSS_RTK_DATA_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_CV2X_RECV_RSU_CLOUD_DATA_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_CV2X_AUX_RESP_JOB_SCJED_DATA_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_CV2X_TRUCK_REQ_JOB_SCHED_DATA_ENUM),
                           CallbackFunc);

    // can_info.fbs
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_CAN_WIRE_CONTROL_FAULT_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_CAN_WIRE_CONTROL_MOVATION_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_CAN_WIRE_CONTROL_STATUS_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_CAN_WIRE_CONTROL_VCU_VER_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_CAN_DEV_STATUS_ENUM),
                           CallbackFunc);

    // control.fbs
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_CONTROL_IMPLEMENT_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_CONTROL_DECISION_ENUM),
                           CallbackFunc);

    //devision.fbs
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_DECISION_OUTPUT_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_DECISION_VEHICLE_INFO_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_DECISION_FAULT_STATUS_ENUM),
                           CallbackFunc);

    //diagnosis.fbs
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_DIAGNOSIS_FAULT_CODE_ENUM),
                           CallbackFunc);
    //at.fbs
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_AT_CSQ_ENUM),
                           CallbackFunc);
    //hmi
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_HMI_MSG0F1C_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_HMI_MSG8403_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_HMI_MSG8F0B_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_HMI_MSG0F01_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_HMI_MSG0F10_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_HMI_MSG0F11_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_HMI_MsgTx_0104_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_HMI_MsgTx_0F03_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_HMI_MsgTx_0F09_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_HMI_MsgTx_8305_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_HMI_MsgTx_8B02_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_HMI_MsgTx_8B07_list_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_HMI_MsgTx_8B07_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_HMI_MsgTx_8B08_list_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_HMI_MsgTx_8B08_ENUM),
                           CallbackFunc);


    //Msg8F02

    _topicRecver.Subscribe(enumToOtherString(Topic_ID::CLOUD_SEND_V2X_RSU_DATA_TOPIC_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::CLOUD_SEND_REQ_WORK_STATUS_ACK_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::CLOUD_SEND_OPR_STATUS_CHANGE_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::CLOUD_SEND_MATER_CHANGE_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::CLOUD_SEND_RECV_V2X_BROAD_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::CLOUD_SEND_CHANGE_TEMP_TASK_FILE_ENUM),
                           CallbackFunc);


    _topicRecver.Subscribe(enumToOtherString(Topic_ID::CLOUD_SEND_DOWNLOAD_FILE_ENUM),
                           CallbackFunc);  //Msg8F0A
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::CLOUD_SEND_REMOTE_CONTROL_ENUM),
                           CallbackFunc);  //Msg8F09
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::CLOUD_SEND_DOCKING_GROUP_ENUM),
                           CallbackFunc);   //Msg8B01
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::CLOUD_SEND_WAITING_IN_LINE_ENUM),
                           CallbackFunc);    //Msg8B03
    //_topicRecver.Subscribe(enumToOtherString(Topic_ID::CLOUD_SEND_CLEAR_FAULT_ENUM),
    //                       CallbackFunc);  //Msg0F0E 0F0E这条HMI也会发
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::CLOUD_SEND_RESP_EXIT_ENUM),
                           CallbackFunc);    //Msg8105
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::CLOUD_SEND_RESP_SIGN_IN_ENUM),
                           CallbackFunc);    //Msg8B10
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::CLOUD_SEND_RESP_SIGN_OUT_ENUM),
                           CallbackFunc);    //Msg8B11
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::CLOUD_SEND_REQ_LOAD_JOB_ENUM),
                           CallbackFunc);     //Msg8307
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::CLOUD_SEND_REQ_UNLOAD_JOB_ENUM),
                           CallbackFunc);    //Msg8308
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::CLOUD_SEND_UPLOAD_TRIGGER_ENUM),
                           CallbackFunc);  //Msg0502
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::CLOUD_SEND_HMI_UPLOAD_RESULT_ENUM),
                           CallbackFunc);     //Msg0F0B
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::CLOUD_SEND_CGU_VERSION_ENUM),
                           CallbackFunc);  //MsgCguVersion
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::CLOUD_SEND_DEV_NAME_ENUM),
                           CallbackFunc);   //Msg1040

    _topicRecver.Subscribe(enumToOtherString(Topic_ID::FUNC_CLOUD_GET_CGU_VERSION_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::FUNC_CLOUD_GET_CGU_VERSION_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::FUNC_CLOUD_GET_CGU_VERSION_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::FUNC_CLOUD_GET_CGU_VERSION_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::FUNC_CLOUD_GET_CGU_VERSION_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::FUNC_CLOUD_GET_CGU_VERSION_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::FUNC_CLOUD_GET_CGU_VERSION_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::FUNC_CLOUD_GET_CGU_VERSION_ENUM),
                           CallbackFunc);
   #endif

    _topicRecver.Subscribe(enumToOtherString(Topic_ID::TOPIC_IPC_MESSAGE_TEST_TOPIC_ENUM),
                           CallbackFunc);
    /*************RCD************/
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::ZMQ_TOPIC_ALL_ENUM),
                           CallbackFunc);                                      /*""*/
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::ZMQ_TOPIC_CONTROL_DATA_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::ZMQ_TOPIC_CONTROL_DEVLINKSTATUS_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::ZMQ_TOPIC_BUTTON_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::ZMQ_TOPIC_CONTROL_LIGHT_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::ZMQ_TOPIC_REQ_REMOTE_CONTROL_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::ZMQ_TOPIC_RESP_REMOTE_CONTROL_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::ZMQ_TOPIC_VEHICLE_STATUS_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::ZMQ_TOPIC_VEHICLE_RAWDATA_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::ZMA_TOPIC_VEHICLE_DELAY_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::ZMQ_TOPIC_REPORT_HMI_MSG_ENUM),
                           CallbackFunc);
    _topicRecver.Subscribe(enumToOtherString(Topic_ID::ZMQ_TOPIC_REQ_SEND_HMIMSG_ENUM),
                           CallbackFunc);


}

//初始化指针
Q_INVOKABLE void dataSend::initPoint()
{

    if(P_dataSend){
        delete P_dataSend;
        P_dataSend = this;
    }
    P_dataSend = this;
}

void dataSend::emitSignal(const void *f,int id, const uint32_t &len)
{    
    emit this->signalSubTopicDataChild(f,id,len);
}

void dataSend::CallbackFunc(const char* topic1, const void*payload, uint32_t len)//数据接受函数、、
{
    int id = enumFromString(std::string(topic1));

    if(id==0){
        std::string t_error = std::string(topic1)+"远端数据获取失败";
        P_dataSend->emitSignal(&t_error,0,len);
        return;
    }
#if 1
    auto msg =GetMessage(payload);
    if(msg->data()&&msg->data()->Data()){
        uint8_t* data = new uint8_t[len];
        memcpy(data, msg->data()->Data(), len);
        P_dataSend->emitSignal(data,id,len);
    }
#endif
//      P_dataSend->emitSignal(payload,id,len);
}
