#ifndef SYSTEM_H
#define SYSTEM_H

#include <QObject>
#include <QMetaType>
#include <QDateTime>

#define SOFT_VERSION    ("V01.00.00.0001")

#ifndef INT_INVALID
#define INT_INVALID (-1) //无效值
#endif

#ifndef STR_INVALID
#define STR_INVALID ("") //无效值
#endif

typedef QList<uint> ModuleDir;
Q_DECLARE_METATYPE(ModuleDir);



/*********************地图*********************/

#include <QObject>
#include <QMetaType>
#include <QDateTime>

#define SOFT_VERSION    ("V01.00.00.0001")

#ifndef INT_INVALID
#define INT_INVALID (-1) //无效值
#endif

#ifndef STR_INVALID
#define STR_INVALID ("") //无效值
#endif
//定义窗口和功能类的名称
#define NAME_message_Widget          "message_Widget"
#define NAME_moreMessage_Widget          "more_message_Widget"
#define NAME_title_TitleInfoWidget      "title_TitleInfoWidget"
#define NAME_info_MineInfoWidget        "info_MineInfoWidget"
#define NAME_info_AuxInfoWidget         "info_AuxInfoWidget"
#define NAME_map_MapWidget              "map_MapWidget"
#define NAME_opt_OptWidget              "opt_OptWidget"
#define NAME_main_ScreenSaveWidget      "main_ScreenSaveWidget"
#define NAME_map_KeyBoardWidget         "map_KeyBoardWidget"
#define NAME_mineinfo_WarningBitWidget  "mineInfo_WarningBitWidget"

#define NAME_map_StopHeadingChoiceWidget            "map_StopHeadingChoiceWidget"
#define NAME_map_StopHeadingParaChoiceWidget        "map_StopHeadingParaChoiceWidget"
#define NAME_map_AbnormalChoiceWidget               "map_AbnormalChoiceWidget"
#define NAME_map_DelayChoiceWidget                  "map_DelayChoiceWidget"
#define NAME_map_FaultChoiceWidget                  "map_FaultChoiceWidget"
#define NAME_map_MaterChoiceWidget                  "map_MaterChoiceWidget"
#define NAME_map_WorkStateChoiceWidget              "map_WorkStateChoiceWidget"
#define NAME_free_MessageBoxFault                   "free_MessageBoxFault"
#define NAME_instance_StopListDeal                  "instanceStopListDeal"

#define NAME_tip_quickStopLabel                  "quickStopLabel"


//using ModuleDir = QList<int8_t>;

//enum ModulePosType//模块
//{
//    MD_NULL=-1,
//    MD_MainProcessor=0,
//    MD_TitlePos,
//    MD_InfoPos,
//    MD_MapPos,
//    MD_MessagePos,
//    MD_OptPos,
//    MD_WarnBitWgtPos,
//    MD_MessageBoxPos,
//    MD_AllScreen,
//    MD_instance,//单例类

//    MD_Max_Map
//};
//各窗口的内部命令
//enum KeyBoardCmdType
//{
//    KB_OK=0,

//    KB_Max
//};
//enum MainProcessorCmdType
//{
//    MP_SwitchWgt=0,
//    MP_NetCmd=1,
//    MP_Internal=2,
//    MP_Function=3,

//    MP_Max
//};

//enum AuxInfoWidgetCmdType
//{
//    AuxInfoWidget_updateStopPointNum=0,
//    AuxInfoWidget_updateUnloadNum=1,
//    AuxInfoWidget_updateCarType=2,
//    AuxInfoWidget_insertCar=3,
//    AuxInfoWidget_removeCar=4,
//    AuxInfoWidget_updateCardInfo=5,
//    AuxInfoWidget_allowIn_allowUnload=6,
//    AuxInfoWidget_LoadOverOut,
//    AuxInfoWidget_ClearList,
//    AuxInfoWidget_allowIn,
//    AuxInfoWidget_StartLoad,
//    AuxInfoWidget_allowLeave,
//    AuxInfoWidget_keyPress,
//    AuxInfoWidget_updateCardList,
//    AuxInfoWidget_clearCardList,

//    AuxInfoWidget__Max
//};
//enum MineInfoWidgetCmdType
//{
//    MineInfoWidget_UpdateFaultLevel=0,//

//    MineInfoWidget_Max
//};
//enum TitleInfoWidgetCmdType
//{
//    TitleInfoWidget_updateStatus=0,
//    TitleInfoWidget_updateName=1,
//    TitleInfoWidget_updateDestination=2,


//    TitleInfoWidget_Max
//};

//enum MessageWgtCmdType
//{
//    MessageWgt_UpdateText=0,
//    MessageWgt_Reszie=1,
//    MessageWgt_Show,
//    MessageWgt_ShowMessage,
//    MessageWgt_SoundLevelTip,


