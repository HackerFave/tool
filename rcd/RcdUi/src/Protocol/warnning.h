#ifndef WARNNING_H
#define WARNNING_H
#include <vector>
#include <map>
#include <string>
#include <bitset>

#define GOLOBAL_ERROR_TYPE 2
using namespace std;
#if(GOLOBAL_ERROR_TYPE==1)
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
    noStatellliteSignal = 64,           //无卫星信号
    steerWheelStatus = 65,              //方向盘状态故障
    frontLeftMmRadarOffline =66,            //左前毫米波掉线
    frontRightMmRadarOffline =67,            //右前毫米波掉线
    frontLeftRadarOffline =68,            //左前雷达掉线
    frontRightRadarOffline =69,            //右前雷达掉线
    unloadTaskWhileEmpty =70,            //空载时收到卸载任务
    loadTaskWhileFull =71,            //满载时收到装载任务
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
    {noStatellliteSignal,{"无卫星信号",""}},
    {steerWheelStatus,{"方向盘状态故障",""}},
    {frontLeftMmRadarOffline,{"左前毫米波掉线",""}},
    {frontRightMmRadarOffline,{"右前毫米波掉线",""}},
    {frontLeftRadarOffline,{"左前雷达掉线",""}},
    {frontRightRadarOffline,{"右前雷达掉线",""}},
    {unloadTaskWhileEmpty,{"空载时收到卸载任务",""}},
    {loadTaskWhileFull,{"满载时收到装载任务",""}},
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

