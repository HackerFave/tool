#ifndef WARNNING_H
#define WARNNING_H
#include <vector>
#include <map>
#include <string>
#include <bitset>

using namespace std;

enum WarningBit
{
    mainPower = 0,                      //主电源模块发生故障
    bakPower = 1,                       //备用电源模块发生故障
    perception = 2,                     //感知模块发生故障
    gnss = 3,                           //GNSS模块发生故障
    v2n = 4,                            //V2N无线网络通讯模块发生故障
    v2x = 5,                            //V2V无线网络通讯模块发生故障
    braking = 6,                        //机械制动力异常
    master = 7,                         //主控模块发生故障
    tpms = 8,                           //胎压监测设备发生故障
    fuelMonitor = 9,                    //油量监测设备发生故障
    hmi = 10,                           //HMI 模块发送故障
    gnssAge = 11,                       //Gnss 龄期超时
    gnssVariance = 12,                  //Gnss 方差超出范围
    gateWayDownloadMapFailed = 13,      //网关下载地图文件失败
    trafficJam = 14,                    //道路堵塞告警
    brakingHardware = 15,               //机械制动硬件故障
    tirePressure = 16,                  //车辆胎压异常报警
    fuel = 17,                          //车辆油量异常报警
    vehcile = 18,                       //车辆故障码
    gnssSignal = 19,                    //惯导漂移故障
    rssi = 20,                          //无线通信信号强度异常告警
    rtkStatus = 21,                     //差分状态异常
    imuStatus = 22,                     //IMU状态异常
    gpsStatus = 23,                     //GPS状态异常
    stopEmergncy = 24,                  //故障紧急停车
    stopObstacle = 25,                  //障碍物紧急停车
    stopManual = 26,                    //人工紧急停车
    collwarning = 27,                   //碰撞预警
    rollWarning = 28,                   //侧翻预警
    yawWarning = 29,                    //偏航预警
    liftServo = 30,                     //举升舵机故障
    liftingState = 31,                  //举升状态异常
    gear = 32,                          //档位故障
    throttleVoltageOutput = 33,         //油门电压输出故障
    throttleDriving = 34,               //油门驱动力异常
    electricBrakeVoltageOutput = 35,    //电制动电压输出故障
    electricBrake = 36,                 //电制动异常
    loadBrakeRelay = 37,                //装载制动继电器故障
    loadBrakeMechanical = 38,           //装载制动机械故障
    turn = 39,                          //转向故障
    leverSensor = 40,                   //拉杆传感器故障
    canControl = 41,                    //线控控制器故障
    frontRadar = 42,                    //前毫米波故障
    frontLidar = 43,                    //前激光雷达故障
    rearRadar = 44,                     //后毫米波故障
    rearLidar = 45,                     //后激光雷达故障
    gatewayCommunication = 46,          //网关通信异常
    safeMcuCommunication = 47,          //safemcu通信异常
    vcuCommunication = 48,              //VCU通信异常
    dynamicRoad = 49,                   //装卸载点动态规划故障
    masterReboot = 50,                  //主控系统重启故障
    gatewayReboot = 51,                 //网关系统重启故障
    hmiReboot = 52,                     //HMI系统重启故障
    taskFileFormat = 53,                //任务文件格式错误
    taskFileNoKey = 54,                 //任务文件无法找到关键点
    notFoundLpx = 55,                   //找不到对应路径文件
    lpxFormat = 56,                     //路径文件格式错误
    calibrationFileList = 57,           //标定文件列表读取失败
    gatewayDownLoadTaskFileFailed = 58, //网关任务文件下载失败
    masterDownLoadTaskFileFailed = 59,  //主控任务文件下载失败
    speedFast = 60,                     //车速过快报警
    speedSlow = 61,                     //车速过慢报警
    trolley = 62,                       //溜车故障
    remoteControlVaild = 63,             //遥控器无效故障
    parkFail = 64,                      //泊车停靠失败
    steerWheelStatus = 65,              //方向盘状态故障
    noStatellliteSignal =66,            //无卫星信号
    engineFailure = 72,                 //发动机故障
    transmissionFailure = 73,           //变速箱故障
    EBSFailure = 74,                    //ebs故障
    EPSFailure =75,                     //eps故障
    headTipLightShortOut = 76,            //左前或者右前转向灯短路
    headLightShortOut = 77,             //近光灯或远光灯短路
    fogLightShortOut = 78,              //旋转报警灯或前雾灯短路
    trumpetShortOut = 79,                       //气喇叭或倒车灯或倒车喇叭短路
    workLightError = 80,                //前或后工作灯短路
    stopLightShortOut = 81,                     //制动灯或侧向灯短路
    positionLightShortOut = 82,         //前或后位置灯短路
    tailTipLightShortOut = 83,          //左后或右后转向灯短路
    wiperShortOut = 84,                 //雨刮器或气压传感器供电短路或辅助散热器1/2短路
    magneticValve = 85,                 //轴差电磁阀短路
    dryerShortOut = 86,                 //干燥器或水寒宝供电短路或排气制动输出短路
    filterClog = 87,                    //转向油滤堵塞或者空滤堵塞报警
};