//    MessageWgt_Max
//};
//enum DelayChoicesWgtCmdType
//{
//    DelayChoicesWgt_ClearChoice=0,
//    DelayChoicesWgt_DelayChoice,

//    DelayChoicesWgt_Max
//};
//enum FaultChoicesWgtCmdType
//{
//    FaultChoicesWgt_ClearChoice=0,
//    FaultChoicesWgt_Confirm,

//    FaultChoicesWgt_Max
//};
//enum WorkStateChoiceWgtCmdType
//{
//    WorkStateChoiceWgt_ClearChoice=0,
//    WorkStateChoiceWgt_Confirm,

//    WorkStateWgt_Max
//};
//enum MaterChoiceWgtCmdType
//{
//    MaterChoiceWgt_ClearChoice=0,
//    MaterChoiceWgt_Confirm_Cancel=1,

//    MaterChoiceWgt_Max
//};
//enum AbnormalChoiceWgtCmdType
//{
//    AbnormalChoiceWgt_ClearChoice=0,
//    AbnormalChoiceWgt_Confirm_Cancel=1,
//    AbnormalChoiceWgt_updateCarType=2,
//    AbnormalChoiceWgt_updateCardInfo,
//    AbnormalChoiceWgt_selectRowNum,
//    AbnormalChoiceWgt_keyPress,


//    AbnormalChoiceWgt_Max
//};
//enum OptWgtCmdType
//{
//    OptWgt_MapACK=1,
//    OptWgt_SwitchPage=2,
//    OptWgt_unLoadRunWorkBtn=3,
//    OptWgt_updateAngle,
//    OptWgt_disableButton,
//    OptWgt_enableButton,

//    OptWgt_Max
//};
//enum MapWgtCmdType
//{
//    MapWgt_ShowTipLabel=0,
//    MapWgt_ShowAngleButton,
//    MapWgt_Init,

//    MapWgt_StartBoundaryCollection,
//    MapWgt_PauseBoundaryCollection,
//    MapWgt_ContinueToBoundaryCollection,
//    MapWgt_EndBoundaryCollection,
//    MapWgt_SaveBoundary,
//    MapWgt_SubmitBoundary,
//    Mapwgt_DiscardBoundary,

//    MapWgt_StartDumpCollection,
//    MapWgt_SaveDumpCollection,
//    MapWgt_DiscardDumpCollection,

//    MapWgt_ChooseParkingSpot,

//    MapWgt_DiggerReady,
//    MapWgt_UpdateStopInfo,
//    MapWgt_UpdateBoundaryDB,
//    MapWgt_ZoomIn,
//    MapWgt_ZoomOut,


//    MapWgt_Max
//};

//enum WarningBitWgtCmdType
//{
//    WarningBitWgt_ShowCmd=0,
//    WarningBitWgt_RemoveWarnBit,
//    WarningBitWgt_InsertWarnBit,
//    WarningBitWgt_ClearCmd,
//    WarningBitWgt_ClearError,


//    WarningBitWgt_Max
//};
//enum MessageBoxCmdType
//{
//    MessageBox_ErrorLevel=0,
//    MessageBox_ShowMessage,


//    MessageBox_Max
//};
/************************************************/
enum ModuleType
{
    MD_MainProcessor=0 ,       //主处理模块
    MD_IPCManager,            //进程通信管理模块
    MD_InteractiveManager,    //输入交互管理模块

    MD_LeftCamera,            //左摄像头界面
    MD_DriveCamera,           //驾驶室摄像头界面
    MD_CloudWeb,              //web界面

    MD_MainCamera,            //主摄像头界面
    MD_MainMonitor,           //主监控界面

    MD_RightCamera,           //右摄像头界面
    MD_VehicleInfo,           //车辆信息界面
    MD_AllInfo,               //所有信息界面
    MD_WarnInfo,              //警告信息界面

    //MD_FunDialog,

    MD_Max
};

// 屏幕功能区域类型
enum ScreenAreaType
{
    SA_MainScreen,          // 主屏幕区域
    SA_RightScreenL,        // 右屏幕左区域
    SA_RightScreenR,        // 右屏幕右区域
    SA_LeftScreenTopL,      // 左屏幕上左区域
    SA_LeftScreenTopR,      // 左屏幕上右区域
    SA_LeftScreenBottom,    // 左屏幕下区域
};

// 布局类型
enum LayoutType
{
    LT_Monitor=0,           //监控时布局
    LT_RemoteControl,       //遥控时布局

    LT_Max
};