#elif(GOLOBAL_ERROR_TYPE==2)
enum WarningBit
{
    frontMiddleRadarFault = 0,                //前中毫米波故障
    frontLidarFault = 1,                      //前激光雷达故障
    rearRadarFault = 2,                       //后毫米波故障
    rearLidarFault = 3,                       //后激光雷达故障
    frontLeftRadarDisconnetion= 4,            //前左毫米波掉线
    frontRightRadarDisconnetion = 5,          //前右毫米波掉线
    leftLidarDisconnetion = 6,                //左激光雷达掉线
    rightLidarDisconnetion = 7,               //右激光雷达掉线
    perceptualSystemFault = 8,                //感知系统模块故障
    rtkStatusAbnormal = 9,                    //差分状态异常
    imuStatusAbnormal = 10,                   //IMU状态异常
    gpsStatusAbnormal = 11,                   //GPS状态异常
    noSatelliteSignal = 12,                   //无卫星信号
    gnssAge = 13,                             //Gnss 龄期超时
    gnssVariance = 14,                        //gnss纬度 经度方差超出范围
    V2VDisconnection = 15,                    //v2v掉线
    V2NDisconnection = 16,                    //v2n掉线
    VCUCommunicationAbnormal = 17,	          //VCU通信异常
    CGUCommunicationAbnormal = 18,            //CGU通信异常
    SCUCommunicationAbnormal = 19,            //SCU通信异常
    CCUCommunicationAbnormal = 20,			  //CCU通信异常
    HMICommunicationAbnormal = 21,            //HMI通信异常
    CGURebootFault = 22, 					  //CGU重启故障
    CCURebootFault = 23,                      //CCU重启故障
    HMIRebootFault = 24,					  //HMI重启故障
    mainPowerFault = 25,                      //主电源模块发生故障
    bakPowerFault = 26,                       //备用电源模块发生故障
    dynamicRoad = 32,                         //装卸载点动态规划故障
    dockingFailure = 33,                      //停靠失败
    taskFileFormat = 34,                      //任务文件格式错误
    taskFileNoKey = 35,                       //任务文件无法找到关键点
    calibrationFileList = 36,                 //标定文件列表读取失败
    liftingStateAbnormal = 37,                //举升状态异常
    trafficJam = 38,                          //道路堵塞告警
    emptyStateReceivedUnloadTask = 39,        //空载状态下接收到卸载任务文件
    fullStateReceivedLoadTask = 40,           //满载状态下接收到装载任务文件
    gatewayDownLoadMapFailed = 41,            //网关地图文件下载失败
    gatewayDownLoadTaskFileFailed = 42,       //网关任务文件下载失败
    masterDownLoadTaskFileFailed = 43,        //主控任务文件下载失败
    rssiWarning = 44,                         //无线通信信号强度异常告警
    fullLoadCloseCar = 45,                    //满载状态收车任务
    inertialNavigationDriftFault = 56,        //惯导漂移故障
    gearFault = 57,                           //档位故障
    steeringFault = 58,                       //转向故障
    trolleyFault = 59,                        //溜车故障
    roadDeviation = 60,                       //道路偏离
    collwarning = 61,                         //碰撞预警
    rollWarning = 62,                         //侧翻预警
    speedFastWarning = 63,                    //车速过快报警
    speedSlowWarning = 64,                    //车速过慢报警
    throttleDrivingAbnormal = 65,             //油门驱动力异常
    electricBrakingAbnormal = 66,             //电制动异常/制动力异常
    mechanicalBrakingAbnormal = 67,           //机械制动力异常
    loadingBrakingWithExecutionDiff = 68,     //装载制动命令与执行不符合
    enginePrimaryFault = 72,                  //发动机故障(一级)
    engineSecondaryFault = 73,               //发动机故障(二级)
    engineThreeFault = 74,                   //发动机故障(三级)
    transmissionPrimaryFault = 75,            //变速箱故障(一级)
    transmissionSecondaryFault = 76,          //变速箱故障(二级)
    transmissionThreeFault = 77,              //变速箱故障(三级)
    EBSFault = 78,                            //EBS故障(一级)
    EPSFault = 79,                            //EPS故障(一级)
    electricDriveSpeedLimit1 = 80,            //电驱动系统类限速行驶1
    electricDriveSpeedLimit2 = 81,            //电驱动系统类限速行驶2、坡行
    electricDriveSpeedLimit3 = 82,            //电驱动系统类限速行驶3
    electricDriveEngineLift  = 83,            //电驱动系统类发动机升速
    electricDriveSystemEvents  = 84,          //电驱动系统类系统事件
    noTractio = 85,                           //无牵引
    WithoutElectricityGoSlow = 86,            //无电缓行
    liftingHardwareFault = 87,                //举升硬件故障
    suspensionCylinderPrimaryFault = 88,      //悬缸故障(一级)
    suspensionCylinderThreeFault = 89,        //悬缸故障(三级)
    tirePrimaryFault = 90,                    //轮胎故障(一级)
    tireThreeFault = 91,                      //轮胎故障(三级)
    tirePressureWarning = 92,                 //车辆胎压异常报警
    LowFuelOilLevel = 93,                     //燃油油位低
    automaticWeighingFault = 94,              //自动称重故障
    selfExtinguished = 95,                   //自主灭火系统
    turnLightShort = 96,                      //左前或右前或左后或右后转向灯短路
    nearLightOrHighBeamShort = 97,           //近光灯或远光灯短路
    circuitAlarmLampOrFrontFogLampShort = 98, //旋转报警灯或前雾灯短路
    gasHornOrReverseLampOrReversingHornShort = 99, //气喇叭或倒车灯或倒车喇叭
    workingLightShort = 100,                  //前或后工作灯短路
    brakeOrSideLightShort = 101,              //制动灯或侧向灯短路
    positionLightShort = 102,                 //前或后位置灯短路
    WiperOrAirPressureOrAuxiliaryShort=103,   //雨刮器或气压传感器供电短路或辅助散热器1/2短路
    shaftSolenoidShort = 104,                 //轴差电磁阀短路
    drierOrWaterHanbaoOrExhaustShort = 105,   //干燥器或水寒宝供电短路或排气制动输出短路
    steeringOilOrEmptyFilterJamAlarm = 106,   //转向油滤堵塞或者空滤堵塞报警
    handBrakeSolenoidValveShort = 107,        //手制动电磁阀短路
    mechanicalBrakeHardwareFault = 108,       //机械制动硬件故障
    leverSensorFault = 109                    //拉杆传感器故障
};