const std::map<int, std::pair<std::string, std::string> > warningMap = {
    /*报警位序号,文字提示,语音播放路径*/
    {mainPower, {"主电源模块发生故障", "mainPowerFault.wav"}},
    {bakPower, {"备用电源模块发生故障", "mainPowerFault.wav"}},
    {liftServo, {"举升舵机故障", ""}},
    {liftingState, {"举升状态异常", "liftingState.wav"}},
    {gear, {"档位故障", "gear.wav"}},
    {throttleVoltageOutput, {"油门故障", "actionAbnormal.wav"}},
    {throttleDriving, {"油门故障", "actionAbnormal.wav"}},
    {electricBrakeVoltageOutput, {"电制动故障", "actionAbnormal.wav"}},
    {electricBrake, {"电制动故障", "actionAbnormal.wav"}},
    {loadBrakeRelay, {"装载制动故障", "actionAbnormal.wav"}},
    {loadBrakeMechanical, {"装载制动故障", "actionAbnormal.wav"}},
    {braking, {"机械制动故障", "actionAbnormal.wav"}},
    {brakingHardware, {"机械制动故障", "actionAbnormal.wav"}},
    {turn, {"转向故障", "actionAbnormal.wav"}},
    {leverSensor, {"拉杆传感器故障", "controlAbnormal.wav"}},
    {canControl, {"线控控制器故障", "controlAbnormal.wav"}},
    {rtkStatus, {"差分状态异常", "gnssAbnormal.wav"}},
    {gnssSignal, {"惯导漂移故障", "gnssAbnormal.wav"}},
    {gpsStatus, {"GPS状态异常", "gnssAbnormal.wav"}},
    {imuStatus, {"IMU状态异常", "gnssData.wav"}},
    {frontRadar, {"前毫米波故障", "sensorAbnormal.wav"}},
    {frontLidar, {"前激光雷达故障", "sensorAbnormal.wav"}},
    {rearRadar, {"后毫米波故障", "sensorAbnormal.wav"}},
    {rearLidar, {"后激光雷达故障", "sensorAbnormal.wav"}},
    {perception, {"感知模块故障", "sensorAbnormal.wav"}},
    {v2x, {"V2V通信异常", "sensorAbnormal.wav"}},
    {v2n, {"V2N通信异常", "sensorAbnormal.wav"}},
    {gatewayCommunication, {"网关通讯异常", "controlAbnormal.wav"}},
    {safeMcuCommunication, {"safemcu通讯异常", "controlAbnormal.wav"}},
    {master, {"主控通讯异常", "controlAbnormal.wav"}},
    {hmi, {"HMI通讯异常", "controlAbnormal.wav"}},
    {vcuCommunication, {"VCU通讯异常", "controlAbnormal.wav"}},
    {dynamicRoad, {"路径规划异常", "dynamicRoad.wav"}},
    {trafficJam, {"道路堵塞故障", "trafficJam.wav"}},
    {collwarning, {"前向碰撞故障", "trafficJam.wav"}},
    {masterReboot, {"系统重启故障", "initFault.wav"}},
    {gatewayReboot, {"系统重启故障", "initFault.wav"}},
    {hmiReboot, {"系统重启故障", "initFault.wav"}},
    {taskFileFormat, {"任务文件格式错误", "initFault.wav"}},
    {taskFileNoKey, {"无法找到关键点", "initFault.wav"}},
    {notFoundLpx, {"无法找到路径文件", "initFault.wav"}},
    {lpxFormat, {"路径文件格式错误", "initFault.wav"}},
    {calibrationFileList, {"无法读取标定文件", "initFault.wav"}},
    {yawWarning, {"偏离路径", "yawWarning.wav"}},
    {speedFast, {"车速过快", "speed.wav"}},
    {speedSlow, {"车速过慢", "speed.wav"}},
    {trolley, {"溜车故障", "speed.wav"}},
    {rollWarning, {"侧翻预警", "./wav"}},
    {parkFail,{"泊车停靠失败",""}},
    {steerWheelStatus,{"方向盘状态故障",""}},
    {noStatellliteSignal,{"无卫星信号",""}},
    {engineFailure,{"发动机故障",""}},
    {transmissionFailure,{"变速箱故障",""}},
    {EBSFailure,{"ebs故障",""}},
    {EPSFailure,{"eps故障",""}},
    {headTipLightShortOut,{"左前或者右前转向灯短路",""}},
    {headLightShortOut,{"近光灯或远光灯短路",""}},
    {fogLightShortOut,{"旋转报警灯或前雾灯短路",""}},
    {trumpetShortOut,{"气喇叭或倒车灯或倒车喇叭短路",""}},
    {workLightError,{"前或后工作灯短路",""}},
    {stopLightShortOut,{"制动灯或侧向灯短路",""}},
    {positionLightShortOut,{"前或后位置灯短路",""}},
    {tailTipLightShortOut,{"左后或右后转向灯短路",""}},
    {wiperShortOut,{"雨刮器或气压传感器供电短路或辅助散热器1/2短路",""}},
    {magneticValve,{"轴差电磁阀短路",""}},
    {dryerShortOut,{"干燥器或水寒宝供电短路或排气制动输出短路",""}},
    {filterClog,{"转向油滤堵塞或者空滤堵塞报警",""}}
};

#define BITSET_LENGTH 8
#define BITSET_BYTE_LEN 16

class warnning
{
public:
    warnning();

    std::vector<std::bitset<BITSET_LENGTH>> warningVector =
        std::vector<std::bitset<BITSET_LENGTH>>(BITSET_BYTE_LEN);
    uint32_t status = 0;

private:
    int _getWarningBit(const std::bitset<BITSET_LENGTH> &_warn, std::vector<size_t> &outResult, int offset);

public:
    std::vector<size_t> getWarningBit();

    bool testWarning(enum WarningBit _bit);

    int loadFromRawData(const char *rawData, int len);
};

#endif // WARNNING_H