//// 主处理模块支持的处理命令
enum MainProcessorCmdType
{
    MP_SwitchWgt=0,     //切换窗
    MP_SwitchLayout,    //切换布局
    MP_SoftKeyPress,    //软键盘按下
    MP_IpcForward,      //消息ipc进程转发
    MP_UpdateSysData,   //系统数据更新
    MP_Max_Map
};

// 各模块通用命令
enum CommonCmdType
{
    CC_KeyPress = 0xA1  //按键按下
};

// 主相机窗口命令类型
enum MainCameraWgtCmdType
{
    MC_PlayUrl,             //更新播放url
    MC_VehicleData,         //当前控制车辆设备信息
    MC_ThrottleBrakeWheel,  //当前控制车辆设备信息
    MC_Speed,               //当前控制车辆速度
    MC_Gear,                //当前控制车辆档位
    MC_VehicleDelay,        //当前控制车辆延迟
    MC_RemoteResule,        //遥控返回结果
    MC_Throttle,            //加速百分比
    MC_Brake,               //刹车
    MC_Wheel,               //方向盘转角
    MC_Other,               //扩展数据
    MC_Speed_Gear,          //车速与档位
    MC_Throttle_RingDila,   //加速踏板百分比+发动机转速
    MC_InputKey = CC_KeyPress   //输入按键
};

// 监控主界面命令
enum MainMonitorWgtCmdType
{
    MM_UpdateAll=0,         //更新界面
    MM_VehicleData,         //当前接入车辆设备信息
    MM_ThrottleBrakeWheel,  //当前接入车辆设备信息
    MM_Speed,               //当前接入车辆设备速度
    MM_Gear,                //当前接入车辆设备档位
    MM_VehicleStatus,       //当前接入车辆设备状态

    MM_InputKey = CC_KeyPress   //输入按键
};

enum RightCameraWgtCmdType
{
    RC_UpdateTitle=0,       //更新标题

    RC_InputKey = CC_KeyPress
};

enum LeftCameraWgtCmdType   //更新标题
{
    LC_UpdateTitle=0,


    LC_Max
};

enum DriveCameraWgtCmdType
{
    DC_UpdateUrl=0,


    DC_Max
};

enum CloudWebWgtCmdType
{
    CW_LoadNewUrl=0,


    CW_Max
};


enum VehicleInfoWgtCmdType
{
    VI_LoadNewUrl=0,
    VI_G2HmiInfo,

    VI_Max,
    VI_Oil
};

enum AllInfoWgtCmdType
{
    AI_LoadNewUrl=0,


    AI_Max
};

enum WarnInfoWgtCmdType
{
    WI_LoadNewUrl=0,


    WI_Max
};

// 进程通讯管理模块命令
enum IPCManagerCmdType
{
    IM_ReqRemoteStart=0,    //开始申请遥控
    IM_ReqRemoteEnd,        //结束遥控
    IM_ReqSendHmiMsg,       //发送HMI的消息命令包

    IM_Max
};

//交互管理支持的命令
enum InteractiveManagerCmdType
{
    IA_KeyPress=0,
    IA_SwitchLayout,
    IA_FocusToNextPage,      //更新焦点到右边焦点
    IA_FocusToPreviousPage,  //更新焦点到左边焦点
    IA_FocusToUpPage,        //更新焦点到上边焦点
    IA_FocusToDownPage,      //更新焦点到下边焦点

    IA_Max
};

enum ProcessType
{
    PT_All = 0x007,     // 非多进程运行
    PT_RightProc = 0x001,//右侧界面
    PT_MidProc = 0x002,//中间界面
    PT_LeftProc = 0x004,//左侧界面

    PT_Max
};


#define IMEI_LENGTH 15

class QSharedMemory;

// 系统功能类
class System
{

public:
    explicit System();
    ~System();

    typedef struct
    {
        uint8_t                  sysStatus;
        char                     vehicleImei[IMEI_LENGTH + 1];
        char                     none[4096 - (IMEI_LENGTH + 2)];
    } SharedDatas;

    static System           *getInstance();
    static void             delayMsecTime(int msecTime,int delayModel = 0);
    static ProcessType      belongTo(ModuleType mdType);
    static bool             messageFilter(ModuleType module);
    static bool             messageFilter(ModuleType module,QString &imei);
    static void             setCurrentVehicle(QString &imei);
    static QString          getCurrentVehicle();
    static void             setProcessType(ProcessType);
    static int              getProcessType();
    static SharedDatas*     getSharedDatas();


private:

    void                     createSharedMemory();
    void                     readSharedMemory();
    void                     writeSharedMemory();

    static System            *m_pInstance;
    SharedDatas              m_sysShareData;
    ProcessType              m_processType;
    QSharedMemory            *m_pSharedMemory;

};

#if defined(pSharedDatas)
#undef pSharedDatas
#endif
#define pSharedDatas System::getSharedDatas()


#endif // SYSTEM_H