const std::map<int, std::pair<std::string, std::string>> warningMap = {
    /*报警位序号,文字提示,语音播放路径*/
    {frontMiddleRadarFault, {"前中毫米波故障", "systemFaultDeal.wav"}},
    {frontLidarFault, {"前激光雷达故障", "systemFaultDeal.wav"}},
    {rearRadarFault, {"后毫米波故障", "systemFaultDeal.wav"}},
    {rearLidarFault, {"后激光雷达故障", "systemFaultDeal.wav"}},
    {frontLeftRadarDisconnetion, {"前左毫米波通信中断", "systemFaultDeal.wav"}},
    {frontRightRadarDisconnetion, {"前右毫米波通信中断", "systemFaultDeal.wav"}},
    {leftLidarDisconnetion, {"左激光雷达通信中断", "systemFaultDeal.wav"}},
    {rightLidarDisconnetion, {"右激光雷达通信中断", "systemFaultDeal.wav"}},
    {perceptualSystemFault, {"感知系统模块故障", "takeOver.wav"}},
    {rtkStatusAbnormal, {"差分定位状态异常", "takeOver.wav"}},
    {imuStatusAbnormal, {"IMU状态异常", "takeOver.wav"}},
    {gpsStatusAbnormal, {"GPS状态异常", "takeOver.wav"}},
    {noSatelliteSignal, {"GPS状态异常", "takeOver.wav"}},
    {gnssAge, {"系统导航状态异常", "takeOver.wav"}},
    {gnssVariance, {"系统导航状态异常", "takeOver.wav"}},
    {V2NDisconnection, {"与平台通信中断", "takeOver.wav"}},
    {VCUCommunicationAbnormal, {"控制单元通信异常", "takeOver.wav"}},
    {CGUCommunicationAbnormal, {"网关通信异常", "systemFaultDeal.wav"}},
    {SCUCommunicationAbnormal, {"SCU通信异常", "takeOver.wav"}},
    {CCUCommunicationAbnormal, {"主控通信异常", "takeOver.wav"}},
    {HMICommunicationAbnormal, {"HMI通信异常", "systemFaultDeal.wav"}},
    {CGURebootFault, {"网关重启故障", "systemFaultDeal.wav"}},
    {CCURebootFault, {"主控重启故障", "takeOver.wav"}},
    {HMIRebootFault, {"HMI重启故障", "systemFaultDeal.wav"}},
    {mainPowerFault, {"主电源故障", "takeOver.wav"}},
    {bakPowerFault, {"备用电源故障", "takeOver.wav"}},
    {dynamicRoad, {"路径规划失败", "pathPlanningFailure.wav"}},
    {dockingFailure, {"停靠失败", "dockingFailure.wav"}},
    {taskFileFormat, {"任务文件异常", "taskFileAbnormal.wav"}},
    {taskFileNoKey, {"路径文件异常", "pathFileAbnormal.wav"}},
    {calibrationFileList, {"标定文件异常", "calibrationFileReadFailure.wav"}},
    {liftingStateAbnormal, {"举升状态异常", "takeOver.wav"}},
    {trafficJam, {"道路堵塞异常", "trafficJam.wav"}},
    {emptyStateReceivedUnloadTask, {"载货状态异常", "loadStatusAbnormal.wav"}},
    {fullStateReceivedLoadTask, {"载货状态异常", "loadStatusAbnormal.wav"}},
    {gatewayDownLoadMapFailed, {"地图文件下载失败", "downLoadMapFailed.wav"}},
    {gatewayDownLoadTaskFileFailed, {"任务文件下载失败", "downLoadTaskFileFailed.wav"}},
    {masterDownLoadTaskFileFailed, {"任务文件下载失败", "downLoadTaskFileFailed.wav"}},
    {rssiWarning, {"无线通信异常", "rssiWarning.wav"}},
    {fullLoadCloseCar, {"满载收车任务", "takeOver.wav"}},//语音待更新
    {inertialNavigationDriftFault, {"惯导漂移故障", "takeOver.wav"}},
    {gearFault, {"档位故障", "takeOver.wav"}},
    {steeringFault, {"转向故障", "takeOver.wav"}},
    {trolleyFault, {"溜车告警", "takeOver.wav"}},
    {roadDeviation, {"道路偏离告警", "takeOver.wav"}},
    {collwarning, {"碰撞告警", "takeOver.wav"}},
    {rollWarning , {"车身姿态异常", "takeOver.wav"}},
    {speedFastWarning, {"车速过快报警", "takeOver.wav"}},
    {speedSlowWarning, {"车速过慢报警", "takeOver.wav"}},
    {throttleDrivingAbnormal, {"油门驱动力异常", "takeOver.wav"}},
    {electricBrakingAbnormal, {"电制动力异常", "takeOver.wav"}},
    {mechanicalBrakingAbnormal, {"机械制动力异常", "takeOver.wav"}},
    {loadingBrakingWithExecutionDiff, {"制动执行异常", "takeOver.wav"}},
    {enginePrimaryFault, {"车辆发动机故障", "takeOver.wav"}},
    {engineSecondaryFault, {"车辆发动机故障", "systemFaultDeal.wav"}},
    {engineThreeFault, {"车辆发动机故障", "systemFaultAttention.wav"}},
    {transmissionPrimaryFault, {"车辆变速箱故障", "takeOver.wav"}},
    {transmissionSecondaryFault, {"车辆变速箱故障", "systemFaultDeal.wav"}},
    {transmissionThreeFault, {"车辆变速箱故障", "systemFaultAttention.wav"}},
    {EBSFault, {"车辆电制动故障", "takeOver.wav"}},
    {EPSFault, {"车辆电助力转向故障", "takeOver.wav"}},
    {electricDriveSpeedLimit1, {"车辆电驱动系统故障", "systemFaultDeal.wav"}},
    {electricDriveSpeedLimit2, {"车辆电驱动系统故障", "systemFaultDeal.wav"}},
    {electricDriveSpeedLimit3, {"车辆电驱动系统故障", "systemFaultDeal.wav"}},
    {electricDriveEngineLift, {"车辆电驱动系统故障", "systemFaultAttention.wav"}},
    {electricDriveSystemEvents, {"车辆电驱动系统故障", "systemFaultAttention.wav"}},
    {noTractio, {"车辆电驱动系统故障", "takeOver.wav"}},
    {WithoutElectricityGoSlow, {"车辆电驱动系统故障", "takeOver.wav"}},
    {liftingHardwareFault, {"车辆举升系统故障", "takeOver.wav"}},
    {suspensionCylinderPrimaryFault, {"车辆悬缸故障", "takeOver.wav"}},
    {suspensionCylinderThreeFault , {"车辆悬缸故障", "systemFaultAttention.wav"}},
    {tirePrimaryFault, {"车辆轮胎故障", "takeOver.wav"}},
    {tireThreeFault, {"车辆轮胎故障", "systemFaultAttention.wav"}},
    {LowFuelOilLevel, {"车辆燃油油位低", "systemFaultAttention.wav"}},
    {automaticWeighingFault, {"车辆自动称重故障", "systemFaultAttention.wav"}},
    {selfExtinguished, {"车辆自主灭火系统故障", "takeOver.wav"}},
    {turnLightShort, {"车辆转向灯短路", "systemFaultAttention.wav"}},
    {nearLightOrHighBeamShort, {"车辆车灯短路", "systemFaultAttention.wav"}},
    {circuitAlarmLampOrFrontFogLampShort, {"车辆车灯短路", "systemFaultAttention.wav"}},
    {gasHornOrReverseLampOrReversingHornShort, {"车辆喇叭短路", "systemFaultAttention.wav"}},
    {workingLightShort, {"车辆车灯短路", "systemFaultAttention.wav"}},
    {brakeOrSideLightShort , {"车辆车灯短路", "systemFaultAttention.wav"}},
    {positionLightShort, {"车辆车灯短路", "systemFaultAttention.wav"}},
    {WiperOrAirPressureOrAuxiliaryShort, {"车辆设备短路", "systemFaultAttention.wav"}},
    {shaftSolenoidShort, {"车辆轴差电磁阀短路", "systemFaultAttention.wav"}},
    {drierOrWaterHanbaoOrExhaustShort, {"车辆排气制动故障", "systemFaultAttention.wav"}},
    {steeringOilOrEmptyFilterJamAlarm, {"车辆油滤故障", "systemFaultDeal.wav"}},
    {handBrakeSolenoidValveShort, {"车辆手制动故障", "takeOver.wav"}},
    {mechanicalBrakeHardwareFault, {"车辆机械制动故障", "takeOver.wav"}},
    {leverSensorFault, {"车辆拉杆传感器故障", "systemFaultAttention.wav"}}
};


const std::map<int, std::pair<std::string, std::string>> warningCloseMap = {
    /*报警位序号,文字提示,语音播放路径*/
//    {frontMiddleRadarFault, {"前中毫米波故障", "systemFaultDeal.wav"}},
//    {frontLidarFault, {"前激光雷达故障", "systemFaultDeal.wav"}},
//    {rearRadarFault, {"后毫米波故障", "systemFaultDeal.wav"}},
//    {rearLidarFault, {"后激光雷达故障", "systemFaultDeal.wav"}},
//    {frontLeftRadarDisconnetion, {"前左毫米波通信中断", "systemFaultDeal.wav"}},
//    {frontRightRadarDisconnetion, {"前右毫米波通信中断", "systemFaultDeal.wav"}},
//    {leftLidarDisconnetion, {"左激光雷达通信中断", "systemFaultDeal.wav"}},
//    {rightLidarDisconnetion, {"右激光雷达通信中断", "systemFaultDeal.wav"}},
    {perceptualSystemFault, {"感知模块故障检测已关闭", "perceptualSystemFaultClose.wav"}},
//    {rtkStatusAbnormal, {"差分定位状态异常", "takeOver.wav"}},
//    {imuStatusAbnormal, {"IMU状态异常", "takeOver.wav"}},
//    {gpsStatusAbnormal, {"GPS状态异常", "takeOver.wav"}},
//    {noSatelliteSignal, {"GPS状态异常", "takeOver.wav"}},
//    {gnssAge, {"系统导航状态异常", "takeOver.wav"}},
//    {gnssVariance, {"系统导航状态异常", "takeOver.wav"}},
//    {V2NDisconnection, {"与平台通信中断", "takeOver.wav"}},
//    {VCUCommunicationAbnormal, {"控制单元通信异常", "takeOver.wav"}},
//    {CGUCommunicationAbnormal, {"网关通信异常", "systemFaultDeal.wav"}},
//    {SCUCommunicationAbnormal, {"SCU通信异常", "takeOver.wav"}},
//    {CCUCommunicationAbnormal, {"主控通信异常", "takeOver.wav"}},
//    {HMICommunicationAbnormal, {"HMI通信异常", "systemFaultDeal.wav"}},
//    {CGURebootFault, {"网关重启故障", "systemFaultDeal.wav"}},
//    {CCURebootFault, {"主控重启故障", "takeOver.wav"}},
//    {HMIRebootFault, {"HMI重启故障", "systemFaultDeal.wav"}},
//    {mainPowerFault, {"主电源故障", "takeOver.wav"}},
//    {bakPowerFault, {"备用电源故障", "takeOver.wav"}},
//    {dynamicRoad, {"路径规划失败", "pathPlanningFailure.wav"}},
//    {dockingFailure, {"停靠失败", "dockingFailure.wav"}},
//    {taskFileFormat, {"任务文件异常", "taskFileAbnormal.wav"}},
//    {taskFileNoKey, {"路径文件异常", "pathFileAbnormal.wav"}},
//    {calibrationFileList, {"标定文件异常", "calibrationFileReadFailure.wav"}},
    {liftingStateAbnormal, {"举升状态异常检测已关闭", "liftingStateAbnormalClose.wav"}},
//    {trafficJam, {"道路堵塞异常", "trafficJam.wav"}},
    {emptyStateReceivedUnloadTask, {"载货状态异常检测已关闭", "loadStatusAbnormalClose.wav"}},
    {fullStateReceivedLoadTask, {"载货状态异常检测已关闭", "loadStatusAbnormalClose.wav"}},
//    {gatewayDownLoadMapFailed, {"地图文件下载失败", "downLoadMapFailed.wav"}},
//    {gatewayDownLoadTaskFileFailed, {"任务文件下载失败", "downLoadTaskFileFailed.wav"}},
//    {masterDownLoadTaskFileFailed, {"任务文件下载失败", "downLoadTaskFileFailed.wav"}},
//    {rssiWarning, {"无线通信异常", "rssiWarning.wav"}},
    {inertialNavigationDriftFault, {"惯导漂移故障检测已关闭", "inertialNavigationDriftFaultClose.wav"}},
    {gearFault, {"档位故障检测已关闭", "gearFaultClose.wav"}},
    {steeringFault, {"转向故障检测已关闭", "steeringFaultClose.wav"}},
    {trolleyFault, {"溜车故障检测已关闭", "trolleyFaultClose.wav"}},
    {roadDeviation, {"道路偏离检测已关闭", "roadDeviationClose.wav"}},
//    {collwarning, {"碰撞告警", "takeOver.wav"}},
    {rollWarning , {"侧翻预警检测已关闭", "rollWarningClose.wav"}},
    {speedFastWarning, {"车速过快检测已关闭", "speedFastWarningClose.wav"}},
//    {speedSlowWarning, {"车速过慢报警", "takeOver.wav"}},
    {throttleDrivingAbnormal, {"油门驱动力异常检测已关闭", "throttleDrivingAbnormalClose.wav"}},
    {electricBrakingAbnormal, {"电制动力异常检测已关闭", "electricBrakingAbnormalClose.wav"}},
    {mechanicalBrakingAbnormal, {"机械制动力异常检测已关闭", "mechanicalBrakingAbnormalClose.wav"}},
    {loadingBrakingWithExecutionDiff, {"制动执行异常检测已关闭", "loadingBrakingWithExecutionDiffClose.wav"}},
//    {enginePrimaryFault, {"车辆发动机故障", "takeOver.wav"}},
//    {engineSecondaryFault, {"车辆发动机故障", "systemFaultDeal.wav"}},
//    {engineThreeFault, {"车辆发动机故障", "systemFaultAttention.wav"}},
//    {transmissionPrimaryFault, {"车辆变速箱故障", "takeOver.wav"}},
//    {transmissionSecondaryFault, {"车辆变速箱故障", "systemFaultDeal.wav"}},
//    {transmissionThreeFault, {"车辆变速箱故障", "systemFaultAttention.wav"}},
//    {EBSFault, {"车辆电制动故障", "takeOver.wav"}},
//    {EPSFault, {"车辆电助力转向故障", "takeOver.wav"}},
//    {electricDriveSpeedLimit1, {"车辆电驱动系统故障", "systemFaultDeal.wav"}},
//    {electricDriveSpeedLimit2, {"车辆电驱动系统故障", "systemFaultDeal.wav"}},
//    {electricDriveSpeedLimit3, {"车辆电驱动系统故障", "systemFaultDeal.wav"}},
//    {electricDriveEngineLift, {"车辆电驱动系统故障", "systemFaultAttention.wav"}},
//    {electricDriveSystemEvents, {"车辆电驱动系统故障", "systemFaultAttention.wav"}},
    {noTractio, {"电驱动系统故障检测已关闭", "noTractioClose.wav"}},
    {WithoutElectricityGoSlow, {"电驱动系统故障检测已关闭", "noTractioClose.wav"}},
//    {liftingHardwareFault, {"车辆举升系统故障", "takeOver.wav"}},
//    {suspensionCylinderPrimaryFault, {"车辆悬缸故障", "takeOver.wav"}},
//    {suspensionCylinderThreeFault , {"车辆悬缸故障", "systemFaultAttention.wav"}},
//    {tirePrimaryFault, {"车辆轮胎故障", "takeOver.wav"}},
//    {tireThreeFault, {"车辆轮胎故障", "systemFaultAttention.wav"}},
//    {LowFuelOilLevel, {"车辆燃油油位低", "systemFaultAttention.wav"}},
//    {automaticWeighingFault, {"车辆自动称重故障", "systemFaultAttention.wav"}},
//    {selfExtinguished, {"车辆自主灭火系统故障", "takeOver.wav"}},
//    {turnLightShort, {"车辆转向灯短路", "systemFaultAttention.wav"}},
//    {nearLightOrHighBeamShort, {"车辆车灯短路", "systemFaultAttention.wav"}},
//    {circuitAlarmLampOrFrontFogLampShort, {"车辆车灯短路", "systemFaultAttention.wav"}},
//    {gasHornOrReverseLampOrReversingHornShort, {"车辆喇叭短路", "systemFaultAttention.wav"}},
//    {workingLightShort, {"车辆车灯短路", "systemFaultAttention.wav"}},
//    {brakeOrSideLightShort , {"车辆车灯短路", "systemFaultAttention.wav"}},
//    {positionLightShort, {"车辆车灯短路", "systemFaultAttention.wav"}},
//    {WiperOrAirPressureOrAuxiliaryShort, {"车辆设备短路", "systemFaultAttention.wav"}},
//    {shaftSolenoidShort, {"车辆轴差电磁阀短路", "systemFaultAttention.wav"}},
//    {drierOrWaterHanbaoOrExhaustShort, {"车辆排气制动故障", "systemFaultAttention.wav"}},
//    {steeringOilOrEmptyFilterJamAlarm, {"车辆油滤故障", "systemFaultDeal.wav"}},
//    {handBrakeSolenoidValveShort, {"车辆手制动故障", "takeOver.wav"}},
    {mechanicalBrakeHardwareFault, {"机械制动故障检测已关闭", "mechanicalBrakeHardwareFaultClose.wav"}},
//    {leverSensorFault, {"车辆拉杆传感器故障", "systemFaultAttention.wav"}}
};
#endif


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
